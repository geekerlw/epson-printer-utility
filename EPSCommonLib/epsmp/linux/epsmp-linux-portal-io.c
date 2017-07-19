/*____________________________   epsmp-linux-portal-io.c   _____________________________*/

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
/*                               Linux USB Portal I/O                                   */
/*                                                                                      */
/*                                    Function Calls                                    */
/*                              --------------------------                              */
/*                  EPS_INT32   epsmpOpenPortal  (void                       		);  */
/*                  EPS_INT16   epsmpReadPortal  (Data, Length, LengthRead   		);  */
/*                  EPS_INT16   epsmpWritePortal (Data, Length, LengthWritten		);  */
/*                  EPS_INT32   epsmpClosePortal (void								);  */
/*                  EPS_FILEDSC epsmpFindFirst   (dev								);  */
/*                  EPS_BOOL	epsmpFindNext    (fd, dev							);  */
/*                  EPS_BOOL	epsmpFindClose   (fd								);  */
/*                                                                                      */
/*******************************************|********************************************/

#if defined(unix) || defined(__unix)
/*------------------------------------  Includes   -------------------------------------*/
/*******************************************|********************************************/
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <memory.h>
#include <malloc.h>
#include <errno.h>
#include "err.h"

#include "epsmp-linux-portal-io.h"
#include "included_headers.h"
#include "XAsyncSocket.h"

/*------------------------------------- Data Types -------------------------------------*/
/*******************************************|********************************************/
#define MAX_DEV_ID_LENGTH   512
#define MAX_PATH_LENGTH     20
#define MAX_DEVICES         20
#define PATH_TO_USB_BUS     "/dev/usb/lp"       /* If your Linux system uses another    */
                                                /* path to the printer, change this     */
                                                /* value accordingly.                   */

/*----------------------------------  Generic Macros   ---------------------------------*/
/*******************************************|********************************************/

/*-----------------------------------  Definitions  ------------------------------------*/
/*******************************************|********************************************/

/*---------------------------  Data Structure Declarations   ---------------------------*/
/*******************************************|********************************************/

/*------------------------------  Local Grobal Variables   -----------------------------*/
/*******************************************|********************************************/
static EPS_INT32	g_FindCounter;						/* find port counter            */ 

/*----------------------------------  Local Functions  ---------------------------------*/
/*******************************************|********************************************/
static EPS_BOOL FindDevice          (EPS_INT32*, EPS_USB_DEVICE*                        );
static EPS_BOOL GetDeviceNumbers	(EPS_FILEDSC, EPS_USB_DEVICE*                       );


/*--------------------------------  Function Declarations   ----------------------------*/
/*******************************************|********************************************/

/*******************************************|********************************************/
/* Function name:   epsmpOpenPortal()                                                   */
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
/*      Opens an I/O portal.                                                            */
/*                                                                                      */
/*******************************************|********************************************/
EPS_FILEDSC   epsmpOpenPortal (

        const EPS_USB_DEVICE* dev

){
    debug_msg("start call fucntion epsmp-linux-portal-io.c/epsmpOpenPortal\n");
    EPS_INT32		nCnt;
	EPS_INT8		usb_path[MAX_PATH_LENGTH];
	EPS_USB_DEVICE  devTmp;
	EPS_FILEDSC     fd = EPS_INVALID_FILEDSC;

	nCnt = 0;
	while( FindDevice(&nCnt, &devTmp) ){
		if(NULL != dev){
			if(devTmp.vid == dev->vid 
			&& devTmp.pid == dev->pid 
			&& devTmp.port == dev->port ){
		
				snprintf(usb_path, MAX_PATH_LENGTH, "%s%d", PATH_TO_USB_BUS, nCnt-1);
				fd = open(usb_path, O_RDWR);
				break;
			}
		} else{
			snprintf(usb_path, MAX_PATH_LENGTH, "%s%d", PATH_TO_USB_BUS, nCnt-1);
			fd = open(usb_path, O_RDWR);
			break;
		}
	}

    debug_msg("Exit call fucntion epsmp-linux-portal-io.c/epsmpOpenPortal\n");
	return fd;
}


