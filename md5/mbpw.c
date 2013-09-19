#include "md5.h"
#include "mbpw.h"
#include <stdio.h>

static
char * s2h(const char * string) {
	int n = strlen(string);
	char * msg = (char*)malloc((2*n)+1);
	int di;
	for (di = 0 ; di < n; ++di)
		sprintf((char*)msg + di * 2, "%02x", string[di]);
	return msg;
}
static
char * munge(const char * string) {
	char * msg;
	int n,i,j,k;
	n = strlen(string);
	i = 0;
	j = n-1;
	k = 0;
	msg = (char *)malloc(strlen(string)+1);

	if (n < 2) {
		strcpy(msg,string);
		return msg;
	}
	while (i < j) {
		msg[k++] = string[j--];
		msg[k++] = string[i++];
		if (i == j)
			msg[k] = string[i];
	}
	msg[strlen(string)] = 0;
	return msg;
}

char *
mbpwcreate(const char *host,
		   const char *fname,
		   const char *lname,
		   const char *email)
{
	md5_state_t state;
	md5_byte_t digest[16];
	char * hex_output;
	int di;
	char * hhost = s2h(host);
	char * hfname = s2h(fname);
	char * hlname = s2h(lname);
	char * hemail = s2h(email);
	char * msg;
	char * msgh;

	di = strlen(hhost) + strlen(hfname) + strlen(hlname) + strlen(hemail);
	msgh = (char*)malloc(di + 1);
	memset(msgh,0,di+1);
	strcat(msgh,hhost);
	strcat(msgh,hfname);
	strcat(msgh,hlname);
	strcat(msgh,hemail);
	msg = munge(msgh);

	md5_init(&state);
	md5_append(&state, (const md5_byte_t *)msg, strlen(msg));
	md5_finish(&state, digest);

	free(hhost);free(hfname);free(hlname);free(hemail);
	free(msgh);free(msg);
	hex_output = (char * ) malloc((int)(16*2 + 1));

	for (di = 0; di < 16; ++di)
	    sprintf((char*)hex_output + di * 2, "%02x", digest[di]);
    return (char * )hex_output;
}
