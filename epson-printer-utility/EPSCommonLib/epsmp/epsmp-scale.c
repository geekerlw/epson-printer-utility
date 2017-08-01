/*__________________________________  epsmp-scale.c   __________________________________*/

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
/*                               Epson Scaling Functions                                */
/*                                                                                      */
/*                                    Function Calls                                    */
/*                              --------------------------                              */
/*        EPS_ERR_CODE epsmpInitScale    (ImageBand, PrintableWidth, PrintableHeight);  */
/*        EPS_ERR_CODE epsmpScale        (ImageBand);                                   */
/*        EPS_ERR_CODE epsmpReleaseScale (void);                                        */
/*                                                                                      */
/*******************************************|********************************************/

/*------------------------------------  Includes   -------------------------------------*/
/*******************************************|********************************************/
#include <stdlib.h>
#include <string.h>

#include "epsmp-scale.h"

/*------------------------------------  Definition   -----------------------------------*/
/*******************************************|********************************************/

/*-------------------------------  Definition of Macro  --------------------------------*/
/*******************************************|********************************************/
    /*** Max, Min                                                                       */
    /*** ------------------------------------------------------------------------------ */
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

/*--------------------------------  Local Data Structs  --------------------------------*/
/*******************************************|********************************************/
    /*** Information for scaling                                                        */
    /*** -------------------------------------------------------------------------------*/
typedef struct  tag_EPS_SCALE_DATA {
    EPS_INT32   factorWhole;
    EPS_INT32   factorFract;
    EPS_INT32   yAcc;
    EPS_INT32   bandWidth;
    EPS_INT32   newBandWidth;
    EPS_INT32   bpp;
    EPS_INT32   xOffset;
    EPS_INT32   printableWidth;
    EPS_INT32   printableHeight;
    EPS_INT32   yCnt;
    EPS_UINT8*  buf;
} EPS_SCALE_DATA;

/*------------------------------  Local Global Variables  ------------------------------*/
/*******************************************|********************************************/
    /*** Scaling State                                                                  */
    /*** ------------------------------------------------------------------------------ */
static EPS_INT32    scaleState = -1;        /* State of scaling routines                */
                                            /* -1 = Scaling not initialized             */
                                            /*  0 = Scaling ok                          */

static EPS_SCALE_DATA   sd;                 /* Scaling Data                             */
static EPS_SCALE_DATA*  scaleData = &sd;    /* Pointer to Scaling Data                  */
                                            /* This declaration was used for the future */
                                            /* where the scaleData would be part of a   */
                                            /* print job struct when supporting multiple*/
                                            /* jobs.                                    */