/*******************************************|********************************************/
/* Function name:   epsmpReadPortal                                                     */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* fd           EPS_UINT8*          File discripter.                                    */
/* Data         EPS_UINT8*          Pointer to a location to read the data to.          */
/* Length       EPS_INT32           Length of the data (in bytes).                      */
/* lengthRead   EPS_INT32*          The number of bytes actually read (the return value */
/*                                  of the read() function).                            */
/*                                                                                      */
/* Return value:                                                                        */
/*      0               - Success                                                       */
/*      -1              - Failure                                                       */
/*                                                                                      */
/* Description:                                                                         */
/*      Reads data from the portal.                                                     */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   epsmpReadPortal (

        EPS_FILEDSC fd,
		EPS_UINT8*  Data,
        EPS_INT32   Length,
        EPS_INT32*  lengthRead

){

	int ret;
	XAsyncSocket async;

	async.Open();

	ret=async.Select(fd, kXAsyncSocketSelect_Read, 100);
	
    debug_msg("start call function epsmp-linux-portal-io.c/epsmpReadPortal\n");

	if (ret == noErr){
    	*lengthRead = read(fd, Data, Length);
	}
   	debug_msg("Exit call function epsmp-linux-portal-io.c/epsmpReadPortal\n");
 
	async.Close();	

   if (*lengthRead >= 0) {
        return 0;
    } 
    else {
        *lengthRead = 0;
        return -1;
    }
}

/*******************************************|********************************************/
/* Function name:   epsmpWritePortal()                                                  */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:            Type:               Description:                                    */
/* fd               EPS_UINT8*          File discripter.                                */
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
/*      Writes data to the portal.                                                      */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   epsmpWritePortal (

        EPS_FILEDSC fd,
		const EPS_UINT8*  Data,
        EPS_INT32   Length,
        EPS_INT32*  lengthWritten

){
    debug_msg("start call function epsmp-linux-portal-io.c/epsmpWritePortal\n");
    EPS_INT32 ret;
    
    ret = write(fd, Data, Length);
    debug_msg("Exit call function epsmp-linux-portal-io.c/epsmpWritePortal\n");
    
    if (ret < 0) {
        lengthWritten = 0;
        return -1;
    } 
    else { 
        *lengthWritten = ret;
        return  0; 
    }
}

/*******************************************|********************************************/
/* Function name:   epsmpClosePortal()                                                  */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:            Type:               Description:                                    */
/* fd               EPS_UINT8*          File discripter.                                */
/*                                                                                      */
/* Return value:                                                                        */
/*      0               - Success                                                       */
/*      -1              - Failure                                                       */
/*                                                                                      */
/* Description:                                                                         */
/*      Closes the portal to the printer.                                               */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   epsmpClosePortal (

        EPS_FILEDSC fd

){
    return close(fd);

}


/*******************************************|********************************************/
/* Function name:   epsmpFindFirst()                                                    */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* dev          EPS_USB_DEVICE      O: pointer to a found device info.                  */
/*                                                                                      */
/* Return value:                                                                        */
/*      (valid file discripter)     - Success (Found)                                   */
/*      EPS_INVALID_FILEDSC         - Failure (Not found)                               */
/*                                                                                      */
/* Description:                                                                         */
/*      Find first usb device.                                                          */
/*                                                                                      */
/*******************************************|********************************************/
EPS_FILEDSC epsmpFindFirst	(
							 
		EPS_USB_DEVICE* dev
		
){
    debug_msg("Start call function epsmp-linux-portal-io.c/epsmpFindFirst\n");
	g_FindCounter = 0;

	if( FindDevice(&g_FindCounter, dev) ){
        debug_msg("Exit call function epsmp-linux-portal-io.c/epsmpFindFirst\n");
		return 1;	/* dummy. sign of success */
	} else{
        debug_msg("Exit call function epsmp-linux-portal-io.c/epsmpFindFirst\n");
		return EPS_INVALID_FILEDSC;
	}
	
}


/*******************************************|********************************************/
/* Function name:   epsmpFindNext()                                                     */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* fd           EPS_FILEDSC         I: Find File discripter.                            */
/* dev          EPS_USB_DEVICE      O: pointer to a found device info.                  */
/*                                                                                      */
/* Return value:                                                                        */
/*      TRUE                        - Success (Found)                                   */
/*      FALSE                       - Failure (It doesn't exist any further.)           */
/*                                                                                      */
/* Description:                                                                         */
/*      Find next usb device.                                                           */
/*                                                                                      */
/*******************************************|********************************************/
EPS_BOOL    epsmpFindNext	(
							 
		EPS_FILEDSC     fdFind, 
		EPS_USB_DEVICE*	dev

){
    debug_msg("Start call function epsmp-linux-portal-io.c/epsmpFindNext \n");
	if( g_FindCounter < 0 ){
		return EPS_INVALID_FILEDSC;
	}

	if( FindDevice(&g_FindCounter, dev) ){
        debug_msg("Exit call function epsmp-linux-portal-io.c/epsmpFindNext \n");
		return TRUE;
	} else{
        debug_msg("Exit call function epsmp-linux-portal-io.c/epsmpFindNext \n");
		return FALSE;
	}
}


