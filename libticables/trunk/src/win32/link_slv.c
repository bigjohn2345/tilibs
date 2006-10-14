/* Hey EMACS -*- win32-c -*- */
/* $Id$ */

/*  libCables - Ti Link Cable library, a part of the TiLP project
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

/* TI-GRAPH LINK USB support */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../gettext.h"
#include "../logging.h"
#include "../ticables.h"
#include "detect.h"
#include "../error.h"

/* 
   This part talk with the USB device driver through the TiglUsb library.
   This module need TiglUsb.h (interface) but does not need TiglUsb.lib (linkage).
*/

#include <stdio.h>
#include <windows.h>

#include "tiglusb.h"

#define MIN_VERSION "3.6"

// Functions pointers for dynamic loading
typedef struct
{
	TIGLUSB_VERSION		TiglUsbVersion;	
	TIGLUSB_PROBE2		TiglUsbProbe;
	TIGLUSB_OPEN2		TiglUsbOpen;
	TIGLUSB_CLOSE2		TiglUsbClose;
	TIGLUSB_CHECK2		TiglUsbCheck;
	TIGLUSB_READS2		TiglUsbReads;
	TIGLUSB_WRITES2		TiglUsbWrites;
	TIGLUSB_RESET2		TiglUsbReset;
	TIGLUSB_SETTIMEOUT2	TiglUsbSetTimeout;
	TIGLUSB_GETTIMEOUT2	TiglUsbGetTimeout;
} TIGLFNCTS;

// Convenient wrappers
#define hDLL	(HANDLE)(h->priv)		// DLL handle on TiglUsb.dll
#define dTGL	((TIGLFNCTS*)(h->priv2))// Function pointers
#define hLNK	(int)(h->priv3)			// Link handle as return by dynTiglUsbOpen

#define dynTiglUsbVersion	(dTGL->TiglUsbVersion)
#define dynTiglUsbProbe		(dTGL->TiglUsbProbe)
#define dynTiglUsbOpen		(dTGL->TiglUsbOpen)
#define dynTiglUsbClose		(dTGL->TiglUsbClose)
#define dynTiglUsbCheck		(dTGL->TiglUsbCheck)
#define dynTiglUsbReads		(dTGL->TiglUsbReads)
#define dynTiglUsbWrites	(dTGL->TiglUsbWrites)
#define dynTiglUsbReset		(dTGL->TiglUsbReset)
#define dynTiglUsbSetTimeout (dTGL->TiglUsbSetTimeout)
#define dynTiglUsbGetTimeout (dTGL->TiglUsbGetTimeout)

static int slv_prepare(CableHandle *h)
{
	char str[64];

	h->address = h->port;
	sprintf(str, "TiglUsb #%i", h->port);
	h->device = strdup(str);

	// detect driver
	if(!win32_detect_tiglusb())
	{
		free(h->device); h->device = NULL;
		return ERR_SLV_LOADLIBRARY;		
	}

	h->priv2 = calloc(1, sizeof(TIGLFNCTS));

	return 0;
}