/*--------------------------------  Local Functions   ----------------------------------*/
/*******************************************|********************************************/
static void InitScaleDataParam(void);

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpInitScale()                                                    */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:            Type:               Description:                                    */
/* imageBand        EPSMP_IMAGE_BAND*   I: Image Band Information                       */
/* printableWidth   const EPS_UINT32    I: Printable Area - Width                       */
/* printableHeight  const EPS_UINT32    I: Printable Area - Height                      */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPSMP_SCALE_ERR_NONE                        - Success                           */
/*      EPSMP_SCALE_ERR_MEMORY_ALLOCATION                                               */
/*      EPSMP_SCALE_ERR_INVALID_IMAGE_PARAM                                             */
/*      EPSMP_SCALE_ERR_INVALID_PRINTABLE_PARAM                                         */
/*      EPSMP_SCALE_ERR_INVALID_BAND_PARAM                                              */
/*      EPSMP_SCALE_ERR_INVALID_BPP                                                     */
/*                                                                                      */
/* Description:                                                                         */
/*      Initialize the data for scaling.                                                */
/*                                                                                      */
/*******************************************|********************************************/
EPS_ERR_CODE    epsmpInitScale (
        
        const EPSMP_IMAGE_BAND*   imageBand,
        const EPS_UINT32    printableWidth,
        const EPS_UINT32    printableHeight

){

/*** Declare Variable Local to Routine                                                  */
    EPS_INT32   xScaleFactor;
    EPS_INT32   yScaleFactor;
    EPS_INT32   scaleFactor;
    EPS_INT32   newBandHeight;
    
/*** Initialize Global/Local Variables                                                  */
    scaleState       = -1;
    InitScaleDataParam();

/*** Check Input Data                                                                   */
    if ((imageBand->imageWidth  <= 0) ||
        (imageBand->imageHeight <= 0)    )
        return((EPS_ERR_CODE)EPSMP_SCALE_ERR_INVALID_IMAGE_PARAM);
    
    if ((printableWidth  <= 0) ||
        (printableHeight <= 0)    )
        return((EPS_ERR_CODE)EPSMP_SCALE_ERR_INVALID_PRINTABLE_PARAM);
    
    if ((imageBand->bandWidth  <= 0) ||
        (imageBand->bandHeight <= 0)    )
        return((EPS_ERR_CODE)EPSMP_SCALE_ERR_INVALID_BAND_PARAM);

    if (imageBand->bpp == 0)
        return((EPS_ERR_CODE)EPSMP_SCALE_ERR_INVALID_BPP);

/*** Initialize Scaling Parameters                                                      */
    xScaleFactor = (EPS_INT32)((1000 * printableWidth)  / imageBand->imageWidth );
    yScaleFactor = (EPS_INT32)((1000 * printableHeight) / imageBand->imageHeight);

    /* get the maximum of the two scale factors */
    scaleFactor = MAX(xScaleFactor, yScaleFactor);

    scaleData->factorWhole = (EPS_INT32)scaleFactor/1000;
    scaleData->factorFract = scaleFactor - (scaleData->factorWhole * 1000);

    if (scaleData->factorFract > 0) {
        newBandHeight = imageBand->bandHeight*(scaleData->factorWhole+1);
    } else {
        newBandHeight = imageBand->bandHeight*(scaleData->factorWhole);
    }

    scaleData->xOffset = 0;
    scaleData->yCnt    = 0;

    if (scaleFactor == yScaleFactor) {
        scaleData->xOffset = (EPS_INT32)((imageBand->bandWidth * scaleFactor - 1000 * (EPS_INT32)printableWidth) / 2000);
    } else {
        scaleData->yCnt    = -(imageBand->imageHeight * scaleFactor - 1000 * (EPS_INT32)printableHeight) / 2000;
    }

/*** Determine new (output) image sizing/parametrics                                    */
    scaleData->bpp             = imageBand->bpp;
    scaleData->bandWidth       = imageBand->bandWidth;
    scaleData->newBandWidth    = (EPS_INT32)MAX(printableWidth,  (EPS_UINT32)(imageBand->bandWidth  * scaleFactor / 1000));
    scaleData->yAcc            = 0;
    scaleData->printableWidth  = printableWidth;
    scaleData->printableHeight = (EPS_INT32)MIN(printableHeight, (EPS_UINT32)(imageBand->imageHeight* scaleFactor / 1000));
    
//    scaleData->buf = malloc(newBandHeight * scaleData->newBandWidth * scaleData->bpp);
    if (scaleData->buf == NULL)
        return (EPS_ERR_CODE)EPSMP_SCALE_ERR_MEMORY_ALLOCATION;

/*** Return to Caller                                                                   */
    scaleState = 0;
    return (EPS_ERR_CODE)EPSMP_SCALE_ERR_NONE;
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpScale()                                                        */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:            Type:               Description:                                    */
/* imageBand        EPSMP_IMAGE_BAND*   I/O: Image Band Information                     */
/* numScaledBand    EPS_INT32           O:   Number of Scaled Band                      */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPSMP_SCALE_ERR_NONE                    - Success                               */
/*      EPSMP_SCALE_ERR_NOT_INITIALIZED                                                 */
/*      EPSMP_SCALE_ERR_INVALID_ARG                                                     */
/*      EPSMP_SCALE_ERR_INVALID_STRUCT                                                  */
/*      EPSMP_SCALE_ERR_INVALID_BAND_PARAM                                              */
/*                                                                                      */
/* Description:                                                                         */
/*      Scale the band data.                                                            */
/*                                                                                      */
/*******************************************|********************************************/
EPS_ERR_CODE    epsmpScale (

        EPSMP_IMAGE_BAND*   imageBand,
        EPS_INT32*          numScaledBand

){

    EPS_INT32 xAcc;
    EPS_INT32 cnt;
    EPS_INT32 idx,jdx,hdx;
    EPS_INT32 xExtra;
    EPS_INT32 yExtra;
    EPS_INT32 bandsPrinted;             /* bands printed                                */
    EPS_INT32 bandsPrintedTotal;
    EPS_INT32 extraBandsPrinted;        /* number of extra bands (>= 0)                 */
    EPS_INT32 extraBandsCounter;        /* global extra bands (can be < 0)              */
    EPS_UINT8* srcBuf;
    EPS_UINT8* buf;
    EPS_INT32 yCntLocal;
    EPS_INT32 offset;

    const EPS_INT32 newBandBPL = scaleData->newBandWidth * scaleData->bpp;
    const EPS_INT32 bandHeight = imageBand->bandHeight;

/*** Has Scaling been Initialized                                                       */
    if (scaleState == -1)
        return((EPS_ERR_CODE)EPSMP_SCALE_ERR_NOT_INITIALIZED);

/*** Are input parameter valid                                                          */
    if (!imageBand)
        return((EPS_ERR_CODE)EPSMP_SCALE_ERR_INVALID_ARG);

/*** Initialize Local Variables                                                         */
    srcBuf            = imageBand->bandData;
    yCntLocal         = 0;
    buf               = scaleData->buf;
    bandsPrintedTotal = 0;

    
/*** Check Input Data                                                                   */
    if  (scaleData    == NULL) return((EPS_ERR_CODE)EPSMP_SCALE_ERR_INVALID_STRUCT);
    if  (imageBand    == NULL) return((EPS_ERR_CODE)EPSMP_SCALE_ERR_INVALID_STRUCT);

    if ((imageBand->rect.top    <  0) || 
        (imageBand->rect.left   <  0) ||
        (imageBand->rect.bottom <  0) ||
        (imageBand->rect.right  <  0) ||
        ((imageBand->rect.bottom - imageBand->rect.top )  <=  0) ||
        ((imageBand->rect.right  - imageBand->rect.left)  <=  0)  )
        return((EPS_ERR_CODE)EPSMP_SCALE_ERR_INVALID_BAND_PARAM); 

/*** Scale Input Data                                                                   */
    for (hdx=0; hdx < bandHeight; hdx++) {
        memset(buf, 255, (scaleData->newBandWidth * scaleData->bpp));
        cnt    = 0;
        xAcc   = 0;
        xExtra = 0;
        yExtra = 0;

        /* process yAcc */
        scaleData->yAcc += scaleData->factorFract;
        if (scaleData->yAcc >= 1000) {
            scaleData->yAcc -= 1000;
            yExtra = 1;
        }

        bandsPrinted      = (scaleData->factorWhole + yExtra);
        extraBandsCounter = (scaleData->factorWhole + yExtra) - 1;
        extraBandsPrinted = extraBandsCounter;

        if (extraBandsPrinted < 0)
            extraBandsPrinted = 0;

        if (bandsPrinted > 0) {
            for (idx=0; idx < scaleData->bandWidth; idx++) {
                /* process xAcc */
                xAcc += scaleData->factorFract;
                if (xAcc >= 1000) { 
                    xAcc  -= 1000;
                    xExtra = 1;
                } else {
                    xExtra = 0;
                }

                for(jdx=0; jdx < (scaleData->factorWhole + xExtra); jdx++) {
                    memcpy((buf + cnt), (srcBuf + idx * scaleData->bpp), scaleData->bpp);
                    cnt += scaleData->bpp;
                }
            }

			for(; cnt < newBandBPL; ) {
				memcpy((buf + cnt), (srcBuf + (scaleData->bandWidth-1) * scaleData->bpp), scaleData->bpp);
				cnt += scaleData->bpp;
			}

            buf += newBandBPL;
        }

        for (idx=1; idx < bandsPrinted; idx++) {
            memcpy(buf,buf-newBandBPL,newBandBPL);
            buf += newBandBPL;
        }

        yCntLocal         += extraBandsCounter;
        bandsPrintedTotal += bandsPrinted;
        srcBuf            += imageBand->bpl;
    }

    /* adjust the rectangle */
    imageBand->rect.right   = scaleData->printableWidth; 
    imageBand->rect.top    += scaleData->yCnt;
    imageBand->rect.bottom  = imageBand->rect.top + bandsPrintedTotal;

    /* set the band bmp */
    imageBand->bandData = (EPS_UINT8*)(scaleData->buf + scaleData->xOffset * scaleData->bpp);
    imageBand->bpl      = newBandBPL;

    if (imageBand->rect.top < 0) {
        if (imageBand->rect.bottom <= 0) {
            /* this is getting cropped */
            bandsPrintedTotal = 0;
        } else {
            offset               = 0 - imageBand->rect.top;
            imageBand->bandData += offset * imageBand->bpl;
            imageBand->rect.top  = 0;
            /*
            bandsPrinted     -= offset;
            */
        }
    }

    if (imageBand->rect.bottom > scaleData->printableHeight) {
        if (imageBand->rect.top < scaleData->printableHeight) {
            offset                   = imageBand->rect.bottom - scaleData->printableHeight;
            imageBand->rect.bottom   = scaleData->printableHeight;
            bandsPrintedTotal       -= offset;
        } else {
            bandsPrintedTotal        = 0;
        }
    } 

    scaleData->yCnt += yCntLocal;

    imageBand->bandWidth  = scaleData->newBandWidth;
    imageBand->bandHeight = imageBand->rect.bottom - imageBand->rect.top;

/*** Return to Caller                                                                   */
    *numScaledBand = bandsPrintedTotal;
    return (EPS_ERR_CODE)EPSMP_SCALE_ERR_NONE;
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpReleaseScale()                                                 */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* ----         void                                                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPSMP_SCALE_ERR_NONE                                                            */
/*      EPSMP_SCALE_ERR_NOT_INITIALIZED                                                 */
/*                                                                                      */
/* Description:                                                                         */
/*      Release the scaling data.                                                       */
/*                                                                                      */
/*******************************************|********************************************/
EPS_ERR_CODE    epsmpReleaseScale (

        void

){

/*** Declare Variable Local to Routine                                                  */

/*** Initialize Local Variables                                                         */

/*** Clear data buffer                                                                  */
    if (scaleData->buf != NULL)
        free(scaleData->buf);

/*** Has Scaling been Initialized                                                       */
    if (scaleState == -1)
        return((EPS_ERR_CODE)EPSMP_SCALE_ERR_NOT_INITIALIZED);

/*** Initialize global variables                                                        */
    scaleState = -1;
    InitScaleDataParam();

/*** Return to Caller                                                                   */
    return((EPS_ERR_CODE)EPSMP_SCALE_ERR_NONE);

}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%                                             %%%%%%%%%%%%%%%%%%%%%*/
/*--------------------             Internal Processing             ---------------------*/
/*%%%%%%%%%%%%%%%%%%%%                                             %%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   InitScaleDataParam()                                                */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* -            -                   -                                                   */
/* Return value:                                                                        */
/*      None                                                                            */
/*                                                                                      */
/* Description:                                                                         */
/*      Initialize scaling data parameters.                                             */
/*                                                                                      */
/*******************************************|********************************************/
static void InitScaleDataParam (

        void

){
    
    scaleData->factorWhole      = 0;
    scaleData->factorFract      = 0;
    scaleData->yAcc             = 0;
    scaleData->bandWidth        = 0;
    scaleData->newBandWidth     = 0;
    scaleData->bpp              = 0;
    scaleData->xOffset          = 0;
    scaleData->printableWidth   = 0;
    scaleData->printableHeight  = 0;
    scaleData->yCnt             = 0;
    scaleData->buf              = NULL;

}

/*_________________________________  epsmp-scale.c   ___________________________________*/
  
/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*       1         2         3         4         5         6         7         8        */
/*******************************************|********************************************/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/***** End of File *** End of File *** End of File *** End of File *** End of File ******/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
