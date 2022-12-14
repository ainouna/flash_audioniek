#ifndef _COMMON_H
#define _COMMON_H
#include <stdio.h>

#define DEBUG

#if defined DEBUG
extern int debug;
#define dprintf(a, b...) \
{ \
	if (debug) \
	{ \
		fprintf(stderr, a, ## b); \
	} \
}
#else
#define dprintf(a,b...) \
do \
{ \
} \
while(0)
#endif

#define errprintf(a, b...) \
{ \
	fprintf(stderr,"%s : %d : ERROR ",  __func__, __LINE__); \
	fprintf(stderr, a, ## b); \
}

#define MAX_STRING 256

#endif
// vim:ts=4
