/* $Aumix: aumix/src/common.c,v 1.9 2002/10/29 21:27:51 trevor Exp $
 *
 * aumix:  adjust audio mixer
 * copyright (c) 1993, 1996-2002 the authors--see AUTHORS file
 *
 * This file is part of aumix.
 *
 * Modified to merge with amixer by Mario Izquierdo (mariodebian) <mariodebian@gmail.com>
 *
 * Aumix is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * Aumix is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * aumix; if not, write to the Free Software Foundation, Inc., 59 Temple Place,
 * Suite 330, Boston, MA 02111-1307, USA.
 */
 
#include <stdio.h>
#include <stdlib.h>		/* getenv() */
#include <getopt.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <linux/soundcard.h>
#include <sys/soundcard.h>

#include "tmixer-oss.h"


int InitializeMixer(char *device_filename);
int MixerStatus(void);
int WriteLevel(int device, int leftright);
int ReadLevel(int device, int *leftright);
int WriteRecSrc(void);
int ReadRecSrc(void);
void ErrorExitWarn(int error, int mode);
int SetShowNoninter(int dev, char *action, char *level);

int InitializeMixer(char *device_filename) {
	if ((mixer_fd = open(device_filename, O_RDWR)) < 0)
		return EOPENMIX;
	ErrorExitWarn(MixerStatus(), 'e');
	if (!devmask)
		return EFINDDEVICE;
	return 0;
}

int MixerStatus(void) {
	if (SOUND_IOCTL(mixer_fd, SOUND_MIXER_READ_DEVMASK, &devmask) == -1)
		return EREADDEV;
	if (SOUND_IOCTL(mixer_fd, SOUND_MIXER_READ_RECMASK, &recmask) == -1)
		return EREADRECMASK;
	ErrorExitWarn(ReadRecSrc(), 'e');
	if (SOUND_IOCTL(mixer_fd, SOUND_MIXER_READ_STEREODEVS, &stereodevs) == -1)
		return EREADSTEREO;
	return 0;
}

int WriteLevel(int device, int leftright) {
	if (SOUND_IOCTL(mixer_fd, MIXER_WRITE(device), &leftright) == -1)
		return EWRITEMIX;
	return 0;
}

int ReadLevel(int device, int *leftright) {
	if (SOUND_IOCTL(mixer_fd, MIXER_READ(device), leftright) == -1)
		return EREADMIX;
	return 0;
}

int WriteRecSrc(void) {
	if (SOUND_IOCTL(mixer_fd, SOUND_MIXER_WRITE_RECSRC, &recsrc) == -1)
		return EWRITERECSRC;
	return 0;
}

int ReadRecSrc(void) {
	if (SOUND_IOCTL(mixer_fd, SOUND_MIXER_READ_RECSRC, &recsrc) == -1)
		return EREADRECSRC;
	return 0;
}

void ErrorExitWarn(int error, int mode) {
	char string[80];
	const char *errorlist[] ={"tmixer-oss:  error opening mixer",
                            "tmixer-oss:  no device found", 
                            "tmixer-oss:  SOUND_MIXER_READ_DEVMASK", 
                            "tmixer-oss:  SOUND_MIXER_READ_RECMASK", 
                            "tmixer-oss:  SOUND_MIXER_READ_RECSRC", 
                            "tmixer-oss:  SOUND_MIXER_READ_STEREODEVS", 
                            "tmixer-oss:  SOUND_MIXER_WRITE_RECSRC", 
                            "tmixer-oss:  MIXER_READ", 
                            "tmixer-oss:  MIXER_WRITE", 
                            "tmixer-oss:  mixer not open", 
                            "tmixer-oss:  unable to open settings file",
                            "tmixer-oss:  unknow commandline action"};
	if (!error)
		return;
		
	if (error > 12) {
		sprintf(string, "tmixer-oss:  unknown error %i", error);
		fprintf(stderr, "%s", string);
	} else if (error > 0) {
		fprintf(stderr, "%s", errorlist[error - 1]);
	}
	fprintf(stderr, "\n");
	if (mode == 'e') {	/* exit */
	    close(mixer_fd);
		exit(EXIT_FAILURE);
	}
	else
		return;		/* warn */
}


