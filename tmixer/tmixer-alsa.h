/*
 *  Headers of tmixer-alsa.c
 *
 *
 *
 */




/** COPY FROM ../include/version.h *****************************/
#define SND_UTIL_VERSION_STR    "1.0.16"



#define LEVEL_BASIC		(1<<0)
#define LEVEL_INACTIVE		(1<<1)
#define LEVEL_ID		(1<<2)

#include <alloca.h>
void *alloca( size_t  size );

#include <math.h>
double rint(double x);


#include <strings.h>
int strncasecmp(const char *s1, const char *s2, size_t n);
