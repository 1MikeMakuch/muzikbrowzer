
//#include <ace/Hash_Map_Manager.h>
#include "Genres.h"
#include "MyString.h"
#include "id3/misc_support.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
static GenreTable gdb;
static GenreTable rgdb;

void
Genre_init() {
	gdb.SetAt("blues","0");
	gdb.SetAt("classic rock","1");
	gdb.SetAt("country","2");
	gdb.SetAt("dance","3");
	gdb.SetAt("disco","4");
	gdb.SetAt("funk","5");
	gdb.SetAt("grunge","6");
	gdb.SetAt("hip-hop","7");
	gdb.SetAt("jazz","8");
	gdb.SetAt("metal","9");
	gdb.SetAt("new age","10");
	gdb.SetAt("oldies","11");
	gdb.SetAt("other","12");
	gdb.SetAt("pop","13");
	gdb.SetAt("r&b","14");
	gdb.SetAt("rap","15");
	gdb.SetAt("reggae","16");
	gdb.SetAt("rock","17");
	gdb.SetAt("techno","18");
	gdb.SetAt("industrial","19");
	gdb.SetAt("alternative","20");
	gdb.SetAt("ska","21");
	gdb.SetAt("death metal","22");
	gdb.SetAt("pranks","23");
	gdb.SetAt("soundtrack","24");
	gdb.SetAt("euro-techno","25");
	gdb.SetAt("ambient","26");
	gdb.SetAt("trip-hop","27");
	gdb.SetAt("vocal","28");
	gdb.SetAt("jazz+funk","29");
	gdb.SetAt("fusion","30");
	gdb.SetAt("trance","31");
	gdb.SetAt("classical","32");
	gdb.SetAt("instrumental","33");
	gdb.SetAt("acid","34");
	gdb.SetAt("house","35");
	gdb.SetAt("game","36");
	gdb.SetAt("sound clip","37");
	gdb.SetAt("gospel","38");
	gdb.SetAt("noise","39");
	gdb.SetAt("alternrock","40");
	gdb.SetAt("bass","41");
	gdb.SetAt("soul","42");
	gdb.SetAt("punk","43");
	gdb.SetAt("space","44");
	gdb.SetAt("meditative","45");
	gdb.SetAt("instrumental pop","46");
	gdb.SetAt("instrumental rock","47");
	gdb.SetAt("ethnic","48");
	gdb.SetAt("gothic","49");
	gdb.SetAt("darkwave","50");
	gdb.SetAt("techno-industrial","51");
	gdb.SetAt("electronic","52");
	gdb.SetAt("pop-folk","53");
	gdb.SetAt("eurodance","54");
	gdb.SetAt("dream","55");
	gdb.SetAt("southern rock","56");
	gdb.SetAt("comedy","57");
	gdb.SetAt("cult","58");
	gdb.SetAt("gangsta","59");
	gdb.SetAt("top 40","60");
	gdb.SetAt("christian","61");
	gdb.SetAt("pop/funk","62");
	gdb.SetAt("jungle","63");
	gdb.SetAt("native american","64");
	gdb.SetAt("cabaret","65");
	gdb.SetAt("new wave","66");
	gdb.SetAt("psychadelic","67");
	gdb.SetAt("rave","68");
	gdb.SetAt("showtunes","69");
	gdb.SetAt("trailer","70");
	gdb.SetAt("lo-fi","71");
	gdb.SetAt("tribal","72");
	gdb.SetAt("acid punk","73");
	gdb.SetAt("acid jazz","74");
	gdb.SetAt("polka","75");
	gdb.SetAt("retro","76");
	gdb.SetAt("musical","77");
	gdb.SetAt("rock & roll","78");
	gdb.SetAt("hard rock","79");
	gdb.SetAt("folk","80");
	gdb.SetAt("folk-rock","81");
	gdb.SetAt("national folk","82");
	gdb.SetAt("swing","83");
	gdb.SetAt("fast fusion","84");
	gdb.SetAt("bebob","85");
	gdb.SetAt("latin","86");
	gdb.SetAt("revival","87");
	gdb.SetAt("celtic","88");
	gdb.SetAt("bluegrass","89");
	gdb.SetAt("avantgarde","90");
	gdb.SetAt("gothic rock","91");
	gdb.SetAt("progressive rock","92");
	gdb.SetAt("psychedelic rock","93");
	gdb.SetAt("symphonic rock","94");
	gdb.SetAt("slow rock","95");
	gdb.SetAt("big band","96");
	gdb.SetAt("chorus","97");
	gdb.SetAt("easy listening","98");
	gdb.SetAt("acoustic","99");
	gdb.SetAt("humour","100");
	gdb.SetAt("speech","101");
	gdb.SetAt("chanson","102");
	gdb.SetAt("opera","103");
	gdb.SetAt("chamber music","104");
	gdb.SetAt("sonata","105");
	gdb.SetAt("symphony","106");
	gdb.SetAt("booty bass","107");
	gdb.SetAt("primus","108");
	gdb.SetAt("porn groove","109");
	gdb.SetAt("satire","110");
	gdb.SetAt("slow jam","111");
	gdb.SetAt("club","112");
	gdb.SetAt("tango","113");
	gdb.SetAt("samba","114");
	gdb.SetAt("folklore","115");
	gdb.SetAt("ballad","116");
	gdb.SetAt("power ballad","117");
	gdb.SetAt("rhythmic soul","118");
	gdb.SetAt("freestyle","119");
	gdb.SetAt("duet","120");
	gdb.SetAt("punk rock","121");
	gdb.SetAt("drum solo","122");
	gdb.SetAt("a capella","123");
	gdb.SetAt("euro-house","124");
	gdb.SetAt("dance hall","125");
//	gdb.SetAt("christmas","1000");
//	gdb.SetAt("children","1001");

    POSITION pos;
    CString key;
    CString val;
    for( pos = gdb.GetStartPosition(); pos != NULL; ) {
        gdb.GetNextAssoc(pos, key, val);
        rgdb.SetAt(val, key);
    }
}

