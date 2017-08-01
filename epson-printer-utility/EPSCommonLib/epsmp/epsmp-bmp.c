/*___________________________________  epsmp-bmp.c   ___________________________________*/

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
/*                              Epson Image Band Functions                              */
/*                                                                                      */
/*                                    Core Function Calls                               */
/*                                --------------------------                            */
/*                   EPS_INT32  epsmpBmpInit            (File, bandHeight, reverse);    */
/*                   EPS_INT32  epsmpBmpGetWidth        (void                   );      */
/*                   EPS_INT32  epsmpBmpGetHeight       (void                   );      */
/*                   EPS_INT32  epsmpBmpGetBytesPerLine (void                   );      */
/*                   EPS_INT32  epsmpBmpGetBytesPerPixel(void                   );      */
/*                   EPS_INT32  epsmpBmpGetBandSize     (void                   );      */
/*                   EPS_INT32  epsmpBmpGetBandData     (buf                    );      */
/*                   EPS_INT32  epsmpBmpGetBandWidth    (void                   );      */
/*                   EPS_INT32  epsmpBmpGetBandHeight   (void                   );      */
/*                   EPS_INT32  epsmpBmpGetPaletteSize  (void                   );      */
/*                   EPS_UINT8* epsmpBmpGetPaletteData  (void                   );      */
/*                   EPS_INT32  epsmpBmpRotate          (inFileName, outFileName);      */
/*                   EPS_INT32  epsmpBmpRelease         (void                   );      */
/*                                                                                      */
/*******************************************|********************************************/

/*------------------------------------  Includes   -------------------------------------*/
/*******************************************|********************************************/
#if defined(WIN32)		
#include <windows.h>				/* BITMAPFILEHEADER define */
#endif
#include "epson-escpr-mem.h"
#include "epsmp-image.h"            /* General image functions                          */
#include "epsmp-bmp.h"

/*------------------------------------  Definition   -----------------------------------*/
/*******************************************|********************************************/

/*------------------------------  Local Global Variables  ------------------------------*/
/*******************************************|********************************************/
    /*** File I/O and Indexing                                                          */
    /*** ------------------------------------------------------------------------------ */
static FILE*            bmpFP;
static EPS_INT32        thisCurrentLine = -1;

    /*** BMP File Header Data                                                           */
    /*** ------------------------------------------------------------------------------ */
static BITMAPFILEHEADER bmpFileHeader;
static BITMAPINFOHEADER bmpInfoHeader;

    /*** BMP Image Attributes                                                           */
    /*** ------------------------------------------------------------------------------ */
static EPS_UINT8*       retBandData;
static EPS_INT32        retBandHeight;

static EPS_INT32        thisRasterWidth;
static EPS_INT32        thisBPL;
static EPS_INT32        thisBPP;

static EPS_BOOL			bandReverse;

    /*** BMP Palette Data                                                               */
    /*** ------------------------------------------------------------------------------ */
static EPS_UINT8*       thisPaletteData;
static EPS_UINT8*       tmpPaletteData;
static EPS_UINT16       thisPaletteSize;

/*---------------------------  Data Structure Declarations   ---------------------------*/
/*******************************************|********************************************/

