#include <stdio.h>
#include "mbpw.h"

void usage() {
	printf("usage: hostid firstname lastname emailaddress\n");
}

int
main(int argc, char *argv[])
{
	char * pw = NULL;

	if (argc < 5) {
		usage();
		return -1;
	}

	pw = mbpwcreate(argv[1],argv[2],argv[3],argv[4]);

	printf("%s\n",pw);

	free(pw);

	return 0;

}
