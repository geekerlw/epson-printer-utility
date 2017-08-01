/*_____________________________   demo-linux-portal-io.h   _____________________________*/

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
/*                        Linux Portal I/O External Definitions                         */
/*                                                                                      */
/*                                    Function Calls                                    */
/*                              --------------------------                              */
/*                  EPS_FILEDSC epsmpOpenPortal  (dev								);  */
/*                  EPS_INT32   epsmpClosePortal (fd								);  */
/*                  EPS_INT16   epsmpReadPortal  (fd, Data, Length, LengthRead		);  */
/*                  EPS_INT16   epsmpWritePortal (fd, Data, Length, LengthWritten	);  */
/*                  EPS_FILEDSC epsmpFindFirst   (dev								);  */
/*                  EPS_BOOL	epsmpFindNext    (fd, dev							);  */
/*                  EPS_BOOL	epsmpFindClose   (fd								);  */
/*                                                                                      */
/*******************************************|********************************************/
#ifndef _DEMO_LINUX_PORTAL_IO_H_
#define _DEMO_LINUX_PORTAL_IO_H_
#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------  Includes   -------------------------------------*/
/*******************************************|********************************************/
#include "../../escpr-lib/inc/epson-escpr-api.h"
//#include "epson-escpr-def.h"

/*------------------------------------- Data Types -------------------------------------*/
/*******************************************|********************************************/

/*----------------------------------  Generic Macros   ---------------------------------*/
/*******************************************|********************************************/

/*-----------------------------------  Definitions  ------------------------------------*/
/*******************************************|********************************************/

/*---------------------------  Data Structure Declarations   ---------------------------*/
/*******************************************|********************************************/

/*---------------------------  Public Function Declarations  ---------------------------*/
/*******************************************|********************************************/
EPS_FILEDSC epsmpOpenPortal (const EPS_USB_DEVICE*		                                );
EPS_INT32   epsmpClosePortal(EPS_FILEDSC                                                );
EPS_INT32   epsmpReadPortal (EPS_FILEDSC, EPS_UINT8*, EPS_INT32, EPS_INT32*             );
EPS_INT32   epsmpWritePortal(EPS_FILEDSC, const EPS_UINT8*, EPS_INT32, EPS_INT32*       );

EPS_FILEDSC epsmpFindFirst	(EPS_USB_DEVICE*                                            );
EPS_BOOL    epsmpFindNext	(EPS_FILEDSC, EPS_USB_DEVICE*                               );
EPS_BOOL    epsmpFindClose  (EPS_FILEDSC                                                );
EPS_INT32	epsmpGetDeviceID(EPS_FILEDSC fd, EPS_INT8* buffer, EPS_INT32*               );
EPS_INT32	epsmpSoftReset  (EPS_FILEDSC fd                                             );


#ifdef __cplusplus
}
#endif
#endif /* def _DEMO_LINUX_PORTAL_IO_H_ */

/*_____________________________   demo-linux-portal-io.h   _____________________________*/
  
/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*       1         2         3         4         5         6         7         8        */
/*******************************************|********************************************/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/***** End of File *** End of File *** End of File *** End of File *** End of File ******/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
