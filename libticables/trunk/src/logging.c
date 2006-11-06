/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*  libticables2 - link cable library, a part of the TiLP project
 *  Copyright (C) 1999-2005  Romain Lievin
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
	This unit contains logging functions for internal use.
*/

#include <glib.h>
#include "logging.h"

void ticables_debug (const gchar *format, ...)
{
  va_list args;
  va_start (args, format);
  g_logv (LOG_DOMAIN, G_LOG_LEVEL_DEBUG, format, args);
  va_end (args);
}

void ticables_info (const gchar *format, ...)
{
  va_list args;
  va_start (args, format);
  g_logv (LOG_DOMAIN, G_LOG_LEVEL_INFO, format, args);
  va_end (args);
}

void ticables_message (const gchar *format, ...)
{
  va_list args;
  va_start (args, format);
  g_logv (LOG_DOMAIN, G_LOG_LEVEL_MESSAGE, format, args);
  va_end (args);
}

void ticables_warning (const gchar *format, ...)
{
  va_list args;
  va_start (args, format);
  g_logv (LOG_DOMAIN, G_LOG_LEVEL_WARNING, format, args);
  va_end (args);
}

void ticables_critical (const gchar *format, ...)
{
  va_list args;
  va_start (args, format);
  g_logv (LOG_DOMAIN, G_LOG_LEVEL_CRITICAL, format, args);
  va_end (args);
}

void ticables_error (const gchar *format, ...)
{
  va_list args;
  va_start (args, format);
  g_logv (LOG_DOMAIN, G_LOG_LEVEL_ERROR, format, args);
  va_end (args);
}
