#include "md5.h"
#include <stdio.h>

/*
 * This file builds an executable that performs various functions related
 * to the MD5 library.  Typical compilation:
 *	gcc -o md5main -lm md5main.c md5.c
 */
static const char *const usage = "\
Usage:\n\
    md5main <string>
";

/* Main program */
int
main(int argc, char *argv[])
{
	md5_state_t state;
	md5_byte_t digest[16];
	char hex_output[16*2 + 1];
	int di;
	if (argc < 2) {
		puts(usage);
		return -1;
	}

	md5_init(&state);
	md5_append(&state, (const md5_byte_t *)argv[1], strlen(argv[1]));
	md5_finish(&state, digest);
	for (di = 0; di < 16; ++di)
	    sprintf(hex_output + di * 2, "%02x", digest[di]);
	puts(hex_output);
    return 0;
}
