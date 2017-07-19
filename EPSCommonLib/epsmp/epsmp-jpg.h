/*___________________________________  epsmp-jpg.h   ___________________________________*/

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
/*                      Epson Jpeg Functions External Definitions                       */
/*                                                                                      */
/*                                    Function Calls                                    */
/*                              --------------------------                              */
/*                  EPS_INT32  epsmpJpgInit            (File, bandHeight);              */
/*                  EPS_INT32  epsmpJpgGetWidth        (void           );               */
/*                  EPS_INT32  epsmpJpgGetHeight       (void           );               */
/*                  EPS_INT32  epsmpJpgGetBytesPerLine (void           );               */
/*                  EPS_INT32  epsmpJpgGetBytesPerPixel(void           );               */
/*                  EPS_INT32  epsmpJpgGetBandSize     (void           );               */
/*                  EPS_INT32  epsmpJpgGetBandData     (buf            );               */
/*                  EPS_INT32  epsmpJpgGetBandWidth    (void           );               */
/*                  EPS_INT32  epsmpJpgGetBandHeight   (void           );               */
/*                  EPS_INT32  epsmpJpgGetPaletteSize  (void           );               */
/*                  EPS_UINT8* epsmpJpgGetPaletteData  (void           );               */
/*                  EPS_INT32  epsmpJpgRelease         (void           );               */
/*                                                                                      */
/*******************************************|********************************************/
#ifndef __EPSON_JPG_H__
#define __EPSON_JPG_H__
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

        /*** Jpeg Error Codes                                                           */
        /*** ---------------------------------------------------------------------------*/
#define EPSMP_JPG_ERR_NONE                      0
#define EPSMP_JPG_ERR_MEMORY_ALLOCATION     -5300       /* Memory Allocation Error      */
#define EPSMP_JPG_ERR_NOT_INITIALIZED       -5301
#define EPSMP_JPG_ERR_INVALID_FILE          -5302
#define EPSMP_JPG_ERR_READING_DATA          -5303
#define EPSMP_JPG_ERR_INVALID_PARAM         -5304

/*---------------------------  Public Function Declarations  ---------------------------*/
/*******************************************|********************************************/
extern EPS_INT32  epsmpJpgInit              (FILE*, EPS_INT32, EPS_BOOL);

extern EPS_INT32  epsmpJpgGetWidth          (void               );
extern EPS_INT32  epsmpJpgGetHeight         (void               );
extern EPS_INT32  epsmpJpgGetBytesPerLine   (void               );
extern EPS_UINT8  epsmpJpgGetBytesPerPixel  (void               );

extern EPS_INT32  epsmpJpgGetBandData       (EPS_UINT8**        );
extern EPS_INT32  epsmpJpgGetBandHeight     (void               );
extern EPS_INT32  epsmpJpgGetBandWidth      (void               );
extern EPS_INT32  epsmpJpgGetBandSize       (void               );

extern EPS_UINT8* epsmpJpgGetPaletteData    (void               );
extern EPS_UINT16 epsmpJpgGetPaletteSize    (void               );

extern EPS_INT32  epsmpJpgRelease           (void				);
extern EPS_INT32  epsmpJpgReset             (EPS_BOOL			);

extern EPS_INT32  epsmpJpgRotate			(const char*, const char*);
extern EPS_INT32  epsmpJpgGetSize           (const char*, EPS_UINT32*, EPS_UINT32*, EPS_UINT32*);
extern EPS_INT32  epsmpJpgReduction			(const char*, const char*, EPS_INT32, EPS_INT32);
extern EPS_INT32  epsmpJpgReductionSize 	(const char*, const char*, EPS_UINT32, EPS_INT32);

extern EPS_INT32  epsmpBmp2Jpg(const EPS_INT8*, const EPS_INT8*);

#ifdef __cplusplus
}
#endif
#endif	/* __EPSON_JPG_H__ */

/*___________________________________  epsmp-jpg.h   ___________________________________*/
  
/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*       1         2         3         4         5         6         7         8        */
/*******************************************|********************************************/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/***** End of File *** End of File *** End of File *** End of File *** End of File ******/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

