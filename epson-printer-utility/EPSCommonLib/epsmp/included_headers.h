/*_______________________________  include-headers.h   _________________________________*/

/*       1         2         3         4         5         6         7         8        */
/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*******************************************|********************************************/
/*
 *   Copyright (c) 2007  Seiko Epson Corporation                 All rights reserved.
 *
 *   Copyright protection claimed includes all forms and matters of
 *   copyrightable material and information now allowed by statutory or judicial
 *   law or hereinafter granted, including without limitation, material generated
 *   from the software programs which are displayed on the screen such as icons,
 *   screen display looks, etc.
 *
 */

#ifndef __INCLUDED_HEADERS__
#define __INCLUDED_HEADERS__
    
	/*** C Standard Library Headers                                                     */
    /*** -------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>

    /*** Various Linux and Windows Functions                                            */
    /*** -------------------------------------------------------------------------------*/
#if defined(WIN32)
#include <process.h>
#include <windows.h>
#include <conio.h>
#include "win_cmn.h"
#include "win_net.h"
#include "win_usb.h"
#elif defined(unix) || defined(__unix)
#include "linux/linux_cmn.h"
#include "linux/linux_net.h"
#include "linux/epsmp-linux-portal-io.h"
#include <unistd.h> 
#include <pthread.h>
#include <dirent.h>
#endif

    /*** The header inclusion for the ESC/P-R Library                                   */
    /*** -------------------------------------------------------------------------------*/
#include "epson-escpr-api.h"


    /*** Image Preprocessing and Parsing                                                */
    /*** -------------------------------------------------------------------------------*/
#include "epsmp-image.h"            /* General image functions                          */
#include "epsmp-bmp.h"              /* Bitmap parsing functions                         */
#ifdef GCOMSW_JPEG_SUPPORT
#include "epsmp-jpg.h"				/* Jpeg parsing functions                           */
#endif /* def GCOMSW_JPEG_SUPPORT */
#include "epsmp-scale.h"            /* Function for scaling                             */
#include "epsmp-prn-parse.h"        /* Function for parsing the settings text file      */

    /*** I/O  Functions                                                                 */
    /*** -------------------------------------------------------------------------------*/
#include "epsmp-print-io.h"         /* Console I/O (for error and status messages, etc) */
#include "epsmp-file-io.h"			/* File I/O (for writing to file					*/

#ifdef GCOMSW_ERGB_CONVERT
    /*** The header inclusion for the eRGB converter                                    */
    /*** -------------------------------------------------------------------------------*/
#include "epson-ergb-converter.h"
#pragma comment(lib, "ERGB.lib")
#endif /* GCOMSW_ERGB_CONVERT */

#endif	/* def __INCLUDED_HEADERS__ */

/*_______________________________  include-headers.h   _________________________________*/
  
/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*       1         2         3         4         5         6         7         8        */
/*******************************************|********************************************/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/***** End of File *** End of File *** End of File *** End of File *** End of File ******/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
