/*__________________________________  epsmp-image.c   __________________________________*/

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
/*                                 Epson Image Functions                                */
/*                                                                                      */
/*                                    Function Calls                                    */
/*                              --------------------------                              */
/*          EPS_ERR_CODE epsmpInitImage        (imageFileName, bandHeight, supportFunc, */
/*                                              reverse, imageBand );                   */
/*          EPS_ERR_CODE epsmpGetBandData      (imageBand);                             */
/*          EPS_ERR_CODE epsmpReleaseImage     (void     );                             */
/*          EPS_ERR_CODE epsmpRGBtoGrayData    (imageBand);                             */
/*          EPS_ERR_CODE epsmpRGBtoGrayPalette (imageBand);                             */
/*          EPS_ERR_CODE epsmpRotateImage      (inFileName, outFileName             );  */
/*                                                                                      */
/*******************************************|********************************************/

/*------------------------------------  Includes   -------------------------------------*/
/*******************************************|********************************************/
#include <string.h>
#include <ctype.h>

#include "epson-escpr-def.h"
#include "epson-escpr-err.h"
#include "epson-escpr-media.h"

#include "epsmp-image.h"
#include "epsmp-bmp.h"
#include "epsmp-jpg.h"
#include "epsmp-rawimg.h"

/*------------------------------------  Definition   -----------------------------------*/
/*******************************************|********************************************/
#define MAX_FILENAME        256

/*------------------------------  Local Global Variables  ------------------------------*/
/*******************************************|********************************************/
    /*** Image Functions Definition                                                     */
    /*** -------------------------------------------------------------------------------*/
static EPSMP_IMAGE_FNS    epsmpImageFns[EPSMP_LAST_IMAGE_TYPE] =
{
    /*** BMP File Format                                                                */
    {
        epsmpBmpInit,
        epsmpBmpGetWidth,
        epsmpBmpGetHeight,
        epsmpBmpGetBandHeight,
        epsmpBmpGetBandWidth,
        epsmpBmpGetBandSize,
        epsmpBmpGetBytesPerPixel,
        epsmpBmpGetPaletteSize,
        epsmpBmpGetPaletteData,
        epsmpBmpGetBandData,
        epsmpBmpRelease,
		epsmpBmpReset
    },
    /*** Jpeg File Format                                                               */
    {
#ifdef GCOMSW_JPEG_SUPPORT
        epsmpJpgInit,
        epsmpJpgGetWidth,
        epsmpJpgGetHeight,
        epsmpJpgGetBandHeight,
        epsmpJpgGetBandWidth,
        epsmpJpgGetBandSize,
        epsmpJpgGetBytesPerPixel,
        epsmpJpgGetPaletteSize,
        epsmpJpgGetPaletteData,
        epsmpJpgGetBandData,
        epsmpJpgRelease,
		epsmpJpgReset
#else
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
		NULL
#endif /* def GCOMSW_JPEG_SUPPORT */
    },

    /*** File raw data                                                                  */
    {
#ifdef GCOMSW_RAWFILE_SUPPORT
        epsmpRawInit,
        epsmpRawGetWidth,
        epsmpRawGetHeight,
        epsmpRawGetBandHeight,
        epsmpRawGetBandWidth,
        epsmpRawGetBandSize,
        epsmpRawGetBytesPerPixel,
        epsmpRawGetPaletteSize,
        epsmpRawGetPaletteData,
        epsmpRawGetBandData,
        epsmpRawRelease,
		epsmpRawReset
#else
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
		NULL
#endif /* def GCOMSW_RAWFILE_SUPPORT */
	}
};

    /*** Image Functions                                                                */
    /*** -------------------------------------------------------------------------------*/
static EPSMP_IMAGE_FNS    imageFns;

    /*** Image File Pointer                                                             */
    /*** -------------------------------------------------------------------------------*/
static FILE*            imageFP = NULL;

    /*** Image Function State                                                           */
    /*** -------------------------------------------------------------------------------*/
