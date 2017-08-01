/*_______________________________    epsmp-file-io.c   _________________________________*/

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
/*                                       File I/O                                       */
/*                                                                                      */
/*                                    Function Calls                                    */
/*                              --------------------------                              */
/*                  EPS_FILEDSC epsmpOpenFile  (dev                            );       */
/*                  EPS_INT32   dummyReadFile  (fd, Data, Length, ReadLength   );       */
/*                  EPS_INT32   epsmpWriteFile (fd, Data, Length, LengthWritten);       */
/*                  EPS_INT32   epsmpCloseFile (fd                             );       */
/*                                                                                      */
/*******************************************|********************************************/

/*------------------------------------  Includes   -------------------------------------*/
/*******************************************|********************************************/
#include <stdio.h>
#include <fcntl.h>
#if defined(unix) || defined(__unix)
#include <unistd.h> 
#endif
#if defined(_WIN32)
#include <windows.h>
#endif

#include "included_headers.h"
#include "epsmp-file-io.h"

/*------------------------------------- Data Types -------------------------------------*/
/*******************************************|********************************************/

/*----------------------------------  Generic Macros   ---------------------------------*/
/*******************************************|********************************************/

/*---------------------------  Data Structure Declarations   ---------------------------*/
/*******************************************|********************************************/

/*------------------------------  Local Compiler Switch  -------------------------------*/
/*******************************************|********************************************/
    /*** Output Target                                                                  */
    /***--------------------------------------------------------------------------------*/
#define LCOMSW_USB_PORT_OUTPUT          1   /* 0: Output to File only                                           */
                                            /* 1: Output to File and Linux USB Port                             */
                                            /*    (There is a limitation for using LCOMSW_USB_PORT_OUTPUT!!!)   */


    /*==================================================================================*/
    /*                Restrictions when using LCOMSW_USB_PORT_OUTPUT                    */
    /*==================================================================================*/
    /* If you print in the following steps, a printer driver will freeze before a       */
    /* printer starts printing operation.                                               */
    /* 1.Set LCOMSW_USB_PORT_OUTPUT = 1, and execute printing in Uni-Directional        */
    /*   Print Demo. (startup parameter -d=u)                                           */
    /* 2.Intentionally force an error with the printer after printing. (Any error is    */
    /*   acceptable)                                                                    */
    /* 3.If you print in Print Demo (Bi-Directional), the printer driver will freeze    */
    /*   before the printer starts printing operation.                                  */
    /*   <For some reason, the printer will refuse to receive data, and even if the     */
    /*   driver sends the initialization command (Epson Packing Command), it will lock  */
    /*   up(waiting for data transmission?) within a data transmission function. >      */
    /*==================================================================================*/

/*-----------------------------------  Definitions  ------------------------------------*/
/*******************************************|********************************************/

/*------------------------------  Local Grobal Variables   -----------------------------*/
/*******************************************|********************************************/
static FILE* outFile;                                   /* Output Data File             */

                                            
/*--------------------------------  Function Declarations   ----------------------------*/
/*******************************************|********************************************/


/*******************************************|********************************************/
/* Function name:   epsmpOpenFile()                                                     */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* dev          EPS_USB_DEVICE      I: pointer to a device info.                        */
/*                                                                                      */
/* Return value:                                                                        */
/*      (valid file discripter)     - Success                                           */
/*      EPS_INVALID_FILEDSC         - Failure                                           */
/*                                                                                      */
/* Description:                                                                         */
/*      Opens File.                                                                     */
/*                                                                                      */
/*******************************************|********************************************/
EPS_FILEDSC   epsmpOpenFile (

        const EPS_USB_DEVICE* dev

){
#if LCOMSW_USB_PORT_OUTPUT
	EPS_FILEDSC fd;
#endif

	(void)dev;

/*** Open File                                                                          */
    outFile = fopen("out.prn", "wb");

    if (outFile == NULL)
        return EPS_INVALID_FILEDSC;

#if LCOMSW_USB_PORT_OUTPUT
/*** Open USB port                                                                      */
	fd = epsmpOpenPortal(NULL);
	if(EPS_INVALID_FILEDSC == fd){
		fclose(outFile);
	}

/*** Return to Caller                                                                   */
	return fd;
#else

	return 0;
#endif
}


/*******************************************|********************************************/
/* Function name:   dummyReadFile()                                                     */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* -            void                -                                                   */
/*                                                                                      */
/* Return value:                                                                        */
/*      -1              - Dummy Failure                                                 */
/*                                                                                      */
/* Description:                                                                         */
/*      Dummy function                                                                  */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   dummyReadFile (

        EPS_FILEDSC fd,
		EPS_UINT8*  Data,
        EPS_INT32   Length,
        EPS_INT32*  lengthRead

){
    (void)fd;
	(void)Data;
    (void)Length;
    (void)lengthRead;

	printf("####################################################\n");
    printf("!!!!!!!   <ERROR> Called dummyReadPortal()   !!!!!!!\n");
    printf("####################################################\n");
    
/*** Return to caller (Dummy Failure)                                                   */
    return -1;

}
/*******************************************|********************************************/
/* Function name:   epsmpWriteFile()                                                    */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:            Type:               Description:                                    */
/* Data             EPS_UINT8*          Pointer to a location to read the data to.      */
/* Length           EPS_INT32           Length of the data (in bytes).                  */
/* lengthWritten    EPS_INT32*          The number of bytes actually read (the return   */
/*                                      value of the read() function).                  */
/*                                                                                      */
/* Return value:                                                                        */
/*      0               - Success                                                       */
/*      -1              - Failure                                                       */
/*                                                                                      */
/* Description:                                                                         */
/*      Writes data.                                                                    */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   epsmpWriteFile (

		EPS_FILEDSC fd,
        const EPS_UINT8*  Data,
        EPS_INT32   Length,
        EPS_INT32*  lengthWritten

){

/*** Output to file                                                                     */
    *lengthWritten = (EPS_INT32)fwrite(Data, sizeof(EPS_UINT8), Length, outFile);
    
    if (*lengthWritten != Length)
        return -1;
    
#if LCOMSW_USB_PORT_OUTPUT
/*** Output to USB port                                                                 */
	return epsmpWritePortal(fd, Data, Length, lengthWritten);

#else

    return 0;
#endif
}

/*******************************************|********************************************/
/* Function name:   epsmpCloseFile()                                                    */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:            Type:               Description:                                    */
/* N/A              void                N/A                                             */
/*                                                                                      */
/* Return value:                                                                        */
/*      0               - Success                                                       */
/*      -1              - Failure                                                       */
/*                                                                                      */
/* Description:                                                                         */
/*      Closes file.                                                                    */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   epsmpCloseFile (

        EPS_FILEDSC fd 

){

/*** Close file                                                                         */
    if (outFile != NULL) { 
        fflush(outFile);
        fclose(outFile);
    }

#if LCOMSW_USB_PORT_OUTPUT
/*** Close USB port                                                                     */
	return epsmpClosePortal(fd);
#else

    return 0;
#endif
}

/*_______________________________    epsmp-file-io.c   _________________________________*/
  
/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*       1         2         3         4         5         6         7         8        */
/*******************************************|********************************************/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/***** End of File *** End of File *** End of File *** End of File *** End of File ******/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
