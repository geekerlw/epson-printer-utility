/*_________________________________  epsmp-scale.h   ___________________________________*/

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
/*                     Epson Scaling Functions External Definitions                     */
/*                                                                                      */
/*                                    Function Calls                                    */
/*                              --------------------------                              */
/*        EPS_ERR_CODE epsmpInitScale    (ImageBand, PrintableWidth, PrintableHeight);  */
/*        EPS_ERR_CODE epsmpScale        (ImageBand);                                   */
/*        EPS_ERR_CODE epsmpReleaseScale (void);                                        */
/*                                                                                      */
/*******************************************|********************************************/
#ifndef __EPSON_SCALE_H__
#define __EPSON_SCALE_H__
#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------  Includes   -------------------------------------*/
/*******************************************|********************************************/
#include "../escpr-lib/inc/epson-typedefs.h"
#include "epsmp-image.h"

/*------------------------------------- Data Types -------------------------------------*/ 
/*******************************************|********************************************/ 

/*----------------------------------  Generic Macros   ---------------------------------*/
/*******************************************|********************************************/

/*-----------------------------------  Definitions  ------------------------------------*/
/*******************************************|********************************************/
#define EPSMP_SCALE_ERR_NONE                            0   /* Routine / Action completed successfully  */
#define EPSMP_SCALE_ERR_MEMORY_ALLOCATION           -5500   /* Memory Allocation Error                  */
#define EPSMP_SCALE_ERR_INVALID_ARG                 -5501
#define EPSMP_SCALE_ERR_INVALID_STRUCT              -5502
#define EPSMP_SCALE_ERR_INVALID_IMAGE_PARAM         -5503
#define EPSMP_SCALE_ERR_INVALID_PRINTABLE_PARAM     -5504
#define EPSMP_SCALE_ERR_INVALID_BAND_PARAM          -5505
#define EPSMP_SCALE_ERR_INVALID_BPP                 -5506
#define EPSMP_SCALE_ERR_NOT_INITIALIZED             -5507

/*---------------------------  Data Structure Declarations   ---------------------------*/
/*******************************************|********************************************/

/*---------------------------  Public Function Declarations  ---------------------------*/
/*******************************************|********************************************/
EPS_ERR_CODE    epsmpInitScale    (const EPSMP_IMAGE_BAND*, const EPS_UINT32, const EPS_UINT32);
EPS_ERR_CODE    epsmpScale        (EPSMP_IMAGE_BAND*, EPS_INT32*                  );
EPS_ERR_CODE    epsmpReleaseScale (void                                                 );


#ifdef __cplusplus
}
#endif
#endif	/* __EPSON_SCALE_H__ */

/*_________________________________  epsmp-scale.h   ___________________________________*/
  
/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*       1         2         3         4         5         6         7         8        */
/*******************************************|********************************************/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/***** End of File *** End of File *** End of File *** End of File *** End of File ******/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
