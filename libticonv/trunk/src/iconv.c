/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*  libticonv - Ti File Format library, a part of the TiLP project
 *  Copyright (C) 2006-2006 Romain Lievin and Kevin Kofler
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/*
	This unit contains an iconv-compatible wrapper for the TI charset
	conversion routines.
*/

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <string.h>
#include <glib.h>
#include <iconv.h>
#include <errno.h>

#include "ticonv.h"
#include "charset.h"

#define TIE		TIEXPORT
#define TIC		TICALL


/* This is a version of tifiles_string_to_model without the "USB" variants
   which don't make much sense for charset use. And we can't use tifiles
   functions here anyway (otherwise we'd have a circular dependency). */
static CalcModel ticonv_string_to_model(const char *str)
{
	if(!g_ascii_strcasecmp(str, "TI73"))
		return CALC_TI73;
	else if(!g_ascii_strcasecmp(str, "TI82"))
		return CALC_TI82;
	else if(!g_ascii_strcasecmp(str, "TI83"))
		return CALC_TI83;
	else if(!g_ascii_strcasecmp(str, "TI83+"))
		return CALC_TI83P;
	else if(!g_ascii_strcasecmp(str, "TI84+"))
		return CALC_TI84P;
	else if(!g_ascii_strcasecmp(str, "TI85"))
		return CALC_TI85;
	else if(!g_ascii_strcasecmp(str, "TI86"))
		return CALC_TI86;
	else if(!g_ascii_strcasecmp(str, "TI89"))
		return CALC_TI89;
	else if(!g_ascii_strcasecmp(str, "TI89t"))
		return CALC_TI89T;
	else if(!g_ascii_strcasecmp(str, "TI92"))
		return CALC_TI92;
	else if(!g_ascii_strcasecmp(str, "TI92+"))
		return CALC_TI92P;
	else if(!g_ascii_strcasecmp(str, "V200"))
		return CALC_V200;
		
	return CALC_NONE;
}

/* Allocate descriptor for code conversion from codeset FROMCODE to
   codeset TOCODE.  */
TIE ticonv_iconv_t TIC ticonv_iconv_open (const char *tocode, const char *fromcode)
{
  ticonv_iconv_t cd;
  cd.src_calc=ticonv_string_to_model(fromcode);
  cd.dest_calc=ticonv_string_to_model(tocode);
  cd.iconv_desc=iconv_open(cd.src_calc?"UTF-16":fromcode,cd.dest_calc?"UTF-16":tocode);
  return cd;
}

/* Convert at most *INBYTESLEFT bytes from *INBUF according to the
   code conversion algorithm specified by CD and place up to
   *OUTBYTESLEFT bytes in buffer at *OUTBUF.  */
TIE size_t TIC ticonv_iconv (ticonv_iconv_t cd, char **__restrict inbuf,
                             size_t *__restrict inbytesleft,
                             char **__restrict outbuf,
                             size_t *__restrict outbytesleft)
{
  if (!inbuf || !inbuf) {
    if (!outbuf || !*outbuf || cd.dest_calc) {
      return iconv(cd.iconv_desc,NULL,NULL,NULL,NULL);
    } else {
      return iconv(cd.iconv_desc,NULL,NULL,outbuf,outbytesleft);
    }
  } else {
    /* FIXME: This function isn't quite compliant to the iconv spec... */
    unsigned short *temp=NULL;
    void *iconv_src;
    size_t inbytes;
    size_t iconv_inbytes;
    size_t bufsize=1024;
    char *buf=NULL;
    void *out;
    size_t result;

    if (cd.src_calc) {
      /* FIXME: This doesn't honor inbytesleft. */
      temp=ticonv_charset_ti_to_utf16(cd.src_calc,*inbuf);
      iconv_src=&temp;
      inbytes=(ticonv_utf16_strlen(temp)+1)<<1;
      iconv_inbytes=inbytes;
    } else {
      iconv_src=inbuf;
      iconv_inbytes=*inbytesleft;
    }

    while(1) {
      void *iconv_temp_src=*inbuf;
      void *iconv_temp_dest;
      size_t iconv_size=bufsize;
      size_t iconv_temp_inbytes=iconv_inbytes;
      iconv(cd.iconv_desc,NULL,NULL,NULL,NULL);
      buf=g_realloc(buf,bufsize);
      iconv_temp_dest=buf;
      result=iconv(cd.iconv_desc,(void*)&iconv_temp_src,&iconv_temp_inbytes,(void*)&iconv_temp_dest,&iconv_size);
      if (result!=(size_t)-1 || errno!=E2BIG) {
        bufsize-=iconv_size;
        break;
      }
      bufsize<<=1;
    }

    if (result!=(size_t)-1) {
      if (cd.dest_calc) {
        out=ticonv_charset_utf16_to_ti(cd.dest_calc,(unsigned short *)buf);
        bufsize=strlen(out)+1;
      } else {
        out=buf;
      }

      if (bufsize>*outbytesleft) {
        memcpy(*outbuf,out,*outbytesleft);
        *outbuf+=*outbytesleft;
        *outbytesleft=0;
        result=-1;
        errno=E2BIG;
        /* FIXME: *inbuf, *inbytesleft not set properly. */
      } else {
        memcpy(*outbuf,out,bufsize);
        *outbuf+=bufsize;
        *outbytesleft-=bufsize;
      }

      if (out!=buf) g_free(out);
    }
    /* FIXME: *outbuf, *outbytesleft not set properly if result==(size_t)-1. */

    g_free(buf);
    g_free(temp);
    /* FIXME: This isn't quite the correct return value. */
    return result;
  }
}

/* Free resources allocated for descriptor CD for code conversion.  */
TIE int TIC ticonv_iconv_close (ticonv_iconv_t cd)
{
  return iconv_close(cd.iconv_desc);
}