static EPS_INT32        heapImageMemory = FALSE;    /* State of imageBand function routines */

    /*** Image Function State                                                           */
    /*** -------------------------------------------------------------------------------*/
static EPS_INT32        readTopLine = 0;

/*---------------------------  Data Structure Declarations   ---------------------------*/
/*******************************************|********************************************/

/*--------------------------------  Local Functions   ----------------------------------*/
/*******************************************|********************************************/

/*------------------------------------  Debug Dump  ------------------------------------*/
/*******************************************|********************************************/
    /*** Dump                                                                           */
    /*** -------------------------------------------------------------------------------*/
#define DEBUGDUMP               0               /* ON: 1    OFF: 0                      */

#if DEBUGDUMP
#define DebugPrint(a)   printf a
#else
#define DebugPrint(a)
#endif


/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpSelectImageType()                                              */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:            Type:               Description:                                    */
/* imageFileName    const EPS_INT8*     Image File Name                                 */
/* supportFunc      const EPS_UINT32    Printer suppoeted function                      */
/*                                                                                      */
/* Return value:                                                                        */
/*      -1                              can't select                                    */
/*      EPSMP_BMP                       btimap image                                    */
/*      EPSMP_JPG                       jpeg (convert to RGB) image                     */
/*      EPSMP_JPGRAW                    jpeg Raw file data                              */
/*      EPSMP_PRN                       print command file data                         */
/*                                                                                      */
/* Description:                                                                         */
/*      Select the treated data type .                                                  */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32    epsmpSelectImageType (

        const EPS_INT8*     imageFileName,
		EPS_UINT32			supportFunc

){
    EPS_INT32   imageType = -1;
    EPS_INT32   idx;
    EPS_INT8    upperName[MAX_FILENAME];

    for (idx=0; idx < MAX_FILENAME; idx++) {
		if (imageFileName[idx] == '\0'){
			upperName[idx] = '\0';
            break;
		}
        upperName[idx] = (EPS_INT8)toupper(imageFileName[idx]);
    }

	if ((strstr(upperName, ".JPG" ) != NULL) ||
		(strstr(upperName, ".jpg" ) != NULL) ||
		(strstr(upperName, ".JPEG") != NULL) ||
		(strstr(upperName, ".jpeg") != NULL)    ) {

		if(EPS_SPF_JPGPRINT & supportFunc){
			imageType = EPSMP_JPGRAW;
		} else{
			imageType = EPSMP_JPG;
		}
	} else if ((strstr(upperName,".BMP") != NULL) ||
			   (strstr(upperName,".bmp") != NULL)    ) {
		imageType = EPSMP_BMP;
	} else if ((strstr(upperName,".PRN") != NULL) ||
			   (strstr(upperName,".prn") != NULL)    ) {
		imageType = EPSMP_PRN;
	}

	return imageType;
}


