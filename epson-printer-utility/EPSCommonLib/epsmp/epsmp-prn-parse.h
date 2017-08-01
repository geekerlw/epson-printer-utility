/*________________________________  epsmp-prn-parse.h   ________________________________*/

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
/*                Epson Parse Script File Functions External Definitions                */
/*                                                                                      */
/*                                    Function Calls                                    */
/*                              --------------------------                              */
/*    EPS_ERR_CODE epsmpGetPageAttribute (ScriptFileName, ImageBand, PageAttribute);    */
/*    EPS_INT32    epsmpGetScriptLineNum (void);                                        */
/*                                                                                      */
/*******************************************|********************************************/
#ifndef __EPSON_PRN_PARSE_H__
#define __EPSON_PRN_PARSE_H__
#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------  Includes   -------------------------------------*/
/*******************************************|********************************************/
#include "../escpr-lib/inc/epson-escpr-api.h"
#include "epsmp-image.h"

/*------------------------------------- Data Types -------------------------------------*/
/*******************************************|********************************************/

/*----------------------------------  Generic Macros   ---------------------------------*/
/*******************************************|********************************************/

/*-----------------------------------  Definitions  ------------------------------------*/
/*******************************************|********************************************/
    /*** Error Codes                                                                    */
    /*** -------------------------------------------------------------------------------*/
#define EPSMP_PRS_ERR_NONE                  EPS_ERR_NONE
#define EPSMP_PRS_ERR_INVALID_ARG           -5400
#define EPSMP_PRS_ERR_TESTSCRIPT_IO         -5401
#define EPSMP_PRS_ERR_INVALID_NUM_PARAM     -5402
#define EPSMP_PRS_ERR_END_OF_TEST           -5403
#define EPSMP_PRS_ERR_UNKNOWN_KEY           -5404
#define EPSMP_PRS_ERR_INVALID_PARAM         -5405

/*---------------------------  Public Function Declarations  ---------------------------*/
/*******************************************|********************************************/
EPS_ERR_CODE    epsmpGetPageAttribute (const EPS_INT8*, EPS_JOB_ATTRIB*			);
EPS_ERR_CODE    epsmpGetPageAttributeFromImage (EPSMP_IMAGE_BAND*, EPS_JOB_ATTRIB*  );
EPS_INT32       epsmpGetScriptLineNum (void                                         );


#ifdef __cplusplus
}
#endif
#endif	/* __EPSON_PRN_PARSE_H__ */

/*________________________________  epsmp-prn-parse.h   ________________________________*/
  
/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*       1         2         3         4         5         6         7         8        */
/*******************************************|********************************************/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/***** End of File *** End of File *** End of File *** End of File *** End of File ******/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