int SetShowNoninter(int dev, char *action, char *level) {
	/* Change or display settings from the command line. */
	char *devstr;
	int tmp;
	if ( (strcmp(action, "scontents") == 0) || (strcmp(action, "sget") == 0) ) {
		devstr = dev_name[dev];
		ErrorExitWarn(ReadLevel(dev, &tmp), 'e');
		printf("%s,volume|switch|,%i,", dev_name[dev], (tmp & 0xFF) );
		if ((tmp & 0xFF) > 0)
		    printf("on");
		else
		    printf("off");
		/*
		if ((1 << (dev)) & recmask) {
			ErrorExitWarn(ReadRecSrc(), 'e');
			printf(", %c", ((1 << dev) & recsrc ? 'R' : 'P'));
		}*/
		printf("\n");
	}
	else if ( strcmp(action, "scontrols") == 0 ) {
		devstr = dev_name[dev];
		ErrorExitWarn(ReadLevel(dev, &tmp), 'e');
		printf("%s|", dev_name[dev] );
	}
	
	else if ( strcmp(action, "sset") == 0 ) {
	    /*fprintf(stderr, "DEBUG: sset: level=%s\n", level);*/
	    if ( (strcmp(level, "mute") == 0) || (strcmp(level, "off") == 0) )
	        tmp=0;
	    else if ( (strcmp(level, "unmute") == 0) || (strcmp(level, "on") == 0) ) {
	        if (getenv("TMIXER_OSSMUTELEVEL") != NULL)
	            tmp=atoi(getenv("TMIXER_OSSMUTELEVEL"));
	        else
	            tmp=OSSUNMUTELEVEL;
	    }
	    else
		    tmp = atoi(level);
		tmp = (tmp > OSSMAXLEVEL) ? OSSMAXLEVEL : tmp;
		tmp = (tmp < 0) ? 0 : tmp;
		ErrorExitWarn(WriteLevel(dev, tmp), 'e');
		/* show channel */
		SetShowNoninter(dev, "scontents", 0);
	}
	else {
	    ErrorExitWarn(12, 'e');
	}
	return 0;
}

void tmixer_oss_help(void)
{
	printf("Usage: tmixer-oss [command]\n");
	printf("\nAvailable commands:\n");
	printf("  scontrols       show all mixer simple controls\n");
	printf("  scontents	  show contents of all mixer simple controls (default command)\n");
	printf("  sset sID P      set contents for one mixer simple control\n");
	printf("  sget sID        get contents for one mixer simple control\n");
	printf("\ntmixer-oss output format:\n");
	printf("  ChannelName,   ChannelType [volume|switch] , Volume in %% , SwitchStatus [on|off] \n\n");
}