/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpInitImage()                                                    */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:            Type:               Description:                                    */
/* imageFileName    const EPS_INT8*     Image File Name                                 */
/* bandHeight       const EPS_INT32     Band Height                                     */
/* supportFunc      const EPS_UINT32    Printer suppoeted function                      */
/* reverse          EPS_INT32           get band reverse                                */
/* imageBand        EPSMP_IMAGE_BAND*   Image Band Information                          */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPSMP_IMAGE_ERR_NONE                    - Success                               */
/*      EPSMP_IMAGE_ERR_INVALID_ARG                                                     */
/*      EPSMP_IMAGE_ERR_FILE_NOT_OPEN                                                   */
/*      EPSMP_IMAGE_ERR_FATAL                                                           */
/*      EPSMP_IMAGE_ERR_INIT_IMAGE                                                      */
/*      EPSMP_IMAGE_ERR_JPEG_NOT_SUPPORTED                                              */
/*                                                                                      */
/* Description:                                                                         */
/*      Initialize image band data.                                                     */
/*                                                                                      */
/*******************************************|********************************************/
EPS_ERR_CODE    epsmpInitImage (

        const EPS_INT8*     imageFileName,
        const EPS_INT32     bandHeight,
		EPS_UINT32			supportFunc,
		EPS_BOOL			reverse,
        EPSMP_IMAGE_BAND*   imageBand

){

/*** Declare Variable Local to Routine                                                  */
    EPS_INT32   retStatus;
    EPS_INT32   imageType;

/*** Are input parameter valid                                                          */
    if ((imageFileName == NULL) ||
        (bandHeight    <= 0   ) ||
        (imageBand     == NULL)   )
        return (EPS_ERR_CODE)EPSMP_IMAGE_ERR_INVALID_ARG;

/*** Initialize Local Variables                                                         */
    retStatus       = EPSMP_IMAGE_ERR_NONE;
    heapImageMemory = FALSE;
    imageType       = -1;

/*** Initialize Local Global Variables                                                  */
    readTopLine = 0;

/*** Find out what type of imageBand we're working with (24-bit BMP, 8-bit BMP, JPEG, etc.) */
	imageType = epsmpSelectImageType(imageFileName, supportFunc);
    
    if (imageType == -1)
        return (EPS_ERR_CODE)EPSMP_IMAGE_ERR_INVALID_ARG;

/*** Validate jpeg support                                                              */
#ifndef GCOMSW_JPEG_SUPPORT
	if (imageType == EPSMP_JPG){
        return (EPS_ERR_CODE)EPSMP_IMAGE_ERR_IMAGE_NOT_SUPPORTED;
	}
#endif
#ifndef GCOMSW_RAWFILE_SUPPORT	
	if (imageType == EPSMP_JPGRAW || imageType == EPSMP_PRN) {
        return (EPS_ERR_CODE)EPSMP_IMAGE_ERR_IMAGE_NOT_SUPPORTED;
	}
#endif

/*** Initialize Image Functions                                                         */
	if(EPSMP_PRN != imageType){
		imageFns = epsmpImageFns[imageType];
	} else{
		imageFns = epsmpImageFns[EPSMP_JPGRAW];
	}

/*** Open Image File                                                                    */
    imageFP = fopen(imageFileName,"rb");
    if (imageFP == NULL)
        return (EPS_ERR_CODE)EPSMP_IMAGE_ERR_FILE_NOT_OPEN;

/*** Initialize Image Data                                                              */
    retStatus = imageFns.Init(imageFP, bandHeight, reverse);
    if (!(retStatus == EPSMP_BMP_ERR_NONE 
		|| retStatus == EPSMP_JPG_ERR_NONE
		|| retStatus == EPSMP_RAW_ERR_NONE))
        return (EPS_ERR_CODE)EPSMP_IMAGE_ERR_INIT_IMAGE;

    heapImageMemory = TRUE;

/*** Set Image Attribute Data                                                           */
    imageBand->imageType   = imageType;
    imageBand->imageWidth  = imageFns.GetWidth();
    imageBand->imageHeight = imageFns.GetHeight();
    imageBand->bandWidth   = imageFns.GetBandWidth();
    imageBand->bandHeight  = imageFns.GetBandHeight();
    imageBand->bpl         = imageFns.GetBytesPerLine();
    imageBand->bpp         = imageFns.GetBytesPerPixel();
    imageBand->paletteSize = 0;
    imageBand->paletteData = NULL;

    if (imageType == EPSMP_BMP) {
        if (imageBand->bpp == 1) { /* Mapped colors                                     */
            imageBand->paletteSize = imageFns.GetPaletteSize();
            imageBand->paletteData = imageFns.GetPaletteData();
            if (!imageBand->paletteSize || !imageBand->paletteData) 
                return (EPS_ERR_CODE)EPSMP_IMAGE_ERR_FATAL;
        } else if (imageBand->bpp != 3) {
            return (EPS_ERR_CODE)EPSMP_IMAGE_ERR_FATAL;
        }
    }

/*** Return to Caller                                                                   */
    return (EPS_ERR_CODE)EPSMP_IMAGE_ERR_NONE;
}


