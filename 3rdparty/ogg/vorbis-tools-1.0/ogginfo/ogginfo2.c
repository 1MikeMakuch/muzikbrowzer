/* Ogginfo
 *
 * A tool to describe ogg file contents and metadata.
 *
 * Copyright 2002 Michael Smith <msmith@layrinth.net.au>
 * Licensed under the GNU GPL, distributed with this program.
 */


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <getopt.h>

#include <ogg/ogg.h>
#include <vorbis/codec.h>

#include <locale.h>
#include "utf8.h"
#include "i18n.h"

#define CHUNK 4500

/* Different implementations have different format strings for 64 bit ints. */
#ifdef _WIN32
#define INT64FORMAT "%I64d"
#else
#define INT64FORMAT "%Ld"
#endif

struct vorbis_release {
    char *vendor_string;
    char *desc;
} releases[] = {
        {"Xiphophorus libVorbis I 20000508", "1.0 beta 1 or beta 2"},
        {"Xiphophorus libVorbis I 20001031", "1.0 beta 3"},
        {"Xiphophorus libVorbis I 20010225", "1.0 beta 4"},
        {"Xiphophorus libVorbis I 20010615", "1.0 rc1"},
        {"Xiphophorus libVorbis I 20010813", "1.0 rc2"},
        {"Xiphophorus libVorbis I 20011217", "1.0 rc3"},
        {"Xiphophorus libVorbis I 20011231", "1.0 rc3"},
        {"Xiph.Org libVorbis I 20020717", "1.0"},
        {NULL, NULL},
    };


/* TODO:
 *
 * - detect decreasing granulepos
 * - detect violations of muxing constraints
 * - better EOS detection (when EOS not explicitly set)
 * - detect granulepos 'gaps' (possibly vorbis-specific).
 * - check for serial number == (unsigned)-1 (will break some tools?)
 * - more options (e.g. less or more verbose)
 */

typedef struct _stream_processor {
    void (*process_page)(struct _stream_processor *, ogg_page *);
    void (*process_end)(struct _stream_processor *);
    int isillegal;
    int shownillegal;
    int isnew;
    long seqno;
    int lostseq;

    int start;
    int end;

    int num;
    char *type;

    ogg_uint32_t serial; /* must be 32 bit unsigned */
    ogg_stream_state os;
    void *data;
} stream_processor;

typedef struct {
    stream_processor *streams;
    int allocated;
    int used;

    int in_headers;
} stream_set;

typedef struct {
    vorbis_info vi;
    vorbis_comment vc;

    long bytes;
    ogg_int64_t lastgranulepos;

    int doneheaders;
} misc_vorbis_info;

static int printinfo = 1;
static int printwarn = 1;
static int verbose = 1;

static stream_set *create_stream_set(void) {
    stream_set *set = calloc(1, sizeof(stream_set));

    set->streams = calloc(5, sizeof(stream_processor));
    set->allocated = 5;
    set->used = 0;

    return set;
}

static void info(char *format, ...) 
{
    va_list ap;

    if(!printinfo)
        return;

    va_start(ap, format);
    vfprintf(stdout, format, ap);
    va_end(ap);
}

static void warn(char *format, ...) 
{
    va_list ap;

    if(!printwarn)
        return;

    va_start(ap, format);
    vfprintf(stdout, format, ap);
    va_end(ap);
}

static void error(char *format, ...) 
{
    va_list ap;

    va_start(ap, format);
    vfprintf(stdout, format, ap);
    va_end(ap);
}

