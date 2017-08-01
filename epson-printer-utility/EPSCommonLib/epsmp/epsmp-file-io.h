/*_______________________________    epsmp-file-io.h   _________________________________*/

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
/*                             File I/O External Definitions                            */
/*                                                                                      */
/*                                    Function Calls                                    */
/*                              --------------------------                              */
/*                  EPS_INT32   epsmpOpenFile  (void                       );           */
/*                  EPS_INT32   dummyReadFile  (fd, Data, Length, ReadLength   );       */
/*                  EPS_INT32   epsmpWriteFile (fd, Data, Length, LengthWritten);       */
/*                  EPS_INT32   epsmpCloseFile (void                       );           */
/*                                                                                      */
/*******************************************|********************************************/
#ifndef _EPSMP_FILE_IO_H_
#define _EPSMP_FILE_IO_H_
#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------  Includes   -------------------------------------*/
/*******************************************|********************************************/
#include "../escpr-lib/inc/epson-typedefs.h"
//#include "../escpr-lib/inc/epson-escpr-def.h"

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
EPS_FILEDSC epsmpOpenFile       (const EPS_USB_DEVICE*                          );
EPS_INT32   dummyReadFile       (EPS_FILEDSC, EPS_UINT8*, EPS_INT32, EPS_INT32* );
EPS_INT32   epsmpWriteFile      (EPS_FILEDSC, const EPS_UINT8*, EPS_INT32, EPS_INT32* );
EPS_INT32   epsmpCloseFile      (EPS_FILEDSC                                    );



#ifdef __cplusplus
}
#endif
#endif /* def _EPSMP_FILE_IO_H_ */

/*_______________________________    epsmp-file-io.h   _________________________________*/
  
/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*       1         2         3         4         5         6         7         8        */
/*******************************************|********************************************/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/***** End of File *** End of File *** End of File *** End of File *** End of File ******/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