/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpGetBandData()                                                  */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* imageBand    EPSMP_IMAGE_BAND*   Image Band Information                              */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPSMP_IMAGE_ERR_NONE            - Success                                       */
/*      EPSMP_IMAGE_ERR_FATAL                                                           */
/*                                                                                      */
/* Description:                                                                         */
/*      Get image band data.                                                            */
/*                                                                                      */
/*******************************************|********************************************/
EPS_ERR_CODE    epsmpGetBandData (

        EPSMP_IMAGE_BAND*   imageBand

){
/*** Declare Variable Local to Routine                                                  */
    EPS_UINT8* bandData;
    EPS_INT32 readLine;

/*** Get the band data                                                                  */
    readLine = imageFns.GetBandData(&bandData);
    
    DebugPrint(("    Read Line = %d\n", readLine));

/*** Check the result of GetBandData()                                                  */
    if (readLine < 0) {
        return (EPS_ERR_CODE)EPSMP_IMAGE_ERR_FATAL;
    } else if (readLine == 0) {
        imageBand->bandData    = NULL;
        imageBand->rect.top    = readTopLine;
        imageBand->rect.left   = 0;
        imageBand->rect.bottom = imageBand->rect.top;
        imageBand->rect.right  = imageBand->bandWidth;
        imageBand->bandWidth   = imageFns.GetBandWidth();
        imageBand->bandHeight  = 0;
        imageBand->bpl         = imageFns.GetBytesPerLine();
        imageBand->paletteSize = imageFns.GetPaletteSize();
        imageBand->paletteData = imageFns.GetPaletteData();

        return (EPS_ERR_CODE)EPSMP_IMAGE_ERR_NONE;
    }
    
/*** Update the band information                                                        */
    imageBand->bandData    = bandData;
    imageBand->rect.top    = readTopLine;
    imageBand->rect.left   = 0;
    imageBand->rect.bottom = imageBand->rect.top + readLine;
    imageBand->rect.right  = imageFns.GetBandWidth();
    imageBand->bandWidth   = imageFns.GetBandWidth();
    imageBand->bandHeight  = readLine;
    imageBand->bpl         = imageFns.GetBytesPerLine();
    imageBand->paletteSize = imageFns.GetPaletteSize();
    imageBand->paletteData = imageFns.GetPaletteData();

/*** Increase the band top position                                                     */
    readTopLine += readLine;

/*** Return to Caller                                                                   */
    return (EPS_ERR_CODE)EPSMP_IMAGE_ERR_NONE;

}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpReleaseImage()                                                 */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* -            -                   -                                                   */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPSMP_IMAGE_ERR_NONE                - Success                                   */
/*      EPSMP_IMAGE_ERR_RELEASE_IMAGE                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Rlease image data.                                                              */
/*                                                                                      */
/*******************************************|********************************************/
EPS_ERR_CODE    epsmpReleaseImage (

        void

){
/*** Declare Variable Local to Routine                                                  */
    EPS_INT32   retStatus;

/*** Initialize Local Global Variables                                                  */
    readTopLine = 0;
    
/*** Check Image Initialize                                                             */
    if (heapImageMemory == FALSE)
        return (EPS_ERR_CODE)EPSMP_IMAGE_ERR_NONE;

/*** Release Image                                                                      */
    retStatus = imageFns.Release();
    
    if (!(retStatus == EPSMP_BMP_ERR_NONE 
		|| retStatus == EPSMP_JPG_ERR_NONE
		|| retStatus == EPSMP_RAW_ERR_NONE))
        return (EPS_ERR_CODE)EPSMP_IMAGE_ERR_RELEASE_IMAGE;

/*** Close Image File                                                                    */
	if (fclose(imageFP) != 0)
	{
		printf("Error, couldn`t close imageFP\n");
		return (EPS_ERR_CODE)EPSMP_IMAGE_ERR_CLOSE_IMAGE;
	}
	imageFP = NULL;

    return (EPS_ERR_CODE)EPSMP_IMAGE_ERR_NONE;

}