static int slv_open(CableHandle *h)
{
	int ret;

	// Create an handle on library and retrieve symbols
	h->priv = (void *)LoadLibrary("TIGLUSB.DLL");
	if (hDLL == NULL) 
	{
		ticables_warning(_("TiglUsb library not found. Have you installed the TiglUsb driver ?"));
		return ERR_SLV_LOADLIBRARY;
	}

	dynTiglUsbVersion = (TIGLUSB_VERSION) GetProcAddress(hDLL, "TiglUsbVersion");
	if (!dynTiglUsbVersion || (strcmp(dynTiglUsbVersion(), MIN_VERSION) < 0)) 
	{
	    char buffer[256];
		sprintf(buffer, _("TiglUsb.dll: version %s mini needed, got version %s.\nPlease download the latest release on <http://ti-lpg.org/prj_usb>."),
			MIN_VERSION, dynTiglUsbVersion());
		ticables_warning(buffer);
		MessageBox(NULL, buffer, "Error in SilverLink support", MB_OK);
		FreeLibrary(hDLL);
		return ERR_SLV_VERSION;
	}
	ticables_info(_("using TiglUsb.dll version %s"), dynTiglUsbVersion());

	dynTiglUsbProbe = (TIGLUSB_PROBE2) GetProcAddress(hDLL, "TiglUsbProbe2");
	if (!dynTiglUsbProbe) 
	{
		ticables_warning(_("Unable to load TiglUsbOpen2 symbol."));
		FreeLibrary(hDLL);
		return ERR_SLV_FREELIBRARY;
	}

	dynTiglUsbOpen = (TIGLUSB_OPEN2) GetProcAddress(hDLL, "TiglUsbOpen2");
	if (!dynTiglUsbOpen) 
	{
		ticables_warning(_("Unable to load TiglUsbOpen2 symbol."));
		FreeLibrary(hDLL);
		return ERR_SLV_FREELIBRARY;
	}

    dynTiglUsbClose = (TIGLUSB_CLOSE2) GetProcAddress(hDLL, "TiglUsbClose2");
	if (!dynTiglUsbClose) 
	{
		ticables_warning(_("Unable to load TiglUsbClose symbol."));
		FreeLibrary(hDLL);
		return ERR_SLV_FREELIBRARY;
	}

	dynTiglUsbCheck = (TIGLUSB_CHECK2) GetProcAddress(hDLL, "TiglUsbCheck2");
	if (!dynTiglUsbCheck) 
	{
		ticables_warning(_("Unable to load TiglUsbCheck symbol."));
		FreeLibrary(hDLL);
		return ERR_SLV_FREELIBRARY;
	}

	dynTiglUsbReads = (TIGLUSB_READS2) GetProcAddress(hDLL, "TiglUsbReads2");
	if (!dynTiglUsbReads) 
	{
		ticables_warning(_("Unable to load TiglUsbRead2 symbol."));
		FreeLibrary(hDLL);
		return ERR_SLV_FREELIBRARY;
	}

	dynTiglUsbWrites = (TIGLUSB_WRITES2) GetProcAddress(hDLL, "TiglUsbWrites2");
	if (!dynTiglUsbWrites) 
	{
	    ticables_warning(_("Unable to load TiglUsbWrite2 symbol."));
		FreeLibrary(hDLL);
		return ERR_SLV_FREELIBRARY;
	}

    dynTiglUsbReset = (TIGLUSB_RESET2) GetProcAddress(hDLL, "TiglUsbReset2");
	if (!dynTiglUsbReset) 
	{
	    ticables_warning(_("Unable to load dynTiglUsbReset symbol."));
		FreeLibrary(hDLL);
		return ERR_SLV_FREELIBRARY;
	}

	dynTiglUsbSetTimeout = (TIGLUSB_SETTIMEOUT2) GetProcAddress(hDLL, "TiglUsbSetTimeout2");
	if (!dynTiglUsbSetTimeout) 
	{
		ticables_warning(_("Unable to load TiglUsbSetTimeout2 symbol."));
		FreeLibrary(hDLL);
		return ERR_SLV_FREELIBRARY;
	}

    dynTiglUsbGetTimeout = (TIGLUSB_GETTIMEOUT2) GetProcAddress(hDLL, "TiglUsbGetTimeout2");
	if (!dynTiglUsbGetTimeout) 
	{
		ticables_warning(_("Unable to load TiglUsbGetTimeout2 symbol."));
		FreeLibrary(hDLL);
		return ERR_SLV_FREELIBRARY;
	}
  
	h->priv3 = (void *)(ret = dynTiglUsbOpen(h->port));
	switch (ret) 
	{
		case TIGLERR2_DEV_OPEN_FAILED: return ERR_SLV_OPEN;
		case TIGLERR2_DEV_ALREADY_OPEN: return ERR_SLV_OPEN;
		default: break;
	}

	dynTiglUsbSetTimeout(hLNK, h->timeout);

	// Reset cable (=slv_reset)
	ret = dynTiglUsbReset(hLNK);
    if(ret == TIGLERR2_RESET_FAILED)
        return ERR_SLV_RESET;

	return 0;
}

