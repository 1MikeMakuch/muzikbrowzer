#ifndef _mbpw_h_
#define _mbpw_h_
#include <string.h>
#include <malloc.h>

#ifdef __cplusplus
extern "C" 
{    
#endif

char * mbpwcreate(const char *host,
				  const char *firstname,
				  const char *lastname,
				  const char *email);
#ifdef __cplusplus
}  /* end extern "C" */
#endif

#define MBPW_HOSTID		"HostId:"
#define MBPW_FNAME		"Firstname:"
#define MBPW_LNAME		"Lastname:"
#define MBPW_EMAIL		"Email:"
#define MBPW_PW			"Password:"
#define MBPW_EXP		"Trial Expires:"
#define MBPW_MBVERSION	"mb Version:"

#endif

