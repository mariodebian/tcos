/*
* common.c part of tcosxmlrpc
* Copyright (C) 2006,2007,2008  mariodebian at gmail
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "common.h"
#include "debug.h"


struct ip_address check_ip_address(char *data) {
  struct ip_address ip;
  /*
  *  split ip address with 4 dots
  *  join ip address in ip.ipstr
  *  and compare with original data
  *
  */
  sscanf(data, "%d.%d.%d.%d", &ip.data[0], &ip.data[1], &ip.data[2], &ip.data[3]);

  snprintf(ip.ipstr, IP_LENGHT, "%d.%d.%d.%d", ip.data[0], ip.data[1], ip.data[2], ip.data[3]);

  dbgtcos("tcosxmlrpc::is_ip() comparing %s with %s ...\n", data, ip.ipstr);

  if ( strcmp(data, ip.ipstr) == 0 ) { 
    ip.is_ip=1;
  }
  else {
    ip.is_ip=0;
  }
  return ip;
}


int
check_for_file(char *fname)
{
/* return 0 if not found or cant read or 1 if found */

  FILE *fptemp;
  if ((fptemp = fopen(fname,"r"))) {
    fclose(fptemp);
    dbgtcos("info check_for_file(): File \"%s\" found.\n", fname);
    return 1;
  }

  dbgtcos("info check_for_file(): File \"%s\" not found or cant read.\n", fname);
  return 0;
}




unsigned int getfilesize(char *fname) {
  struct stat infobuf;              /* place to store info */

  if ( stat( fname, &infobuf) == -1 )  /* get info */
    return -1;
  else
    return infobuf.st_size;
}


/*
 * Base64 encoding/decoding (RFC1341)
 * Copyright (c) 2005, Jouni Malinen <j@w1.fi>
 * Adapted to TCOS by Mario Izquierdo (no line breaks output to stdout)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */


unsigned char *readfile(const char *name, size_t *len)
{
	FILE *f;
	unsigned char *buf;
	size_t fret;
        UNUSED(fret);

	f = fopen(name, "rb");
	if (f == NULL)
		return NULL;

	fseek(f, 0, SEEK_END);
	*len = ftell(f);
	fseek(f, 0, SEEK_SET);

	buf = malloc(*len);
	if (buf == NULL) {
		fclose(f);
		return NULL;
	}

	fret=fread(buf, 1, *len, f);
	fclose(f);

	return buf;
}

void *memset(void *s, int c, size_t n)
{
	char *p = s;
	while (n--)
		*p++ = c;
	return s;
}

/*
 * Base64 code taken from wpasupplicant package
 * http://www.koders.com/c/fidC4909D9B4301A6E931115BCFD347E5DB79620A7D.aspx?s=base64_table#L20
 *
*/

static const unsigned char base64_table[65] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/**
 * base64_encode - Base64 encode
 * @src: Data to be encoded
 * @len: Length of the data to be encoded
 * @out_len: Pointer to output length variable, or %NULL if not used
 * Returns: Allocated buffer of out_len bytes of encoded data,
 * or %NULL on failure
 *
 * Caller is responsible for freeing the returned buffer. Returned buffer is
 * nul terminated to make it easier to use as a C string. The nul terminator is
 * not included in out_len.
 */
unsigned char * base64_encode(const unsigned char *src, size_t len,
			      size_t *out_len)
{
	unsigned char *out, *pos;
	const unsigned char *end, *in;
	size_t olen;

	olen = len * 4 / 3 + 4; /* 3-byte blocks to 4-byte */
	olen += olen / 72; /* line feeds */
	olen++; /* nul termination */
	out = malloc(olen);
	if (out == NULL)
		return NULL;

	end = src + len;
	in = src;
	pos = out;
	/*line_len = 0;*/
	while (end - in >= 3) {
		*pos++ = base64_table[in[0] >> 2];
		*pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
		*pos++ = base64_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
		*pos++ = base64_table[in[2] & 0x3f];
		in += 3;
	}

	if (end - in) {
		*pos++ = base64_table[in[0] >> 2];
		if (end - in == 1) {
			*pos++ = base64_table[(in[0] & 0x03) << 4];
			*pos++ = '=';
		} else {
			*pos++ = base64_table[((in[0] & 0x03) << 4) |
					      (in[1] >> 4)];
			*pos++ = base64_table[(in[1] & 0x0f) << 2];
		}
		*pos++ = '=';
	}
    
	*pos = '\0';
	if (out_len)
		*out_len = pos - out;
	return out;
}