static void vorbis_process(stream_processor *stream, ogg_page *page )
{
    ogg_packet packet;
    misc_vorbis_info *inf = stream->data;
    int i, header=0;
    int k;

    ogg_stream_pagein(&stream->os, page);

    while(ogg_stream_packetout(&stream->os, &packet) > 0) {
        if(inf->doneheaders < 3) {
            if(vorbis_synthesis_headerin(&inf->vi, &inf->vc, &packet) < 0) {
                warn(_("Warning: Could not decode vorbis header "
                       "packet - invalid vorbis stream (%d)\n"), stream->num);
                continue;
            }
            header = 1;
            inf->doneheaders++;
            if(inf->doneheaders == 3) {
                if(ogg_page_granulepos(page) != 0 || ogg_stream_packetpeek(&stream->os, NULL) == 1)
                    warn(_("Warning: Vorbis stream %d does not have headers "
                           "correctly framed. Terminal header page contains "
                           "additional packets or has non-zero granulepos\n"),
                            stream->num);
                info(_("Vorbis headers parsed for stream %d, "
                       "information follows...\n"), stream->num);

                info(_("Version: %d\n"), inf->vi.version);
                k = 0;
                while(releases[k].vendor_string) {
                    if(!strcmp(inf->vc.vendor, releases[k].vendor_string)) {
                        info(_("Vendor: %s (%s)\n"), inf->vc.vendor, 
                                    releases[k].desc);
                        break;
                    }
                    k++;
                }
                if(!releases[k].vendor_string)
                    info(_("Vendor: %s\n"), inf->vc.vendor);
                info(_("Channels: %d\n"), inf->vi.channels);
                info(_("Rate: %ld\n\n"), inf->vi.rate);

                if(inf->vi.bitrate_nominal > 0)
                    info(_("Nominal bitrate: %f kb/s\n"), 
                            (double)inf->vi.bitrate_nominal / 1000.0);
                else
                    info(_("Nominal bitrate not set\n"));

                if(inf->vi.bitrate_upper > 0)
                    info(_("Upper bitrate: %f kb/s\n"), 
                            (double)inf->vi.bitrate_upper / 1000.0);
                else
                    info(_("Upper bitrate not set\n"));

                if(inf->vi.bitrate_lower > 0)
                    info(_("Lower bitrate: %f kb/s\n"), 
                            (double)inf->vi.bitrate_lower / 1000.0);
                else
                    info(_("Lower bitrate not set\n"));

                if(inf->vc.comments > 0)
                    info(_("User comments section follows...\n"));

                for(i=0; i < inf->vc.comments; i++) {
                    char *sep = strchr(inf->vc.user_comments[i], '=');
                    char *decoded;
                    int j;
                    int broken = 0;
                    unsigned char *val;
                    int bytes;
                    int remaining;

                    if(sep == NULL) {
                        warn(_("Warning: Comment %d in stream %d is invalidly "
                               "formatted, does not contain '=': \"%s\"\n"), 
                               i, stream->num, inf->vc.user_comments[i]);
                        continue;
                    }

                    for(j=0; j < sep-inf->vc.user_comments[i]; j++) {
                        if(inf->vc.user_comments[i][j] < 0x20 ||
                           inf->vc.user_comments[i][j] > 0x7D) {
                            warn(_("Warning: Invalid comment fieldname in "
                                   "comment %d (stream %d): \"%s\"\n"),
                                    i, stream->num, inf->vc.user_comments[i]);
                            broken = 1;
                            break;
                        }
                    }

                    if(broken)
                        continue;

                    val = inf->vc.user_comments[i];

                    j = sep-inf->vc.user_comments[i]+1;
                    while(j < inf->vc.comment_lengths[i])
                    {
                        remaining = inf->vc.comment_lengths[i] - j;
                        if((val[j] & 0x80) == 0)
                            bytes = 1;
                        else if((val[j] & 0x40) == 0x40) {
                            if((val[j] & 0x20) == 0)
                                bytes = 2;
                            else if((val[j] & 0x10) == 0)
                                bytes = 3;
                            else if((val[j] & 0x08) == 0)
                                bytes = 4;
                            else if((val[j] & 0x04) == 0)
                                bytes = 5;
                            else if((val[j] & 0x02) == 0)
                                bytes = 6;
                            else {
                                warn(_("Warning: Illegal UTF-8 sequence in "
                                       "comment %d (stream %d): length "
                                       "marker wrong\n"),
                                        i, stream->num);
                                broken = 1;
                                break;
                            }
                        }
                        else {
                            warn(_("Warning: Illegal UTF-8 sequence in comment "
                                   "%d (stream %d): length marker wrong\n"),
                                    i, stream->num);
                            broken = 1;
                            break;
                        }

                        if(bytes > remaining) {
                            warn(_("Warning: Illegal UTF-8 sequence in comment "
                                   "%d (stream %d): too few bytes\n"),
                                    i, stream->num);
                            broken = 1;
                            break;
                        }

                        switch(bytes) {
                            case 1:
                                /* No more checks needed */
                                break;
                            case 2:
                                if((val[j+1] & 0xC0) != 0x80)
                                    broken = 1;
                                if((val[j] & 0xFE) == 0xC0)
                                    broken = 1;
                                break;
                            case 3:
                                if(!((val[j] == 0xE0 && val[j+1] >= 0xA0 && 
                                        val[j+1] <= 0xBF && 
                                        (val[j+2] & 0xC0) == 0x80) ||
                                   (val[j] >= 0xE1 && val[j] <= 0xEC &&
                                        (val[j+1] & 0xC0) == 0x80 &&
                                        (val[j+2] & 0xC0) == 0x80) ||
                                   (val[j] == 0xED && val[j+1] >= 0x80 &&
                                        val[j+1] <= 0x9F &&
                                        (val[j+2] & 0xC0) == 0x80) ||
                                   (val[j] >= 0xEE && val[j] <= 0xEF &&
                                        (val[j+1] & 0xC0) == 0x80 &&
                                        (val[j+2] & 0xC0) == 0x80)))
                                    broken = 1;
                                if(val[j] == 0xE0 && (val[j+1] & 0xE0) == 0x80)
                                    broken = 1;
                                break;
                            case 4:
                                if(!((val[j] == 0xF0 && val[j+1] >= 0x90 &&
                                        val[j+1] <= 0xBF &&
                                        (val[j+2] & 0xC0) == 0x80 &&
                                        (val[j+3] & 0xC0) == 0x80) ||
                                     (val[j] >= 0xF1 && val[j] <= 0xF3 &&
                                        (val[j+1] & 0xC0) == 0x80 &&
                                        (val[j+2] & 0xC0) == 0x80 &&
                                        (val[j+3] & 0xC0) == 0x80) ||
                                     (val[j] == 0xF4 && val[j+1] >= 0x80 &&
                                        val[j+1] <= 0x8F &&
                                        (val[j+2] & 0xC0) == 0x80 &&
                                        (val[j+3] & 0xC0) == 0x80)))
                                    broken = 1;
                                if(val[j] == 0xF0 && (val[j+1] & 0xF0) == 0x80)
                                    broken = 1;
                                break;
                            /* 5 and 6 aren't actually allowed at this point*/  
                            case 5:
                                broken = 1;
                                break;
                            case 6:
                                broken = 1;
                                break;
                        }

                        if(broken) {
                            warn(_("Warning: Illegal UTF-8 sequence in comment "
                                   "%d (stream %d): invalid sequence\n"),
                                    i, stream->num);
                            broken = 1;
                            break;
                        }

                        j += bytes;
                    }

                    if(!broken) {
                        if(utf8_decode(sep+1, &decoded) < 0) {
                            warn(_("Warning: Failure in utf8 decoder. This "
                                   "should be impossible\n"));
                            continue;
                        }
                        *sep = 0;
                        info("\t%s=%s\n", inf->vc.user_comments[i], decoded);
                        free(decoded);
                    }
                }
            }
        }
    }

    if(!header) {
        ogg_int64_t gp = ogg_page_granulepos(page);
        if(gp > 0) {
            if(gp < inf->lastgranulepos)
                warn(_("Warning: granulepos in stream %d decreases from " 
                        INT64FORMAT " to " INT64FORMAT "\n"), stream->num,
                        inf->lastgranulepos, gp);
            inf->lastgranulepos = gp;
        }
        inf->bytes += page->header_len + page->body_len;
    }
}

