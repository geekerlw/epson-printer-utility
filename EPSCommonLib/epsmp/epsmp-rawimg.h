/*_________________________________  epsmp-rawimg.h   __________________________________*/

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
/*                    Epson Image Band Functions External Definitions                   */
/*                                                                                      */
/*                                    Function Calls                                    */
/*                              --------------------------                              */
/*                   EPS_INT32  epsmpRawInit            (File, bandHeight, reverse);    */
/*                   EPS_INT32  epsmpRawGetWidth        (void                   );      */
/*                   EPS_INT32  epsmpRawGetHeight       (void                   );      */
/*                   EPS_INT32  epsmpRawGetBytesPerLine (void                   );      */
/*                   EPS_INT32  epsmpRawGetBytesPerPixel(void                   );      */
/*                   EPS_INT32  epsmpRawGetBandSize     (void                   );      */
/*                   EPS_INT32  epsmpRawGetBandData     (buf                    );      */
/*                   EPS_INT32  epsmpRawGetBandWidth    (void                   );      */
/*                   EPS_INT32  epsmpRawGetBandHeight   (void                   );      */
/*                   EPS_INT32  epsmpRawGetPaletteSize  (void                   );      */
/*                   EPS_UINT8* epsmpRawGetPaletteData  (void                   );      */
/*                   EPS_INT32  epsmpRawRelease         (void                   );      */
/*                                                                                      */
/*******************************************|********************************************/
#ifndef __EPSON_RAWIMG_H__
#define __EPSON_RAWIMG_H__
#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------  Includes   -------------------------------------*/
/*******************************************|********************************************/
#include "epson-typedefs.h"

/*------------------------------------- Data Types -------------------------------------*/ 
/*******************************************|********************************************/ 

/*----------------------------------  Generic Macros   ---------------------------------*/
/*******************************************|********************************************/

/*-----------------------------------  Definitions  ------------------------------------*/
/*******************************************|********************************************/
        /*** BMP Error Codes                                                            */
        /*** ---------------------------------------------------------------------------*/
#define EPSMP_RAW_ERR_NONE                      0     /* Routine / Action completed successfully  */
#define EPSMP_RAW_ERR_MEMORY_ALLOCATION     -5400     /* Memory Allocation Error          */
#define EPSMP_RAW_ERR_INVALID_ARG           -5401     /* Invalid Argument in call Error   */
#define EPSMP_RAW_ERR_NOT_INITIALIZED       -5402
#define EPSMP_RAW_ERR_INVALID_FILE          -5403	

/*---------------------------  Data Structure Declarations   ---------------------------*/
/*******************************************|********************************************/

/*---------------------------  Public Function Declarations  ---------------------------*/
/*******************************************|********************************************/
extern EPS_INT32  epsmpRawInit             (FILE* fp, EPS_INT32, EPS_BOOL);
extern EPS_INT32  epsmpRawGetWidth         (void					);
extern EPS_INT32  epsmpRawGetHeight        (void					);
extern EPS_INT32  epsmpRawGetBytesPerLine  (void					);
extern EPS_UINT8  epsmpRawGetBytesPerPixel (void					);
extern EPS_INT32  epsmpRawGetBandSize      (void					);
extern EPS_INT32  epsmpRawGetBandData      (EPS_UINT8**				);
extern EPS_INT32  epsmpRawGetBandWidth     (void					);
extern EPS_INT32  epsmpRawGetBandHeight    (void					);
extern EPS_UINT16 epsmpRawGetPaletteSize   (void					);
extern EPS_UINT8* epsmpRawGetPaletteData   (void					);
extern EPS_INT32  epsmpRawRelease          (void					);
extern EPS_INT32  epsmpRawReset            (EPS_BOOL				);

#ifdef __cplusplus
}
#endif
#endif	/* __EPSON_RAWIMG_H__ */

/*_________________________________  epsmp-rawimg.h   __________________________________*/
  
/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*       1         2         3         4         5         6         7         8        */
/*******************************************|********************************************/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/***** End of File *** End of File *** End of File *** End of File *** End of File ******/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