EPS_INT32    epsmpResetImage (

        EPSMP_IMAGE_BAND*   imageBand,
		EPS_BOOL			revrese

){
/*** Declare Variable Local to Routine                                                  */
    EPS_INT32   retStatus;

/*** Reset Image                                                                        */
	retStatus = imageFns.Reset(revrese);

	if (!(retStatus == EPSMP_BMP_ERR_NONE 
		|| retStatus == EPSMP_JPG_ERR_NONE
		|| retStatus == EPSMP_RAW_ERR_NONE)){
     
		return (EPS_ERR_CODE)EPSMP_IMAGE_ERR_INIT_IMAGE;
	}

    imageBand->imageWidth  = imageFns.GetWidth();
    imageBand->imageHeight = imageFns.GetHeight();
    imageBand->bandWidth   = imageFns.GetBandWidth();
    imageBand->bandHeight  = imageFns.GetBandHeight();
    imageBand->bpl         = imageFns.GetBytesPerLine();
    imageBand->bpp         = imageFns.GetBytesPerPixel();
	imageBand->rect.top = imageBand->rect.bottom
		= imageBand->rect.left = imageBand->rect.right = 0;

	readTopLine = 0;

    return (EPS_ERR_CODE)EPSMP_IMAGE_ERR_NONE;
}


