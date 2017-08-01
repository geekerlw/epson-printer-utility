/*________________________________  epsmp-print-io.c    ________________________________*/

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
/*******************************************|********************************************/
/*                                                                                      */
/*                                   Epson Print I/O                                    */
/*                                                                                      */
/*                                    Function Calls                                    */
/*                              --------------------------                              */
/*                          void err_printf(const char* fmt, ...);                      */
/*                          void dbg_printf(const char* fmt, ...);                      */
/*                          void out_printf(const char* fmt, ...);                      */
/*                                                                                      */
/*******************************************|********************************************/

/*------------------------------------  Includes   -------------------------------------*/
/*******************************************|********************************************/
#include <stdio.h>
#include <stdarg.h>

#include "epsmp-print-io.h"

/*----------------------------------  Generic Macros   ---------------------------------*/
/*******************************************|********************************************/

/*-----------------------------------  Definitions  ------------------------------------*/
/*******************************************|********************************************/

/*------------------------------  Local Global Variables  ------------------------------*/
/*******************************************|********************************************/

/*---------------------------  Data Structure Declarations  ----------------------------*/
/*******************************************|********************************************/

/*----------------------------------  Local Functions  ---------------------------------*/
/*******************************************|********************************************/

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   err_printf()                                                        */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* fmt          char*               Format for print function                           */
/* ...          varaible            Varaible list of Arguments                          */
/*                                                                                      */
/* Return value:                                                                        */
/*      None                                                                            */
/*                                                                                      */
/* Description:                                                                         */
/*      Print error to console.                                                         */
/*                                                                                      */
/*******************************************|********************************************/
void    err_printf (

        const char* fmt,
        ...

){
    va_list ap;
    va_start(ap,fmt);

    vfprintf(stderr,fmt,ap);

    va_end(ap);

    fflush(stderr);

}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   dbg_printf()                                                        */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* fmt          char*               Format for print function                           */
/* ...          varaible            Varaible list of Arguments                          */
/*                                                                                      */
/* Return value:                                                                        */
/*      None                                                                            */
/*                                                                                      */
/* Description:                                                                         */
/*      Print debug message to console.                                                 */
/*                                                                                      */
/*******************************************|********************************************/
void    dbg_printf (

        const char* fmt,
        ...

){
    va_list ap;
    va_start(ap,fmt);

    vfprintf(stderr,fmt,ap);

    va_end(ap);

    fflush(stderr);

}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   out_printf()                                                        */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* fmt          char*               Format for print function                           */
/* ...          varaible            Varaible list of Arguments                          */
/*                                                                                      */
/* Return value:                                                                        */
/*      None                                                                            */
/*                                                                                      */
/* Description:                                                                         */
/*      Print message to console.                                                       */
/*                                                                                      */
/*******************************************|********************************************/
void    out_printf (

        const char* fmt,
        ...

){
    va_list ap;
    va_start(ap,fmt);

    vfprintf(stdout,fmt,ap);

    va_end(ap);

    fflush(stdout);

}

/*________________________________  epsmp-print-io.c    ________________________________*/

/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*       1         2         3         4         5         6         7         8        */
/*******************************************|********************************************/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/***** End of File *** End of File *** End of File *** End of File *** End of File ******/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