/*--------------------------------  Local Functions   ----------------------------------*/
/*******************************************|********************************************/
static EPS_INT32 ReadLittleEndian2(FILE*, EPS_UINT16*);
static EPS_INT32 ReadLittleEndian4(FILE*, EPS_UINT32*);

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpBmpInit()                                                      */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* fp           FILE*               Image File Pointer                                  */
/* bandHeight   EPS_INT32           Image Band Height                                   */
/* reverse      EPS_INT32           get band reverse                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPSMP_BMP_ERR_NONE                  - Success                                   */
/*      EPSMP_BMP_ERR_MEMORY_ALLOCATION                                                 */
/*      EPSMP_BMP_ERR_INVALID_FILE                                                      */
/*      EPSMP_BMP_ERR_NOT_A_BMP_FILE                                                    */
/*      EPSMP_BMP_ERR_UNSUPPORTED_BPP                                                   */
/*      EPSMP_BMP_ERR_COMPRESSION_ON                                                    */
/*                                                                                      */
/* Description:                                                                         */
/*      Initialize BMP file.                                                            */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32 epsmpBmpInit (

        FILE*       fp,
        EPS_INT32   bandHeight,
		EPS_BOOL	reverse

){

/*** Declare Variable Local to Routine                                                  */
EPS_INT32   retState;
EPS_UINT32   idx;                            /* General loop/index variable              */
EPS_UINT8   temp;

/*** Validate input parameters                                                          */
    if (!fp)
        return((EPS_INT32)EPSMP_BMP_ERR_INVALID_FILE);

/*** Initialize Global/Local Variables                                                  */
    retState        = EPSMP_BMP_ERR_NONE;
    bmpFP           = fp;
    thisPaletteData = NULL;
    tmpPaletteData  = NULL;
    thisPaletteSize = 0;
    retBandData     = NULL;
    retBandHeight   = bandHeight;
	bandReverse     = reverse;

/*** Read Header / Image Attribute data from BMP file                                   */
    if ((retState = ReadLittleEndian2(bmpFP,&bmpFileHeader.bfType         )) != EPSMP_BMP_ERR_NONE)
        return retState;
    if ((retState = ReadLittleEndian4(bmpFP,&bmpFileHeader.bfSize         )) != EPSMP_BMP_ERR_NONE)
        return retState;
    if ((retState = ReadLittleEndian2(bmpFP,&bmpFileHeader.bfReserved1    )) != EPSMP_BMP_ERR_NONE)
        return retState;
    if ((retState = ReadLittleEndian2(bmpFP,&bmpFileHeader.bfReserved2    )) != EPSMP_BMP_ERR_NONE)
        return retState;
    if ((retState = ReadLittleEndian4(bmpFP,&bmpFileHeader.bfOffBits      )) != EPSMP_BMP_ERR_NONE)
        return retState;
    if ((retState = ReadLittleEndian4(bmpFP,&bmpInfoHeader.biSize         )) != EPSMP_BMP_ERR_NONE)
        return retState;
    if ((retState = ReadLittleEndian4(bmpFP,&bmpInfoHeader.biWidth        )) != EPSMP_BMP_ERR_NONE)
        return retState;
    if ((retState = ReadLittleEndian4(bmpFP,&bmpInfoHeader.biHeight       )) != EPSMP_BMP_ERR_NONE)
        return retState;
    if ((retState = ReadLittleEndian2(bmpFP,&bmpInfoHeader.biPlanes       )) != EPSMP_BMP_ERR_NONE)
        return retState;
    if ((retState = ReadLittleEndian2(bmpFP,&bmpInfoHeader.biBitCount     )) != EPSMP_BMP_ERR_NONE)
        return retState;
    if ((retState = ReadLittleEndian4(bmpFP,&bmpInfoHeader.biCompression  )) != EPSMP_BMP_ERR_NONE)
        return retState;
    if ((retState = ReadLittleEndian4(bmpFP,&bmpInfoHeader.biSizeImage    )) != EPSMP_BMP_ERR_NONE)
        return retState;
    if ((retState = ReadLittleEndian4(bmpFP,&bmpInfoHeader.biXPelsPerMeter)) != EPSMP_BMP_ERR_NONE)
        return retState;
    if ((retState = ReadLittleEndian4(bmpFP,&bmpInfoHeader.biYPelsPerMeter)) != EPSMP_BMP_ERR_NONE)
        return retState;
    if ((retState = ReadLittleEndian4(bmpFP,&bmpInfoHeader.biClrUsed      )) != EPSMP_BMP_ERR_NONE)
        return retState;
    if ((retState = ReadLittleEndian4(bmpFP,&bmpInfoHeader.biClrImportant )) != EPSMP_BMP_ERR_NONE)
        return retState;

/*** Check Read Data                                                                    */
    if (bmpFileHeader.bfType != 0x4D42)
        return((EPS_INT32)EPSMP_BMP_ERR_NOT_A_BMP_FILE);

    if ((bmpInfoHeader.biBitCount != 24) &&
        (bmpInfoHeader.biBitCount !=  8)    )
        return((EPS_INT32)EPSMP_BMP_ERR_UNSUPPORTED_BPP);

    if (bmpInfoHeader.biCompression != 0) 
        return((EPS_INT32)EPSMP_BMP_ERR_COMPRESSION_ON);

/*** Initialize Data for 8-bit BMP                                                      */
    if (bmpInfoHeader.biBitCount == 8) {
        if (bmpInfoHeader.biClrUsed == 0)
            bmpInfoHeader.biClrUsed = 256; 

        /*** Allocate palette                                                           */
        thisPaletteSize = (EPS_UINT16)bmpInfoHeader.biClrUsed * 3;

//        thisPaletteData = malloc(thisPaletteSize);
        if (thisPaletteData == NULL)
            return((EPS_INT32)EPSMP_BMP_ERR_MEMORY_ALLOCATION);

//        tmpPaletteData = malloc(thisPaletteSize);
        if (tmpPaletteData == NULL)
            return((EPS_INT32)EPSMP_BMP_ERR_MEMORY_ALLOCATION);

        /*** Read Palette Data                                                          */
        for (idx = 0; idx < bmpInfoHeader.biClrUsed; idx++) {
            if (fread(thisPaletteData+idx*3+2, 1,1,bmpFP) < 1)    /* b                  */
                return((EPS_INT32)EPSMP_BMP_ERR_READING_DATA);
            if (fread(thisPaletteData+idx*3+1, 1,1,bmpFP) < 1)    /* g                  */
                return((EPS_INT32)EPSMP_BMP_ERR_READING_DATA);
            if (fread(thisPaletteData+idx*3+0, 1,1,bmpFP) < 1)    /* r                  */
                return((EPS_INT32)EPSMP_BMP_ERR_READING_DATA);
            if (fread(&temp                   ,1,1,bmpFP) < 1)    /* padding bit        */
                return((EPS_INT32)EPSMP_BMP_ERR_READING_DATA);
        }
   }

    thisBPL = (bmpInfoHeader.biWidth * bmpInfoHeader.biBitCount) / 8;
    if ((thisBPL % 4) != 0)
        thisBPL = ((thisBPL / 4) + 1) * 4;

    thisBPP = 3;
    if (bmpInfoHeader.biBitCount == 8) thisBPP = 1;

    /*** Allocate buffer for Band Data                                                  */
    thisRasterWidth = bmpInfoHeader.biWidth * thisBPP;
//    retBandData     = malloc(retBandHeight * thisRasterWidth);
    if (retBandData == NULL)
        return (EPS_INT32)EPSMP_BMP_ERR_MEMORY_ALLOCATION;

	thisCurrentLine = (FALSE == reverse)?0:bmpInfoHeader.biHeight;

/*** Return to Caller                                                                   */
    return((EPS_INT32)EPSMP_BMP_ERR_NONE);
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpBmpGetWidth()                                                  */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* ----         void                                                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      Image width in pixels           - Success                                       */
/*      EPSMP_BMP_ERR_NOT_INITIALIZED                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Return image width in pixels.                                                   */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   epsmpBmpGetWidth (

        void

){

/*** Declare Variable Local to Routine                                                  */

/*** Initialize Local Variables                                                         */

/*** Has BMP file been Initialized                                                      */
    if (thisCurrentLine == -1)
        return((EPS_INT32)EPSMP_BMP_ERR_NOT_INITIALIZED);

/*** Return to Caller                                                                   */
    return((EPS_INT32)bmpInfoHeader.biWidth);
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpBmpGetHeight()                                                 */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* ----         void                                                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      Image height in lines (pixels)      - Success                                   */
/*      EPSMP_BMP_ERR_NOT_INITIALIZED                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Return image height in lines (pixels)                                           */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   epsmpBmpGetHeight (

        void

) {

/*** Declare Variable Local to Routine                                                  */

/*** Initialize Local Variables                                                         */

/*** Has BMP file been Initialized                                                      */
    if (thisCurrentLine == -1)
        return((EPS_INT32)EPSMP_BMP_ERR_NOT_INITIALIZED);

/*** Return to Caller                                                                   */
    return((EPS_INT32)bmpInfoHeader.biHeight);
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpBmpGetBytesPerLine()                                           */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* ----         void                                                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      Image BPL in pixels             - Success                                       */
/*      EPSMP_BMP_ERR_NOT_INITIALIZED                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Return image BPL in pixels                                                      */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   epsmpBmpGetBytesPerLine (

        void

) {

/*** Declare Variable Local to Routine                                                  */

/*** Initialize Local Variables                                                         */

/*** Has BMP file been Initialized                                                      */
    if (thisCurrentLine == -1) return((EPS_INT32)EPSMP_BMP_ERR_NOT_INITIALIZED);

/*** Return to Caller                                                                   */
    return((EPS_INT32)thisBPL);
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpBmpGetBytesPerPixel()                                          */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* ----         void                                                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      Current Bytes Per Pixel         - Success                                       */
/*      EPSMP_BMP_ERR_NOT_INITIALIZED                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Return Bytes Per Pixel.                                                         */
/*                                                                                      */
/*******************************************|********************************************/
EPS_UINT8 epsmpBmpGetBytesPerPixel (

        void

) {

/*** Declare Variable Local to Routine                                                  */

/*** Initialize Local Variables                                                         */

/*** Return to Caller                                                                   */
    return((EPS_UINT8)thisBPP);
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpBmpGetBandSize()                                               */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* ----         void                                                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      Band size in bytes              - Success                                       */
/*      EPSMP_BMP_ERR_NOT_INITIALIZED                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Return band size in bytes                                                       */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   epsmpBmpGetBandSize (

        void

) {

/*** Declare Variable Local to Routine                                                  */

/*** Initialize Local Variables                                                         */

/*** Has BMP file been Initialized                                                      */
    if (thisCurrentLine == -1)
        return((EPS_INT32)EPSMP_BMP_ERR_NOT_INITIALIZED);

/*** Return to Caller                                                                   */
    return((EPS_INT32)epsmpBmpGetBandWidth() * thisBPP);
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name: epsmpBmpGetBandData()                                                 */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* dest         EPS_UINT8**         O: Read band data                                   */
/*                                                                                      */
/* Return value:                                                                        */
/*      Number of bands read            - Success                                       */
/*      EPSMP_BMP_ERR_NOT_INITIALIZED                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Read band data and return number of bands read.                                 */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   epsmpBmpGetBandData (

        EPS_UINT8**     dest

) {

/*** Declare Variable Local to Routine                                                  */
EPS_INT32   idx, jdx, kdx;
EPS_INT32   position;
EPS_UINT8   temp;
EPS_INT32   linesRead;
EPS_UINT8*  buf;

/*** Has BMP file been Initialized                                                      */
    if (thisCurrentLine == -1)
        return((EPS_INT32)EPSMP_BMP_ERR_NOT_INITIALIZED);

/*** Initialize Local Variables                                                         */
    linesRead = 0;
    buf       = retBandData;
    *dest     = retBandData;

/*** Read in "retBandHeight" number of image lines for this band                        */
	if( FALSE == bandReverse){
		for (idx = 0; (idx < retBandHeight) && (thisCurrentLine < (EPS_INT32)bmpInfoHeader.biHeight); idx++) {

			position = bmpFileHeader.bfOffBits + 
								thisBPL * (bmpInfoHeader.biHeight - (thisCurrentLine + 1));

			if (fseek(bmpFP, position, SEEK_SET) != 0) {
				return((EPS_INT32)EPSMP_BMP_ERR_READING_DATA);
			}

			if (fread(buf + idx * thisRasterWidth, 1, thisRasterWidth, bmpFP) < (unsigned)thisRasterWidth) {
				return((EPS_INT32)EPSMP_BMP_ERR_READING_DATA);
			}

			thisCurrentLine++;
			linesRead++;
		}
	
	} else{
		for (idx = 0; (idx < retBandHeight) && (thisCurrentLine > 0); idx++) {

			for (jdx = (bmpInfoHeader.biWidth - 1) * thisBPP, kdx = 0; 
				jdx >= 0; 
				jdx -= thisBPP, kdx += thisBPP)
			{
				position = bmpFileHeader.bfOffBits 
							+ thisBPL * (bmpInfoHeader.biHeight - thisCurrentLine)
							+ jdx;

				if (fseek(bmpFP, position, SEEK_SET) != 0) {
					return((EPS_INT32)EPSMP_BMP_ERR_READING_DATA);
				}

				if (fread(buf + idx * thisRasterWidth + kdx, 1, thisBPP, bmpFP) < (unsigned)thisBPP) {
					return((EPS_INT32)EPSMP_BMP_ERR_READING_DATA);
				}
			}

			thisCurrentLine--;
			linesRead++;
		}
	}

	/*** if RGB, swap bits around                                                       */
    if (thisBPP == 3) {
        for (idx = 0; idx < (thisRasterWidth * retBandHeight); idx += bmpInfoHeader.biBitCount/8) {
            temp       = buf[idx+2];
            buf[idx+2] = buf[idx];
            buf[idx]   = temp;
        }
    }

/*** Return to Caller                                                                   */
    return((EPS_INT32)linesRead);
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpBmpGetBandWidth()                                              */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* ----         void                                                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      Band width in lines (pixels)        - Success                                   */
/*      EPSMP_BMP_ERR_NOT_INITIALIZED                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Return band width in lines (pixels)                                             */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   epsmpBmpGetBandWidth (

        void

) {

/*** Declare Variable Local to Routine                                                  */

/*** Initialize Local Variables                                                         */

/*** Has BMP file been Initialized                                                      */
    if (thisCurrentLine == -1)
        return((EPS_INT32)EPSMP_BMP_ERR_NOT_INITIALIZED);

/*** Return to Caller                                                                   */
    return((EPS_INT32)epsmpBmpGetWidth());
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpBmpGetBandHeight()                                             */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* ----         void                                                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      Band height in lines (pixels)       - Success                                   */
/*      EPSMP_BMP_ERR_NOT_INITIALIZED                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Return band height in lines (pixels)                                            */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   epsmpBmpGetBandHeight (

        void

) {

/*** Declare Variable Local to Routine                                                  */

/*** Initialize Local Variables                                                         */

/*** Has BMP file been Initialized                                                      */
    if (thisCurrentLine == -1)
        return((EPS_INT32)EPSMP_BMP_ERR_NOT_INITIALIZED);

/*** Return to Caller                                                                   */
    return((EPS_INT32)retBandHeight);
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpBmpGetPaletteSize()                                            */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* ----         void                                                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      Palette size in bytes           - Success                                       */
/*      EPSMP_BMP_ERR_NOT_INITIALIZED                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Return palette size in bytes                                                    */
/*                                                                                      */
/*******************************************|********************************************/
EPS_UINT16   epsmpBmpGetPaletteSize (

        void

) {

/*** Declare Variable Local to Routine                                                  */

/*** Initialize Local Variables                                                         */

/*** Return to Caller                                                                   */
    return thisPaletteSize;
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpBmpGetPaletteData()                                            */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* ----         void                                                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      Pointer to palette data buf     - Success                                       */
/*      NULL                            - Failed                                        */
/*                                                                                      */
/* Description:                                                                         */
/*      Return the pointer to palette data buf.                                         */
/*                                                                                      */
/*******************************************|********************************************/
EPS_UINT8*  epsmpBmpGetPaletteData (

        void

) {

/*** Declare Variable Local to Routine                                                  */

/*** Initialize Local Variables                                                         */

/*** Has BMP file been Initialized                                                      */
    if (thisCurrentLine == -1)
        return((EPS_UINT8*)NULL);

/*** Return to Caller                                                                   */
    if (thisBPP == 1) {
        memcpy(tmpPaletteData, thisPaletteData, thisPaletteSize);
        return((EPS_UINT8*)tmpPaletteData);
    } else{
        return((EPS_UINT8*)NULL);
    }
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name    epsmpBmpRelease()                                                   */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* ----         void                                                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPSMP_BMP_ERR_NONE                  - Success                                   */
/*      EPSMP_BMP_ERR_NOT_INITIALIZED                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Release BMP Data.                                                               */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   epsmpBmpRelease (

        void

) {

/*** Declare Variable Local to Routine                                                  */

/*** Initialize Local Variables                                                         */

/*** Release buffers                                                                    */
	if (thisPaletteData != NULL){
        free(thisPaletteData);
		thisPaletteData = NULL;
	}

	if (tmpPaletteData != NULL){
        free(tmpPaletteData);
		tmpPaletteData = NULL;
	}

	if (retBandData != NULL){
        free(retBandData);
		retBandData = NULL;
	}

/*** Has BMP file been Initialized                                                      */
    if (thisCurrentLine == -1)
        return((EPS_INT32)EPSMP_BMP_ERR_NOT_INITIALIZED);

/*** Release Global Variables                                                           */
    bmpFP           = NULL;
    thisCurrentLine = -1;

    memset(&bmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
    memset(&bmpInfoHeader, 0, sizeof(BITMAPINFOHEADER));
    
    retBandHeight   = 0;

    thisRasterWidth = 0;
    thisBPL         = 0;
    thisBPP         = 0;

    thisPaletteSize = 0;

/*** Return to Caller                                                                   */
    return((EPS_INT32)EPSMP_BMP_ERR_NONE);
}


EPS_INT32   epsmpBmpReset (

        EPS_BOOL reverse

) {
	bandReverse = reverse;
   	thisCurrentLine = (FALSE == reverse)?0:bmpInfoHeader.biHeight;
    return((EPS_INT32)EPSMP_BMP_ERR_NONE);
}


/*******************************************|********************************************/
/*                                                                                      */
/* Function name    bmpRotate()                                                         */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* inFileName   const char*          Name of existing bmp file                          */
/* outFileName  const char*          Name of Conversion result file with bmp            */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPSMP_BMP_ERR_NONE                  - Success                                   */
/*      EPSMP_BMP_ERR_NOT_INITIALIZED                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Rotates the bmp with filename passed as a parameter 90 clockwise.               */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32 epsmpBmpRotate (const char* inFileName, const char* outFileName)
{
	FILE*	inputFile;
	FILE*	outputFile;

	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;

	EPS_UINT8*	inputFilePaletteData = NULL;
	EPS_INT32	inputFilePaletteSize;
	EPS_INT32	inputFileBPL;
	EPS_INT32	inputFileBPP;
	EPS_UINT8	temp;
	EPS_INT32	i;
	EPS_UINT32	j;
	EPS_UINT32  pos;
	EPS_UINT32	origImageOffset = 0;

	EPS_UINT8*  buf = NULL;
	EPS_UINT8	tempBuf[4];
	EPS_UINT32  outputFileBPL;
	EPS_UINT32  outputFileSize;
	EPS_UINT32	outputFileOffset = 0;

	EPS_ERR_CODE retState = EPSMP_BMP_ERR_NONE;

	/* Open the input file. */
	if ((inputFile = fopen(inFileName, "rb")) == NULL) {
		/*out_printf("Error opening inputFile.\n");*/
		return EPSMP_BMP_ERR_FILE_NOT_OPEN;
	}
	if ((outputFile = fopen(outFileName, "wb")) == NULL) {
		/*out_printf("Error opening outputFile.\n");*/
		return EPSMP_BMP_ERR_FILE_NOT_OPEN;
	}

	/* Parse header */
    if ((retState = ReadLittleEndian2(inputFile,&bmpFileHeader.bfType         )) != EPSMP_BMP_ERR_NONE)
        return retState;
    if ((retState = ReadLittleEndian4(inputFile,&bmpFileHeader.bfSize         )) != EPSMP_BMP_ERR_NONE)
        return retState;
    if ((retState = ReadLittleEndian2(inputFile,&bmpFileHeader.bfReserved1    )) != EPSMP_BMP_ERR_NONE)
        return retState;
    if ((retState = ReadLittleEndian2(inputFile,&bmpFileHeader.bfReserved2    )) != EPSMP_BMP_ERR_NONE)
        return retState;
    if ((retState = ReadLittleEndian4(inputFile,&bmpFileHeader.bfOffBits      )) != EPSMP_BMP_ERR_NONE)
        return retState;
    if ((retState = ReadLittleEndian4(inputFile,&bmpInfoHeader.biSize         )) != EPSMP_BMP_ERR_NONE)
        return retState;
    if ((retState = ReadLittleEndian4(inputFile,&bmpInfoHeader.biWidth        )) != EPSMP_BMP_ERR_NONE)
        return retState;
    if ((retState = ReadLittleEndian4(inputFile,&bmpInfoHeader.biHeight       )) != EPSMP_BMP_ERR_NONE)
        return retState;
    if ((retState = ReadLittleEndian2(inputFile,&bmpInfoHeader.biPlanes       )) != EPSMP_BMP_ERR_NONE)
        return retState;
    if ((retState = ReadLittleEndian2(inputFile,&bmpInfoHeader.biBitCount     )) != EPSMP_BMP_ERR_NONE)
        return retState;
    if ((retState = ReadLittleEndian4(inputFile,&bmpInfoHeader.biCompression  )) != EPSMP_BMP_ERR_NONE)
        return retState;
    if ((retState = ReadLittleEndian4(inputFile,&bmpInfoHeader.biSizeImage    )) != EPSMP_BMP_ERR_NONE)
        return retState;
    if ((retState = ReadLittleEndian4(inputFile,&bmpInfoHeader.biXPelsPerMeter)) != EPSMP_BMP_ERR_NONE)
        return retState;
    if ((retState = ReadLittleEndian4(inputFile,&bmpInfoHeader.biYPelsPerMeter)) != EPSMP_BMP_ERR_NONE)
        return retState;
    if ((retState = ReadLittleEndian4(inputFile,&bmpInfoHeader.biClrUsed      )) != EPSMP_BMP_ERR_NONE)
        return retState;
    if ((retState = ReadLittleEndian4(inputFile,&bmpInfoHeader.biClrImportant )) != EPSMP_BMP_ERR_NONE)
        return retState;

	/*** Check Read Data                                                                    */
    if (bmpFileHeader.bfType != 0x4D42)
        return((EPS_INT32)EPSMP_BMP_ERR_NOT_A_BMP_FILE);

    if ((bmpInfoHeader.biBitCount != 24) &&
        (bmpInfoHeader.biBitCount !=  8)    )
        return((EPS_INT32)EPSMP_BMP_ERR_UNSUPPORTED_BPP);

    if (bmpInfoHeader.biCompression != 0) 
        return((EPS_INT32)EPSMP_BMP_ERR_COMPRESSION_ON);

	    if (bmpInfoHeader.biBitCount == 8) {
        if (bmpInfoHeader.biClrUsed == 0)
            bmpInfoHeader.biClrUsed = 256; 

        /*** Allocate palette                                                           */
        inputFilePaletteSize = bmpInfoHeader.biClrUsed * 3;

//        inputFilePaletteData = malloc(inputFilePaletteSize);
        if (inputFilePaletteData == NULL)
            return((EPS_INT32)EPSMP_BMP_ERR_MEMORY_ALLOCATION);

        /*** Read Palette Data                                                          */
        for (i = 0; i < (EPS_INT32)bmpInfoHeader.biClrUsed; i++) {
			if (fread(inputFilePaletteData+i*3+2, 1, 1, inputFile) < 1){/* b            */
                retState = EPSMP_BMP_ERR_READING_DATA;
				break;
			}
			if (fread(inputFilePaletteData+i*3+1, 1, 1, inputFile) < 1){/* g            */
                retState = EPSMP_BMP_ERR_READING_DATA;
				break;
			}
			if (fread(inputFilePaletteData+i*3+0, 1, 1, inputFile) < 1){/* r            */
                retState = EPSMP_BMP_ERR_READING_DATA;
				break;
			}
			if (fread(&temp                   ,1, 1, inputFile) < 1){  /* padding bit   */
                retState = EPSMP_BMP_ERR_READING_DATA;
				break;
			}
        }
		if(EPSMP_BMP_ERR_NONE != retState){
			free(inputFilePaletteData);
			return EPSMP_BMP_ERR_NONE;
		}
   }

    inputFileBPL = (bmpInfoHeader.biWidth * bmpInfoHeader.biBitCount) / 8; 
    while ((inputFileBPL % 4) != 0){
		inputFileBPL++;							 /* Account for padding */
		origImageOffset++;
	}

    inputFileBPP = 3;
	if (bmpInfoHeader.biBitCount == 8) {
		inputFileBPP = 1;
	}

	/*** Allocate buffer for Band Data                                                  */
//	buf = malloc ( Max(bmpInfoHeader.biHeight * inputFileBPL, bmpFileHeader.bfOffBits) );
	if (buf == NULL){
		if(NULL != inputFilePaletteData)free(inputFilePaletteData);
		return (EPS_INT32)EPSMP_BMP_ERR_MEMORY_ALLOCATION;
	}

	fseek(inputFile, 0, 0);

	/* Write header of outputFile */
	fread(buf, 1, bmpFileHeader.bfOffBits, inputFile);
	fwrite(buf, 1, bmpFileHeader.bfOffBits, outputFile);

	/* Change various info in header (switch height/width) */
	outputFileBPL = ((bmpInfoHeader.biHeight * bmpInfoHeader.biBitCount) / 8);
	while (outputFileBPL % 4 != 0){
		outputFileBPL++;
		outputFileOffset++;
	}
	outputFileSize = outputFileBPL * bmpInfoHeader.biWidth;
	pos = fseek(outputFile, 2, 0);
	pos = fseek(inputFile, 2, 0);
	fread(tempBuf, 1, 4, inputFile);
	fwrite(tempBuf, 1, 4, outputFile);

	/* Write width/height */
	fseek(outputFile, 18, 0);
	fseek(inputFile, 22, 0);
	fread(tempBuf, 1, 4, inputFile);
	fwrite (tempBuf, 1, 4, outputFile);

	fseek(inputFile, 18, 0);
	fread(tempBuf, 1, 4, inputFile);
	fwrite (tempBuf, 1, 4, outputFile);
	fseek(inputFile, bmpFileHeader.bfOffBits, 0);
	fseek(outputFile, bmpFileHeader.bfOffBits, 0);
	
	/* Reset buffer */
	memset(buf, 0, bmpInfoHeader.biHeight * inputFileBPL);

	/* Read data from inputFile into buf */
	fread(buf, 1, bmpInfoHeader.biHeight * inputFileBPL, inputFile);
	
	/* Write correctly sequenced data into OutputFile */
	for (i = 0; i < (EPS_INT32)bmpInfoHeader.biWidth; i++) {
		for (j = 0; j < (EPS_UINT32)bmpInfoHeader.biHeight; j++) {
			pos = (inputFileBPL * j) + inputFileBPL - (inputFileBPP*(i + 1)) - origImageOffset;
			fwrite(buf + pos, 1, inputFileBPP, outputFile);
		}

		for (j = 0; j < outputFileOffset; j++){
			fwrite("\0", 1, 1, outputFile);
		}
    }

	if(NULL != inputFilePaletteData)free(inputFilePaletteData);
	free(buf);

	fclose(inputFile);
	fclose(outputFile);

	return retState;
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
/* Function name:   ReadLittleEndian2()                                                 */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* fp           FILE*               I: BMP file pointer                                 */
/* value        EPS_UINT16*         O: Read value (2 bytes)                             */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPSMP_BMP_ERR_NONE                  - Success                                   */
/*      EPSMP_BMP_ERR_FILE_NOT_OPEN                                                     */
/*      EPSMP_BMP_ERR_INVALID_ARG                                                       */
/*      EPSMP_BMP_ERR_READING_DATA                                                      */
/*                                                                                      */
/* Description:                                                                         */
/*      Read value from BMP file                                                        */
/*                                                                                      */
/*******************************************|********************************************/
static EPS_INT32    ReadLittleEndian2 (

        FILE*       fp, 
        EPS_UINT16* value

){

/*** Declare Variable Local to Routine                                                  */
size_t    retStatus;                  /* Return status of internal calls          */
EPS_INT32       idx;
EPS_UINT8       bytes[2];

/*** Check File                                                                         */
    if (!fp   )
        return((EPS_INT32)EPSMP_BMP_ERR_FILE_NOT_OPEN);
    if (!value)
        return((EPS_INT32)EPSMP_BMP_ERR_INVALID_ARG);

/*** Initialize Local Variables                                                         */

/*** Read 2 bytes from file                                                             */
    for (idx = 0; idx < 2; idx++) {
        retStatus = fread(&bytes[idx],1,1,fp);
        if (retStatus < 1)
            return((EPS_INT32)EPSMP_BMP_ERR_READING_DATA);
    }

/*** Put bytes in return value                                                          */
    *value = (bytes[1] << 8) + bytes[0];

/*** Return to Caller                                                                   */
    return((EPS_INT32)EPSMP_BMP_ERR_NONE);

}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   ReadLittleEndian4()                                                 */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* fp           FILE*               I: BMP file pointer                                 */
/* value        EPS_UINT32*         O: Read value (4 bytes)                             */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPSMP_BMP_ERR_NONE                  - Success                                   */
/*      EPSMP_BMP_ERR_FILE_NOT_OPEN                                                     */
/*      EPSMP_BMP_ERR_INVALID_ARG                                                       */
/*      EPSMP_BMP_ERR_READING_DATA                                                      */
/*                                                                                      */
/* Description:                                                                         */
/*      Read value from BMP file                                                        */
/*                                                                                      */
/*******************************************|********************************************/
static EPS_INT32    ReadLittleEndian4 (

        FILE*       fp, 
        EPS_UINT32* value

){

/*** Declare Variable Local to Routine                                                  */
size_t    retStatus;                  /* Return status of internal calls          */
EPS_INT32       idx;
EPS_UINT8       bytes[4];

/*** Check File                                                                         */
    if (!fp   )
        return((EPS_INT32)EPSMP_BMP_ERR_FILE_NOT_OPEN);
    if (!value)
        return((EPS_INT32)EPSMP_BMP_ERR_INVALID_ARG);

/*** Initialize Local Variables                                                         */

/*** Read 4 bytes from file                                                             */
    for (idx = 0; idx < 4; idx++) {
        retStatus = fread(&bytes[idx],1,1,fp);
        if (retStatus < 1)
            return((EPS_INT32)EPSMP_BMP_ERR_READING_DATA);
    }

/*** Put bytes in return value                                                          */
    *value = (bytes[3] << 24) +
             (bytes[2] << 16) + 
             (bytes[1] <<  8) + 
              bytes[0];


/*** Return to Caller                                                                   */
    return((EPS_INT32)EPSMP_BMP_ERR_NONE);

}

/*___________________________________  epsmp-bmp.c   ___________________________________*/

/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*       1         2         3         4         5         6         7         8        */
/*******************************************|********************************************/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/***** End of File *** End of File *** End of File *** End of File *** End of File ******/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