static void vorbis_end(stream_processor *stream) 
{
    misc_vorbis_info *inf = stream->data;
    long minutes, seconds;
    double bitrate, time;

    time = (double)inf->lastgranulepos / inf->vi.rate;
    minutes = (long)time / 60;
    seconds = (long)time - minutes*60;
    bitrate = inf->bytes*8 / time / 1000.0;

    info(_("Vorbis stream %d:\n"
           "\tTotal data length: %ld bytes\n"
           "\tPlayback length: %ldm:%02lds\n"
           "\tAverage bitrate: %f kbps\n"), 
            stream->num,inf->bytes, minutes, seconds, bitrate);

    vorbis_comment_clear(&inf->vc);
    vorbis_info_clear(&inf->vi);

    free(stream->data);
}

static void process_null(stream_processor *stream, ogg_page *page)
{
    /* This is for invalid streams. */
}

static void process_other(stream_processor *stream, ogg_page *page )
{
    ogg_packet packet;

    ogg_stream_pagein(&stream->os, page);

    while(ogg_stream_packetout(&stream->os, &packet) > 0) {
        /* Should we do anything here? Currently, we don't */
    }
}
        

static void free_stream_set(stream_set *set)
{
    int i;
    for(i=0; i < set->used; i++) {
        if(!set->streams[i].end) {
            warn(_("Warning: EOS not set on stream %d\n"), 
                    set->streams[i].num);
            if(set->streams[i].process_end)
                set->streams[i].process_end(&set->streams[i]);
        }
        ogg_stream_clear(&set->streams[i].os);
    }

    free(set->streams);
    free(set);
}