/*******************************************|********************************************/
/* Function name:   epsmpFindClose()                                                    */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* fd           EPS_FILEDSC         I: Find File discripter.                            */
/*                                                                                      */
/* Return value:                                                                        */
/*      TRUE                        - Success                                           */
/*      FALSE                       - Failure                                           */
/*                                                                                      */
/* Description:                                                                         */
/*      Close find proccess.                                                            */
/*                                                                                      */
/*******************************************|********************************************/
EPS_BOOL    epsmpFindClose  (
							 
		EPS_FILEDSC fdFind

){
	g_FindCounter = -1;
	return TRUE;
}


/*******************************************|********************************************/
/* Function name:   epsmpFindNext()                                                     */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* pCounter     EPS_INT32*          I: Find loop counter.                               */
/* dev          EPS_USB_DEVICE      O: pointer to a found device info.                  */
/*                                                                                      */
/* Return value:                                                                        */
/*      TRUE                        - Success (Found)                                   */
/*      FALSE                       - Failure (Not found)                               */
/*                                                                                      */
/* Description:                                                                         */
/*      Find usb device.                                                                */
/*                                                                                      */
/*******************************************|********************************************/
static EPS_BOOL FindDevice	(
							
		EPS_INT32*		pCounter,
		EPS_USB_DEVICE* dev
		
){
	EPS_INT32   i;
    EPS_INT8    usb_path[MAX_PATH_LENGTH];
    EPS_INT32   FoundDevice = FALSE;
	EPS_FILEDSC fd = -1;

    for (i = *pCounter; (FoundDevice == FALSE) && (i < MAX_DEVICES); i++) {
        
        /* Increment the USB device path string - i.e. /dev/usb/lp0,        */
        /* /dev/usb/lp1, /dev/usb/lp2, /dev/usb/lp3, etc...                 */
        /* This is the typical location for USB printers on most Linux      */
        /* systems. If your particular Linux platform uses a different      */
        /* device path, change the PATH_TO_USB_BUS string accordingly.      */
        snprintf(usb_path, MAX_PATH_LENGTH, "%s%d", PATH_TO_USB_BUS, i);
//fprintf(stderr, "FindDevice %s\n", usb_path);
        /* Try to open the USB path in read-write mode and get a file descriptor */
        fd = open(usb_path, O_RDWR);
        
		if (fd >= 0){
			if( GetDeviceNumbers(fd, dev) ){
				FoundDevice = TRUE;
				close(fd);
				i++;
				break;
			}

			close(fd);
		} else{
//fprintf(stderr, "FindDevice %s\n", strerror(errno));
		}
    }

	*pCounter = i;

	return FoundDevice;
}