#ifdef HAVE_TMIXER_MAIN
int tmixer_oss(int argc, char *argv[]) {
#else
int main(int argc, char *argv[]) {
#endif
	int ii, index=-1;
	
	static struct option long_option[] =
	{
		{"help", 0, NULL, 'h'},
		{"card", 1, NULL, 'c'},
		{"device", 1, NULL, 'D'},
		{"quiet", 0, NULL, 'q'},
		{"inactive", 0, NULL, 'i'},
		{"debug", 0, NULL, 'd'},
		{"nocheck", 0, NULL, 'n'},
		{"version", 0, NULL, 'v'},
		{"abstract", 1, NULL, 'a'},
		{"stdin", 0, NULL, 's'},
		{NULL, 0, NULL, 0},
	};
	
	while (1) {
		int c;

		if ((c = getopt_long(argc, argv, "hc:D:qidnva:s", long_option, NULL)) < 0)
			break;
		switch (c) {
		case 'h':
			tmixer_oss_help();
			return 0;
		case 'c':
			fprintf(stderr, "tmixer-oss: WARNING: OSS driver don't support -c (--card) option.\n");
			break;
		case 'D':
			fprintf(stderr, "tmixer-oss: WARNING: OSS driver don't support -D (--device) option.\n");
			break;
		case 'q':
			/* do nothing */
			break;
		case 'i':
			/* do nothing */
			break;
		case 'd':
			/* do nothing */
			break;
		case 'n':
			/* do nothing */
			break;
		case 'v':
			printf("tmixer-oss version (TCOS) " TCOS_VERSION "\n");
			close(mixer_fd);
			return 1;
		case 'a':
			/* do nothing */
			break;
		case 's':
			/* do nothing */
			break;
		default:
			fprintf(stderr, "tmixer-oss: Invalid switch or option needs an argument.\n");
			exit(EXIT_FAILURE);
		}
	}
	
	/*printf("argc=%d optind=%d\n", argc, optind);*/
	if (mixer_fd == -1)
		ErrorExitWarn(InitializeMixer(DEVICE_FILENAME), 'e');

	if (argc - optind <= 0) {
	    /* default OPTION empty == scontents */
		for (ii = 0; ii < SOUND_MIXER_NRDEVICES; ii++) {
			if ((1 << ii) & (devmask | recmask))
				ErrorExitWarn(SetShowNoninter(ii, "scontents", ""), 'e');
		}
		close(mixer_fd);
		exit(EXIT_SUCCESS);
	}
	if (!strcmp(argv[optind], "help")) {
		tmixer_oss_help();
		close(mixer_fd);
		exit(EXIT_SUCCESS);
		
	} else if (!strcmp(argv[optind], "scontrols") || !strcmp(argv[optind], "simple")) {
		for (ii = 0; ii < SOUND_MIXER_NRDEVICES; ii++) {
			if ((1 << ii) & (devmask | recmask))
				ErrorExitWarn(SetShowNoninter(ii, "scontrols", ""), 'e');
		}
		close(mixer_fd);
		exit(EXIT_SUCCESS);
		
	} else if (!strcmp(argv[optind], "scontents")) {
		for (ii = 0; ii < SOUND_MIXER_NRDEVICES; ii++) {
			if ((1 << ii) & (devmask | recmask))
				ErrorExitWarn(SetShowNoninter(ii, "scontents", ""), 'e');
		}
		close(mixer_fd);
		exit(EXIT_SUCCESS);
		
	} else if (!strcmp(argv[optind], "sset") || !strcmp(argv[optind], "sget")) {
	    /*fprintf(stderr, "DEBUG: argv[optind]=%s argv[optind+1]=%s argv[optind+2]=%s\n", argv[optind], argv[optind+1], argv[optind+2]);
	    fprintf(stderr, "argc=%d optind=%d strcmp=%d argv[optind]='%s'\n",argc, optind, strcmp(argv[optind], "sget"), argv[optind]);*/
	    if ( strcmp(argv[optind], "sget") ) {
	        if (argc < optind +3) {
	            fprintf(stderr, "tmixer-oss: ERROR: need value to change channel '%s'\n",argv[optind+1]);
	            close(mixer_fd);
	            exit(EXIT_FAILURE);
	        }
	    }
	    if( strcmp(argv[optind+1], "Master") == 0 )       index=0;
	    else if( strcmp(argv[optind+1], "Bass") == 0 )    index=1;
	    else if( strcmp(argv[optind+1], "Treble") == 0 )  index=2;
	    else if( strcmp(argv[optind+1], "Synth") == 0 )   index=3;
	    else if( strcmp(argv[optind+1], "PCM") == 0 )     index=4;
	    else if( strcmp(argv[optind+1], "Speaker") == 0 ) index=5;
	    else if( strcmp(argv[optind+1], "Line") == 0 )    index=6;
	    else if( strcmp(argv[optind+1], "Mic") == 0 )     index=7;
	    else if( strcmp(argv[optind+1], "CD") == 0 )      index=8;
	    else if( strcmp(argv[optind+1], "Mix") == 0 )     index=9;
	    else if( strcmp(argv[optind+1], "PCM2") == 0 )    index=10;
	    else if( strcmp(argv[optind+1], "Record") == 0 )  index=11;
	    else if( strcmp(argv[optind+1], "Input") == 0 )   index=12;
	    else if( strcmp(argv[optind+1], "Output") == 0 )  index=13;
	    else if( strcmp(argv[optind+1], "Line 1") == 0 )  index=14;
	    else if( strcmp(argv[optind+1], "Line 2") == 0 )  index=15;
	    else if( strcmp(argv[optind+1], "Line 3") == 0 )  index=16;
	    else if( strcmp(argv[optind+1], "Digital1") == 0 ) index=17;
	    else if( strcmp(argv[optind+1], "Digital2") == 0 ) index=18;
	    else if( strcmp(argv[optind+1], "Digital3") == 0 ) index=19;
	    else if( strcmp(argv[optind+1], "Phone In") == 0 ) index=20;
	    else if( strcmp(argv[optind+1], "PhoneOut") == 0 ) index=21;
	    else if( strcmp(argv[optind+1], "Video") == 0 )   index=22;
	    else if( strcmp(argv[optind+1], "Radio") == 0 )   index=23;
	    else if( strcmp(argv[optind+1], "Monitor") == 0 ) index=24;
	    
	    else
	            fprintf(stderr, "tmixer-oss: ERROR: unknow channel '%s'\n",argv[optind+1]);
	    
	    
	    if (index >= 0) {
	        /*fprintf(stderr, "DEBUG: argv[optind]=%s argv[optind+1]=%s argv[optind+2]=%s\n", argv[optind], argv[optind+1], argv[optind+2]);*/
	        ErrorExitWarn(SetShowNoninter(index, argv[optind], argv[optind+2] ), 'e');
	    }
	    
		close(mixer_fd);
		exit(EXIT_SUCCESS);
	} else {
		fprintf(stderr, "tmixer-oss: ERROR: Unknown command '%s'...\n", argv[optind]);
	}
	
	ErrorExitWarn(12, 'e');
	close(mixer_fd);
	exit(EXIT_FAILURE);
}