static int streams_open(stream_set *set)
{
    int i;
    int res=0;
    for(i=0; i < set->used; i++) {
        if(!set->streams[i].end)
            res++;
    }

    return res;
}

static void null_start(stream_processor *stream)
{
    stream->process_end = NULL;
    stream->type = "invalid";
    stream->process_page = process_null;
}

static void other_start(stream_processor *stream, char *type)
{
    if(type)
        stream->type = type;
    else
        stream->type = "unknown";
    stream->process_page = process_other;
    stream->process_end = NULL;
}

static void vorbis_start(stream_processor *stream)
{
    misc_vorbis_info *info;

    stream->type = "vorbis";
    stream->process_page = vorbis_process;
    stream->process_end = vorbis_end;

    stream->data = calloc(1, sizeof(misc_vorbis_info));

    info = stream->data;

    vorbis_comment_init(&info->vc);
    vorbis_info_init(&info->vi);

}

static stream_processor *find_stream_processor(stream_set *set, ogg_page *page)
{
    ogg_uint32_t serial = ogg_page_serialno(page);
    int i, found = 0;
    int invalid = 0;
    stream_processor *stream;

    for(i=0; i < set->used; i++) {
        if(serial == set->streams[i].serial) {
            /* We have a match! */
            found = 1;
            stream = &(set->streams[i]);

            set->in_headers = 0;
            /* if we have detected EOS, then this can't occur here. */
            if(stream->end) {
                stream->isillegal = 1;
                return stream;
            }

            stream->isnew = 0;
            stream->start = ogg_page_bos(page);
            stream->end = ogg_page_eos(page);
            stream->serial = serial;
            return stream;
        }
    }

    /* If there are streams open, and we've reached the end of the
     * headers, then we can't be starting a new stream.
     * XXX: might this sometimes catch ok streams if EOS flag is missing,
     * but the stream is otherwise ok?
     */
    if(streams_open(set) && !set->in_headers)
        invalid = 1;

    set->in_headers = 1;

    if(set->allocated < set->used)
        stream = &set->streams[set->used];
    else {
        set->allocated += 5;
        set->streams = realloc(set->streams, sizeof(stream_processor)*
                set->allocated);
        stream = &set->streams[set->used];
    }
    set->used++;
    stream->num = set->used; /* We count from 1 */

    stream->isnew = 1;
    stream->isillegal = invalid;

    {
        int res;
        ogg_packet packet;

        /* We end up processing the header page twice, but that's ok. */
        ogg_stream_init(&stream->os, serial);
        ogg_stream_pagein(&stream->os, page);
        res = ogg_stream_packetout(&stream->os, &packet);
        if(res <= 0) {
            warn(_("Warning: Invalid header page, no packet found\n"));
            null_start(stream);
        }
        else if(packet.bytes >= 7 && memcmp(packet.packet, "\001vorbis", 7)==0)
            vorbis_start(stream);
        else if(packet.bytes >= 8 && memcmp(packet.packet, "OggMIDI\0", 8)==0) 
            other_start(stream, "MIDI");
        else
            other_start(stream, NULL);

        res = ogg_stream_packetout(&stream->os, &packet);
        if(res > 0) {
            warn(_("Warning: Invalid header page in stream %d, "
                              "contains multiple packets\n"), stream->num);
        }

        /* re-init, ready for processing */
        ogg_stream_clear(&stream->os);
        ogg_stream_init(&stream->os, serial);
   }

   stream->start = ogg_page_bos(page);
   stream->end = ogg_page_eos(page);
   stream->serial = serial;

   return stream;
}