/**
 * base64_decode - Base64 decode
 * @src: Data to be decoded
 * @len: Length of the data to be decoded
 * @out_len: Pointer to output length variable
 * Returns: Allocated buffer of out_len bytes of decoded data,
 * or %NULL on failure
 *
 * Caller is responsible for freeing the returned buffer.
 */
unsigned char * base64_decode(const unsigned char *src, size_t len,
			      size_t *out_len)
{
	unsigned char dtable[256], *out, *pos, in[4], block[4], tmp;
	size_t i, count, olen;

	memset(dtable, 0x80, 256);
	for (i = 0; i < sizeof(base64_table) - 1; i++)
		dtable[base64_table[i]] = (unsigned char) i;
	dtable['='] = 0;

	count = 0;
	for (i = 0; i < len; i++) {
		if (dtable[src[i]] != 0x80)
			count++;
	}

	if (count % 4)
		return NULL;

	olen = count / 4 * 3;
	pos = out = malloc(olen);
	if (out == NULL)
		return NULL;

	count = 0;
	for (i = 0; i < len; i++) {
		tmp = dtable[src[i]];
		if (tmp == 0x80)
			continue;

		in[count] = src[i];
		block[count] = tmp;
		count++;
		if (count == 4) {
			*pos++ = (block[0] << 2) | (block[1] >> 4);
			*pos++ = (block[1] << 4) | (block[2] >> 2);
			*pos++ = (block[2] << 6) | block[3];
			count = 0;
		}
	}

	if (pos > out) {
		if (in[2] == '=')
			pos -= 2;
		else if (in[3] == '=')
			pos--;
	}

	*out_len = pos - out;
	return out;
}





/* split string into tokens, return token array */
/* http://www.c.happycodings.com/Miscellaneous/code37.html */
char **split(char *string, char *delim) {
    char **tokens = NULL;
    char *working = NULL;
    char *token = NULL;
    int idx = 0;

    tokens  = malloc(sizeof(char *) * MAXTOKENS);
    if(tokens == NULL)
        return NULL;
    working = malloc(sizeof(char) * strlen(string) + 1);
    if(working == NULL)
        return NULL;

    /* to make sure, copy string to a safe place */
    strcpy(working, string);
    for(idx = 0; idx < MAXTOKENS; idx++)
        tokens[idx] = NULL;

    token = strtok(working, delim);
    idx = 0;

    /* always keep the last entry NULL termindated */
    while((idx < (MAXTOKENS - 1)) && (token != NULL)) {
        tokens[idx] = malloc(sizeof(char) * strlen(token) + 1);
        if(tokens[idx] != NULL) {
            strcpy(tokens[idx], token);
            idx++;
            token = strtok(NULL, delim);
        }
    }

    free(working);
    return tokens;
}


/* replace \n in command output */
void remove_line_break( char *s ) {
    s[strcspn ( s, "\n" )] = '\0';
}


#ifdef TEST_MAIN
int main(int argc, char *argv[])
{
	FILE *f;
	size_t len, elen;
	unsigned char *buf, *e;

	if (argc != 4) {
		printf("Usage: base64 <encode|decode> <in file> <out file>\n");
		return -1;
	}

	buf = readfile(argv[2], &len);
	if (buf == NULL)
		return -1;

	if (strcmp(argv[1], "encode") == 0)
		e = base64_encode(buf, len, &elen);
	else
		e = base64_decode(buf, len, &elen);
	if (e == NULL)
		return -2;
	f = fopen(argv[3], "w");
	if (f == NULL)
		return -3;
	fwrite(e, 1, elen, f);
	fclose(f);
	free(e);

	return 0;
}

#endif