static int slv_close(CableHandle *h)
{
	/*int ret =*/ dynTiglUsbClose(hLNK);
	
    if (hDLL != NULL)
	{
        FreeLibrary(hDLL);
		h->priv = NULL;

		free(h->priv2);
		h->priv2 = NULL;
	}

	return 0;
}

static int slv_reset(CableHandle *h)
{
	int ret;
#ifdef SLV_RESET
    ret = dynTiglUsbReset(hLNK);
    if(ret == TIGLERR2_RESET_FAILED)
        return ERR_SLV_RESET;
#else
	ret = slv_close(h);
	if(ret) return ret;

	ret = slv_open(h);
	if(ret) return ret;
#endif

	return 0;
}

static int slv_probe(CableHandle *h)
{
	int ret;
	unsigned int *list;
	int open = 0;

	if(dynTiglUsbProbe == NULL)
	{
		open = !0;
		h->priv = (void *)LoadLibrary("TIGLUSB.DLL");
		if (hDLL == NULL) 
		{
			ticables_warning(_("TiglUsb library not found. Have you installed the TiglUsb driver ?"));
			return ERR_SLV_LOADLIBRARY;
		}

		dynTiglUsbProbe = (TIGLUSB_PROBE2) GetProcAddress(hDLL, "TiglUsbProbe2");
		if (!dynTiglUsbProbe) 
		{
			ticables_warning(_("Unable to load TiglUsbOpen2 symbol."));
			FreeLibrary(hDLL);
			return ERR_SLV_FREELIBRARY;
		}
	}

	ret = dynTiglUsbProbe(&list);
    
	if(ret > 0)
	{
		if(list[h->address-1] == PID_TIGLUSB)
		{
			if(open)
			{
				FreeLibrary(hDLL);
				dynTiglUsbProbe = NULL;
			}
			return 0;
		}
	}

	if(open)
	{
		FreeLibrary(hDLL);
		dynTiglUsbProbe = NULL;
	}

	return ERR_PROBE_FAILED;
}

static int raw_probe(CableHandle *h)
{
	int ret;
	unsigned int *list;
	int open = 0;

	if(dynTiglUsbProbe == NULL)
	{
		open = !0;
		h->priv = (void *)LoadLibrary("TIGLUSB.DLL");
		if (hDLL == NULL) 
		{
			ticables_warning(_("TiglUsb library not found. Have you installed the TiglUsb driver ?"));
			return ERR_SLV_LOADLIBRARY;
		}

		dynTiglUsbProbe = (TIGLUSB_PROBE2) GetProcAddress(hDLL, "TiglUsbProbe2");
		if (!dynTiglUsbProbe) 
		{
			ticables_warning(_("Unable to load TiglUsbOpen2 symbol."));
			FreeLibrary(hDLL);
			return ERR_SLV_FREELIBRARY;
		}
	}

	ret = dynTiglUsbProbe(&list);
    
	if(ret > 0)
	{
		if(list[h->address-1] == PID_TI89TM || 
			list[h->address-1] == PID_TI84P ||
			list[h->address-1] == PID_TI84P_SE)
		{
			if(open)
			{
				FreeLibrary(hDLL);
				dynTiglUsbProbe = NULL;
			}
			return 0;
		}
	}

	if(open)
	{
		FreeLibrary(hDLL);
		dynTiglUsbProbe = NULL;
	}

	return ERR_PROBE_FAILED;
}

static int slv_timeout(CableHandle *h)
{
	dynTiglUsbSetTimeout(hLNK, h->timeout);

	return 0;
}