/*******************************************|********************************************/
/* Function name:   GetDeviceNumbers()                                                  */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* fd           EPS_FILEDSC         I: File discripter. (It's Opened)                   */
/* dev          EPS_USB_DEVICE      O: pointer to a device info.                        */
/*                                                                                      */
/* Return value:                                                                        */
/*      TRUE                        - Success                                           */
/*      FALSE                       - Failure                                           */
/*                                                                                      */
/* Description:                                                                         */
/*      Get usb device Info(VID, PID, Bus Address) And fill the EPS_USB_DEVICE structure.*/
/*                                                                                      */
/*******************************************|********************************************/
static EPS_BOOL GetDeviceNumbers(
						  
		EPS_FILEDSC fd, 
		EPS_USB_DEVICE* dev
		
){
    debug_msg("Start call function epsmp-linux-portal-io.c/GetDeviceNumbers \n");
    EPS_INT32   iostatus;                       /* Status of the initial I/O request    */
	int 		val[2];
	
    /* ***************************************************************************
     * Since the file descriptor seems to be OK, let's try and get a VID, PID. 
     * Epson ESC/P-R printers conform to the USB printer class specification.
     * Lucky for us, the Linux kernel has some definitions for printer class
     * commands.
     * 
     * Therefore, we're going to avoid the bare-metal approach (by using the 
     * read() and write() functions) to get the printer's VID, PID; instead,
     * we'll use the ioctl (input-output control) function to access the Linux
     * kernel's internal printer class driver.
     *                  
     * So let's take a look at how ioctl actually works in the Linux kernel.
     * 
     * This is the encoding of an ioctl command:
     * 
     *              size of the parameter structure
     *                          |
     *    access mode           |
     *              |           |            command
     *              |           |               |
     *              -- ----------------- -------------------
     *              00 00 0000 0000 0000 0000 0000 0000 0000
     *              -- ----------------- --------- ---------
     *              |           |           |           |
     *  +-----------+           |           |           ordinal (sequential) number
     *  |                       |           |           8 bits (_IOC_NRBITS)
     *  |                       |           |
     *  |                       |       type (magic number)
     *  |                       |       device major number
     *  |                       |       8 bits (_IOC_TYPEBITS)
     *  |                       size
     *  direction               14 bits (_IOC_SIZEBITS)
     *  2 bits (_IOC_DIRBITS)
     * 
     * Epson ESC/P-R printers behave according to the USB printer class specification.
     * Therefore, there is a standard command, GET_DEVICE_ID, which will return the
     * printer's VID, PID. Therefore, we use the ioctl function (with the file
     * descriptor for the Linux device) and send the correct command to the printer.
     *  
     * This is accomplished by using the _IOC() command to put together all our requests
     * a single ioctl command. We're going to use the following data, do the bit-bashing 
     * with _IOC(), and send the whole request to the IO subsystem using ioctl():
     *
     * _IOC_DIRBITS:    _IOC_READ (2 bits corresponding to "read")
     * _IOC_TYPEBITS:   'P' (USB printer class)
     * _IOC_NRBITS:     1
     * _IOC_SIZEBITS:   DeviceID.length (we only really care about the first part
     *                  of the device ID string, which contains the manufacturer
     *                  and model data)
     * 
     * At this point you may be wondering why the command is '6'. The Linux kernel
     * source file "Linux/drivers/usb/class/usblp.c" actually contains the 
     * following definitions:
     * 
     * GET_DEVICE_ID    1
     * GET_PROTOCOLS    2
     * SET_PROTOCOL     3
     * HP_SET_CHANNEL   4
     * GET_BUS_ADDRESS  5
     * GET_VID_PID      6
     * SOFT_RESET       7
     * 
     * You must have the "USB printer" driver enabled in the Linux kernel for this
     * USB printer class command to work. If you find the ioctl doesn't appear to 
     * do anything, check your kernel configuration and recompile if necessary.
     * 
     * 
     */ 

     /* GET_VID_PID */
	iostatus = ioctl(fd, _IOC(_IOC_READ, 'P', 6, sizeof(val)), val);
    if (iostatus < 0){
        return FALSE;
    }

	dev->vid = val[0];
	dev->pid = val[1];
     
     /* GET_BUS_ADDRESS */
    iostatus = ioctl(fd, _IOC(_IOC_READ, 'P', 5, sizeof(val)), val);
    if (iostatus < 0){
        return FALSE;
    }
    
    dev->port = (val[0] << 16) + val[1];

    debug_msg("Exit call function epsmp-linux-portal-io.c/GetDeviceNumbers \n");
	return TRUE;
}


/*******************************************|********************************************/
/* Function name:   epsmpGetDeviceID()                                                  */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* fd           EPS_FILEDSC         I: File discripter. (It's Opened)                   */
/* buffer       EPS_INT8*           O: IEEE1284 Device ID String.                       */
/* bufLen       EPS_INT32*          O: Lingth of Device ID String.                      */
/*                                                                                      */
/* Return value:                                                                        */
/*      0               - Success                                                       */
/*      -1              - Failure                                                       */
/*                                                                                      */
/* Description:                                                                         */
/*      Get device ID stringe.                                                          */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32 epsmpGetDeviceID(EPS_FILEDSC fd, EPS_INT8* buffer, EPS_INT32* bufLen)
{
    debug_msg("Start call function epsmp-linux-portal-io.c/epsmpGetDeviceID \n");
    EPS_INT32   iostatus;                       /* Status of the initial I/O request    */
	unsigned char device_id [MAX_DEV_ID_LENGTH];
    int len;

    iostatus = ioctl(fd, _IOC(_IOC_READ, 'P', 1, sizeof(device_id)), device_id);
    if (iostatus < 0){
        return -1;
    }

	len = (((unsigned int)device_id[0] & 0xFF) << 8) 
		+ (device_id[1] & 0xFF);

	if(len > *bufLen){
		memcpy(buffer, device_id, *bufLen);
	} else{
		memcpy(buffer, device_id, len);
	}
	*bufLen = len;

    debug_msg("Exit call function epsmp-linux-portal-io.c/epsmpGetDeviceID \n");
	return 0;
}


/*******************************************|********************************************/
/* Function name:   epsmpGetDeviceID()                                                  */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* fd           EPS_FILEDSC         I: File discripter. (It's Opened)                   */
/*                                                                                      */
/* Return value:                                                                        */
/*      0               - Success                                                       */
/*      -1              - Failure                                                       */
/*                                                                                      */
/* Description:                                                                         */
/*      Soft reset to printer.                                                          */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32 epsmpSoftReset(EPS_FILEDSC fd)
{
    EPS_INT32   iostatus;                       /* Status of the initial I/O request    */
    iostatus = ioctl(fd, _IOC(_IOC_NONE, 'P', 7, 0), NULL);
    if (iostatus < 0){
        return -1;
    }

	return 0;
}

#endif

/*____________________________   epsmp-linux-portal-io.c   _____________________________*/
  
/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*       1         2         3         4         5         6         7         8        */
/*******************************************|********************************************/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/***** End of File *** End of File *** End of File *** End of File *** End of File ******/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