static int get_next_page(FILE *f, ogg_sync_state *sync, ogg_page *page, 
        ogg_int64_t *written)
{
    int ret;
    char *buffer;
    int bytes;

    while((ret = ogg_sync_pageout(sync, page)) <= 0) {
        if(ret < 0)
            warn(_("Warning: Hole in data found at approximate offset "
                   INT64FORMAT " bytes. Corrupted ogg.\n"), *written);

        buffer = ogg_sync_buffer(sync, CHUNK);
        bytes = fread(buffer, 1, CHUNK, f);
        if(bytes <= 0) {
            ogg_sync_wrote(sync, 0);
            return 0;
        }
        ogg_sync_wrote(sync, bytes);
        *written += bytes;
    }

    return 1;
}

static void process_file(char *filename) {
    FILE *file = fopen(filename, "rb");
    ogg_sync_state sync;
    ogg_page page;
    stream_set *processors = create_stream_set();
    int gotpage = 0;
    ogg_int64_t written = 0;

    if(!file) {
        error(_("Error opening input file \"%s\": %s\n"), filename,
                    strerror(errno));
        return;
    }

    printf(_("Processing file \"%s\"...\n\n"), filename);

    ogg_sync_init(&sync);

    while(get_next_page(file, &sync, &page, &written)) {
        stream_processor *p = find_stream_processor(processors, &page);
        gotpage = 1;

        if(!p) {
            error(_("Could not find a processor for stream, bailing\n"));
            return;
        }

        if(p->isillegal && !p->shownillegal) {
            warn(_("Warning: illegally placed page(s) for logical stream %d\n"
                   "This indicates a corrupt ogg file.\n"), p->num);
            p->shownillegal = 1;
            continue;
        }

        if(p->isnew) {
            info(_("New logical stream (#%d, serial: %08x): type %s\n"), 
                    p->num, p->serial, p->type);
            if(!p->start)
                warn(_("Warning: stream start flag not set on stream %d\n"),
                        p->num);
        }
        else if(p->start)
            warn(_("Warning: stream start flag found in mid-stream "
                      "on stream %d\n"), p->num);

        if(p->seqno++ != ogg_page_pageno(&page)) {
            if(!p->lostseq) 
                warn(_("Warning: sequence number gap in stream %d. Got page "
                       "%ld when expecting page %ld. Indicates missing data.\n"
                       ), p->num, ogg_page_pageno(&page), p->seqno - 1);
            p->seqno = ogg_page_pageno(&page);
            p->lostseq = 1;
        }
        else
            p->lostseq = 0;

        if(!p->isillegal) {
            p->process_page(p, &page);

            if(p->end) {
                if(p->process_end)
                    p->process_end(p);
                info(_("Logical stream %d ended\n"), p->num);
                p->isillegal = 1;
            }
        }
    }

    if(!gotpage)
        error(_("Error: No ogg data found in file \"%s\".\n"
                "Input probably not ogg.\n"), filename);

    free_stream_set(processors);

    ogg_sync_clear(&sync);

    fclose(file);
}

static void usage(void) {
    printf(_("ogginfo 1.0\n"
             "(c) 2002 Michael Smith <msmith@labyrinth.net.au>\n"
             "\n"
             "Usage: ogginfo [flags] files1.ogg [file2.ogg ... fileN.ogg]\n"
             "Flags supported:\n"
             "\t-h Show this help message\n"
             "\t-q Make less verbose. Once will remove detailed informative\n"
             "\t   messages, two will remove warnings\n"
             "\t-v Make more verbose. This may enable more detailed checks\n"
             "\t   for some stream types.\n\n"));
}

int main(int argc, char **argv) {
    int f, ret;

    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGE, LOCALEDIR);
    textdomain(PACKAGE);

    if(argc < 2) {
        fprintf(stderr, 
                _("Usage: ogginfo [flags] file1.ogg [file2.ogg ... fileN.ogg]\n"
                  "\n"
                  "Ogginfo is a tool for printing information about ogg files\n"
                  "and for diagnosing problems with them.\n"
                  "Full help shown with \"ogginfo -h\".\n"));
        exit(1);
    }

    while((ret = getopt(argc, argv, "hvq")) >= 0) {
        switch(ret) {
            case 'h':
                usage();
                return 0;
            case 'v':
                verbose++;
                break;
            case 'q':
                verbose--;
                break;
        }
    }

    if(verbose < 1)
        printinfo = 0;
    if(verbose < 0) 
        printwarn = 0;

    if(optind >= argc) {
        fprintf(stderr, 
                _("No input files specified. \"ogginfo -h\" for help\n"));
        return 1;
    }

    for(f=optind; f < argc; f++) {
        process_file(argv[f]);
    }

    return 0;
}