/*******************************************|********************************************/
/*                                                                                      */
/* Function name: epsmpRGBtoGrayData()                                                  */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* imageBand    EPSMP_IMAGE_BAND*   Image to convert                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPSMP_IMAGE_ERR_NONE                - Success                                   */
/*      EPSMP_IMAGE_ERR_INVALID_ARG                                                     */
/*                                                                                      */
/* Description:                                                                         */
/*      Converts Image Band Data from 24 bit RGB to 24 Bit Grayscale.                   */
/*                                                                                      */
/*******************************************|********************************************/
EPS_ERR_CODE    epsmpRGBtoGrayData (

        EPSMP_IMAGE_BAND*   imageBand

){
/*** Declare Variable Local to Routine                                                  */
    EPS_UINT8* tempData;
    EPS_UINT8  tempGray;
    EPS_INT32 idx, jdx;

/*** Convert 24-bit RGB ImageBand to 24-bit Gray Scale ImageBand                        */
    if (imageBand->bpp == 3) {
        for (idx=0; idx < imageBand->bandHeight; idx++) {
            tempData = imageBand->bandData + idx * imageBand->bpl;
            for (jdx=0; jdx < (imageBand->bandWidth * 3); jdx+=3) {
                tempGray = (EPS_UINT8)(( 30 * tempData[jdx  ] +
                                         59 * tempData[jdx+1] +
                                         11 * tempData[jdx+2]   ) / 100);
                tempData[jdx  ] = tempGray;
                tempData[jdx+1] = tempGray;
                tempData[jdx+2] = tempGray;
            }
        }
    } else{
        return (EPS_ERR_CODE)EPSMP_IMAGE_ERR_INVALID_ARG;
    }
    
    return (EPS_ERR_CODE)EPSMP_IMAGE_ERR_NONE;
    
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name: epsmpRGBtoGrayPalette()                                               */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* imageBand    EPSMP_IMAGE_BAND*   Image to convert                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPSMP_IMAGE_ERR_NONE                - Success                                   */
/*      EPSMP_IMAGE_ERR_INVALID_ARG                                                     */
/*                                                                                      */
/* Description:                                                                         */
/*      Converts Palette Data from RGB to Grayscale.                                    */
/*                                                                                      */
/*******************************************|********************************************/
EPS_ERR_CODE    epsmpRGBtoGrayPalette (

        EPSMP_IMAGE_BAND*   imageBand

){
/*** Declare Variable Local to Routine                                                  */
    EPS_UINT8* tempData;
    EPS_UINT8  tempGray;
    EPS_INT32 idx;

/*** Convert RGB Pallete Data to 24-BIT Gray Pallete Data                               */
    if (imageBand->bpp == 1) {
        tempData = imageBand->paletteData;
        for (idx=0; idx < imageBand->paletteSize; idx+=3) {
            tempGray = (EPS_UINT8)(( 30 * tempData[idx  ] +
                                     59 * tempData[idx+1] +
                                     11 * tempData[idx+2]   ) / 100);
            tempData[idx  ] = tempGray;
            tempData[idx+1] = tempGray;
            tempData[idx+2] = tempGray;
        }
    } else{
        return (EPS_ERR_CODE)EPSMP_IMAGE_ERR_INVALID_ARG;
    }
    
    return (EPS_ERR_CODE)EPSMP_IMAGE_ERR_NONE;

}


/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpPrepareImage()                                                 */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:            Type:               Description:                                    */
/* imageFileName    const EPS_INT8*     Image File Name                                 */
/* outFileName      const EPS_INT8*     Output File Name                                */
/* supportFunc      EPS_UINT32          Ability of target printer                       */
/* nSizeID          EPS_UINT32          Media size ID                                   */
/* bRotation        EPS_BOOL            Use rotation flag                               */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPSMP_IMAGE_ERR_NONE                    - Success                               */
/*      EPSMP_IMAGE_ERR_INVALID_ARG                                                     */
/*      EPSMP_IMAGE_ERR_FILE_NOT_OPEN                                                   */
/*      EPSMP_IMAGE_ERR_FATAL                                                           */
/*      EPSMP_IMAGE_ERR_INIT_IMAGE                                                      */
/*      EPSMP_IMAGE_ERR_JPEG_NOT_SUPPORTED                                              */
/*                                                                                      */
/* Description:                                                                         */
/*      Image Pre Process. Resize, Rotation.                                            */
/*      Resize : Height & Width reduces to 9600 picxel or less.                         */
/*             : Jpeg file size reduces to possible.                                    */
/*                                                                                      */
/*******************************************|********************************************/
EPS_ERR_CODE    epsmpPrepareImage (

        const EPS_INT8*     inFileName,
		char*               outFileName,
		EPS_UINT32			supportFunc,
		EPS_UINT32			nJpegMaxSize,
		EPS_UINT32			printWidth, 
		EPS_UINT32			printHeight, 
		EPS_BOOL			bRotation

){

/*** Declare Variable Local to Routine                                                  */
    EPS_ERR_CODE    retStatus = EPSMP_IMAGE_ERR_NONE;
    EPS_INT32	    imageType = -1;
#ifdef GCOMSW_JPEG_SUPPORT
	EPS_UINT32	    jpgFileSize, jpgHeight, jpgWidth;
	EPS_INT32       jpgFractTmp, jpgFractVal, jpgFraction;
#endif

	imageType = epsmpSelectImageType(inFileName, supportFunc);
	if( imageType == -1 ){
        return (EPS_ERR_CODE)EPSMP_IMAGE_ERR_INVALID_ARG;
	}

	if(EPSMP_JPGRAW == imageType){
#ifdef GCOMSW_JPEG_SUPPORT
		/* jpeg size reducation, if need */
		retStatus = epsmpJpgGetSize(inFileName, &jpgFileSize, &jpgHeight, &jpgWidth);
		if( EPSMP_IMAGE_ERR_NONE == retStatus ){

			jpgFraction = 1;
			if(EPS_JPEG_PIXEL_MAX < jpgHeight || EPS_JPEG_PIXEL_MAX < jpgWidth){
				strcpy(outFileName, "temp.jpg");
				/* select fraction */
				jpgFractTmp = ((jpgHeight > jpgWidth)?jpgHeight:jpgWidth) / EPS_JPEG_PIXEL_MAX;
				if(jpgFractTmp >= 1){
					jpgFractVal = 0;
					do{
						jpgFraction = 1 << ++jpgFractVal;
					} while(jpgFractTmp >= jpgFraction);
				}

				if(nJpegMaxSize < jpgFileSize){
					/* By FileSize & Fraction */
					retStatus = epsmpJpgReductionSize(inFileName, outFileName, nJpegMaxSize, jpgFraction);
				} else{
					/* By Fraction */
					retStatus = epsmpJpgReduction(inFileName, outFileName, 75, jpgFraction); /* 75 = IJG Recommendation */
				}
			} else if(nJpegMaxSize < jpgFileSize) {
					/* By FileSize */
				strcpy(outFileName, "temp.jpg");
				retStatus = epsmpJpgReductionSize(inFileName, outFileName, nJpegMaxSize, 1);
			} else{
				strcpy(outFileName, inFileName);
			}
		}
#else
		strcpy(outFileName, inFileName);
        retStatus = EPSMP_IMAGE_ERR_NONE; /* Jpeg Data transmits without resizing. */
#endif

	} else{
		/* rotation, if need */
		if(bRotation){
			retStatus = epsmpRotateImage(printWidth, printHeight, inFileName, outFileName);
		} else{
			strcpy(outFileName, inFileName);
		}
	}

	return retStatus;
}


/*******************************************|********************************************/
/*                                                                                      */
/* Function name: epsmpRotateImage()                                                    */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* inFileName   const char*         Original Image                                      */
/* imageBand    EPSMP_IMAGE_BAND*   Result Image                                        */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPSMP_IMAGE_ERR_NONE                - Success                                   */
/*      EPSMP_IMAGE_ERR_INVALID_ARG                                                     */
/*                                                                                      */
/* Description:                                                                         */
/*      Rotate Image.                                                                   */
/*                                                                                      */
/*******************************************|********************************************/
EPS_ERR_CODE epsmpRotateImage (EPS_UINT32 printWidth, EPS_UINT32 printHeight, const char* inFileName, char* outFileName)
{
	EPSMP_IMAGE_BAND    imageBand;                          
	EPS_ERR_CODE		retStatus;
	EPS_INT32           nImgAsp, nPaperAsp;
	
	retStatus = epsmpInitImage(inFileName, 64, EPS_SPF_RGBPRINT, FALSE, &imageBand);
	if (retStatus != EPSMP_IMAGE_ERR_NONE){
/*		out_printf("ERROR: epsmpInitImage in appRotateImage() (%d)\n", retValue);*/
		return retStatus;
	}

	nImgAsp = imageBand.imageWidth / imageBand.imageHeight;
	nPaperAsp = printWidth / printHeight;
	if( (nImgAsp >= 1 && nPaperAsp < 1)
		|| (nImgAsp < 1 && nPaperAsp >= 1) ){
		printf("Image width is greater than image height.  Rotating image.\n");
		switch( epsmpIsImageFile(inFileName) ){
			case EPSMP_JPG:
#ifdef GCOMSW_JPEG_SUPPORT
				strcpy(outFileName, "temp.jpg");
				retStatus = epsmpJpgRotate(inFileName, outFileName);
#else
				retStatus = EPSMP_IMAGE_ERR_IMAGE_NOT_SUPPORTED;
#endif
				break;
			case EPSMP_BMP:
				strcpy(outFileName, "temp.bmp");
				retStatus = epsmpBmpRotate(inFileName, outFileName);
				break;

			default:
				strcpy(outFileName, inFileName);
				break;
		}
	} else{
		strcpy(outFileName, inFileName);
	}

	epsmpReleaseImage();

	return retStatus;
}


EPS_INT32 epsmpIsImageFile (const char* fileName)
{
	char tmp[20];
	strcpy(tmp, fileName);
	char* pExt = strrchr(tmp, '.');
	if(pExt){
		if( 0 == strcmp(pExt, ".JPG")	||
			0 == strcmp(pExt, ".JPEG")	||
			0 == strcmp(pExt, ".jpg")	||
			0 == strcmp(pExt, ".jpeg")){
			
			return EPSMP_JPG;
		} else if(0 == strcmp(pExt, ".BMP")	||
				  0 == strcmp(pExt, ".bmp")){

			return EPSMP_BMP;
		} else{
			return EPSMP_LAST_IMAGE_TYPE;
		}

	} else{
		return EPSMP_LAST_IMAGE_TYPE;
	}
}


/*__________________________________  epsmp-image.c   __________________________________*/
  
/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*       1         2         3         4         5         6         7         8        */
/*******************************************|********************************************/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/***** End of File *** End of File *** End of File *** End of File *** End of File ******/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
