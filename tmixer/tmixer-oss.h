/*
 *
 *  Headers of tmixer-oss.c
 *
 *
 */



#define OSSMAXLEVEL 100		/* highest level permitted by OSS drivers */

#define OSSUNMUTELEVEL 85	/* level of unmute (no way to get old level) */
/* you can overwrite this level with TMIXER_OSSMUTELEVEL environment var */


#define SOUND_IOCTL(a,b,c)	ioctl(a,b,c)

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

enum {
	ENOERROR,
	EOPENMIX,		/* trouble opening mixer device */
	EFINDDEVICE,	/* no device found */
	EREADDEV,		/* SOUND_MIXER_READ_DEVMASK */
	EREADRECMASK,	/* SOUND_MIXER_READ_RECMASK */
	EREADRECSRC,	/* SOUND_MIXER_READ_RECSRC */
	EREADSTEREO,	/* SOUND_MIXER_READ_STEREODEVS */
	EWRITERECSRC,	/* SOUND_MIXER_WRITE_RECSRC */
	EREADMIX,		/* MIXER_READ */
	EWRITEMIX,		/* MIXER_WRITE */
	ENOTOPEN,		/* mixer not open */
	EFILE			/* unable to open settings file */
};



/* name of mixer device file */
#define DEVICE_FILENAME "/dev/mixer"

int mixer_fd = -1;
int devmask = 0;
int recmask = 0;
int recsrc = 0;
int stereodevs = 0;


char *dev_name[SOUND_MIXER_NRDEVICES] = { \
	"Master", \
	"Bass", \
	"Treble", \
	"Synth", \
	"PCM", \
	"Speaker", \
	"Line", \
	"Mic", \
	"CD", \
	"Mix", \
	"PCM2", \
	"Record", \
	"Input", \
	"Output", \
	"Line 1", \
	"Line 2", \
	"Line 3", \
	"Digital1", \
	"Digital2", \
	"Digital3", \
	"Phone In", \
	"PhoneOut", \
	"Video", \
	"Radio", \
	"Monitor" };