static int slv_put(CableHandle *h, uint8_t *data, uint32_t len)
{
	int ret;

	ret = dynTiglUsbWrites(hLNK, data, len);

	switch (ret) 
	{
	case TIGLERR2_WRITE_TIMEOUT:
		return ERR_WRITE_TIMEOUT;
	case TIGLERR2_WRITE_ERROR:
		return ERR_WRITE_ERROR;
	default:
		break;
	}

	return 0;
}

static int slv_get(CableHandle *h, uint8_t *data, uint32_t len)
{
	int ret;

	ret = dynTiglUsbReads(hLNK, data, len);

	switch (ret) 
	{
	case TIGLERR2_READ_TIMEOUT:
		return ERR_READ_TIMEOUT;
	case TIGLERR2_READ_ERROR:
		return ERR_READ_ERROR;
	default:
		break;
	}

	return 0;
}

static int slv_check(CableHandle *h, int *status)
{
	int ret = dynTiglUsbCheck(hLNK, status);

    switch (ret) 
	{
    case TIGLERR2_READ_TIMEOUT:
        return ERR_READ_TIMEOUT;
    case TIGLERR2_READ_ERROR:
        return ERR_READ_ERROR;
    default:
        break;
    }

	return 0;
}

static int slv_set_red_wire(CableHandle *h, int b)
{
	return 0;
}

static int slv_set_white_wire(CableHandle *h, int b)
{
	return 0;
}

static int slv_get_red_wire(CableHandle *h)
{
	return 1;
}

static int slv_get_white_wire(CableHandle *h)
{
	return 1;
}

const CableFncts cable_slv = 
{
	CABLE_SLV,
	"SLV",
	N_("SilverLink"),
	N_("SilverLink (TI-GRAPH LINK USB) cable"),
	0,
	&slv_prepare,
	&slv_open, &slv_close, &slv_reset, &slv_probe, &slv_timeout,
	&slv_put, &slv_get, &slv_check,
	&slv_set_red_wire, &slv_set_white_wire,
	&slv_get_red_wire, &slv_get_white_wire,
};

const CableFncts cable_raw = 
{
	CABLE_USB,
	"USB",
	N_("DirectLink"),
	N_("DirectLink (direct USB) cable"),
	0,
	&slv_prepare,
	&slv_open, &slv_close, &slv_reset, &raw_probe, &slv_timeout,
	&slv_put, &slv_get, &slv_check,
	&slv_set_red_wire, &slv_set_white_wire,
	&slv_get_red_wire, &slv_get_white_wire,
};

//=======================

// returns number of devices and list of PIDs (dynamically allocated)
TIEXPORT1 int TICALL usb_probe_devices(int **list)
{
	HANDLE	hDll;
	TIGLUSB_PROBE2	dynProbe = NULL;
	int ret;
	PUINT tmp;

	hDll = LoadLibrary("TIGLUSB.DLL");
	if (hDll == NULL) 
	{
		ticables_warning(_("TiglUsb library not found. Have you installed the TiglUsb driver ?"));
		return ERR_SLV_LOADLIBRARY;
	}

	dynProbe = (TIGLUSB_PROBE2) GetProcAddress(hDll, "TiglUsbProbe2");
	if (!dynProbe) 
	{
		ticables_warning(_("Unable to load TiglUsbOpen2 symbol."));
		FreeLibrary(hDll);
		return ERR_SLV_FREELIBRARY;
	}

	ret = dynProbe(&tmp);
	// we need to copy the result; tmp points on DLL space 
	// which is not valid after call to FreeLibrary
	*list = (int *)calloc(USB_MAX+1, sizeof(int));
	if(ret > 0)	memcpy(*list, tmp, (USB_MAX+1) * sizeof(UINT));

	FreeLibrary(hDll);
	dynProbe = NULL;

	return 0;
}