CString
Genre_normalize(const CString genre) {
	CString tmp, tmp2;
	tmp = string_replace(genre, '(', "");
	tmp = string_replace(tmp, ')', "");

    if (rgdb.Lookup(tmp, tmp2) != 0) {
        tmp = tmp2;
    }
	return string_downcase(tmp);
}

int
Genre_getInt(CString genre) {
    CString val;
    if (gdb.Lookup(genre, val) != 0) {
        return atoi((LPCTSTR) val);
    } else {
        return -1;
    }
}

static void
insertSort(CStringList &list, CString &string) {
    POSITION pos = list.GetHeadPosition();
    if (pos == NULL) {
        list.AddTail(string);
        return;
    }
    bool inserted = false;
    for (pos = list.GetHeadPosition(); pos != NULL; ) {
        CString lstring = list.GetAt(pos);
        if (string.CompareNoCase(lstring) == 1) {
            list.InsertBefore(pos, string);
            inserted = true;
            break;
        }
        list.GetNext(pos);
    }
    if (!inserted)
        list.AddTail(string);
}
void
Genre_getGenres(CStringList & glist) {
    POSITION pos;
    for (pos = gdb.GetStartPosition(); pos != NULL;) {
        CString genre, val;
        gdb.GetNextAssoc(pos, genre, val);
        insertSort(glist, genre);
    }
    return;
}

void
Genre_addGenre(ID3_Tag & id3, const char * genre) {

  ID3_Frame* frame = NULL;
  if (genre)
  {
    ID3_RemoveGenres(&id3);

    if (NULL == id3.Find(ID3FID_CONTENTTYPE))
    {
      frame = new ID3_Frame(ID3FID_CONTENTTYPE);
      if (NULL != frame)
      {
        int g = Genre_getInt(genre);
        char sGenre[100];
        if (g > 0) {
            sprintf(sGenre, "(%d)", g);
        } else {
            sprintf(sGenre, "%s", genre);
        }
        frame->GetField(ID3FN_TEXT)->Set(sGenre);
        id3.AttachFrame(frame);
      }
    }
  }
  
  return;
}
