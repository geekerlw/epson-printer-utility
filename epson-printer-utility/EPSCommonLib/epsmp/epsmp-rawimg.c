/*_________________________________  epsmp-rawimg.c   __________________________________*/

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

/*------------------------------------  Includes   -------------------------------------*/
/*******************************************|********************************************/
#include "included_headers.h"
#include "epsmp-rawimg.h"


/*------------------------------------  Definition   -----------------------------------*/
/*******************************************|********************************************/
//#define EPS_RAWIMG_BUFFER_SIZE	(1024*4)
#define EPS_RAWIMG_BUFFER_SIZE	(1024*64-1)

/*------------------------------  Local Global Variables  ------------------------------*/
/*******************************************|********************************************/
    /*** File I/O and Indexing                                                          */
    /*** ------------------------------------------------------------------------------ */
static FILE*            rawimgFP = 0;
static EPS_INT32        fileSize = 0;
static EPS_UINT8*       retBandData = NULL;
static EPS_INT32        linesRead = 0;


/*---------------------------  Data Structure Declarations   ---------------------------*/
/*******************************************|********************************************/

/*--------------------------------  Local Functions   ----------------------------------*/
/*******************************************|********************************************/

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpRawInit()                                                      */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* fp           FILE*               Image File Pointer                                  */
/* bandHeight   EPS_INT32           Image Band Height                                   */
/* reverse      EPS_INT32           get band reverse                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPSMP_RAW_ERR_NONE                  - Success                                   */
/*      EPSMP_RAW_ERR_MEMORY_ALLOCATION                                                 */
/*      EPSMP_RAW_ERR_INVALID_FILE                                                      */
/*      EPSMP_RAW_ERR_NOT_A_BMP_FILE                                                    */
/*      EPSMP_RAW_ERR_UNSUPPORTED_BPP                                                   */
/*      EPSMP_RAW_ERR_COMPRESSION_ON                                                    */
/*                                                                                      */
/* Description:                                                                         */
/*      Initialize BMP file.                                                            */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32 epsmpRawInit (

        FILE*       fp,
        EPS_INT32   bandHeight,
		EPS_BOOL	reverse

){

	(void)bandHeight;
	(void)reverse;

/*** Validate input parameters                                                          */
    if (!fp)
        return((EPS_INT32)EPSMP_RAW_ERR_INVALID_FILE);

    rawimgFP = fp;

	linesRead = 0;

	if(0 == fseek(rawimgFP, 0, SEEK_END) ){
		fileSize = ftell(rawimgFP);
		fseek(rawimgFP, 0, SEEK_SET);
	}

//    retBandData = malloc(EPS_RAWIMG_BUFFER_SIZE);
    if (retBandData == NULL)
        return (EPS_INT32)EPSMP_RAW_ERR_MEMORY_ALLOCATION;

    return((EPS_INT32)EPSMP_RAW_ERR_NONE);
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpRawGetWidth()                                                  */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* ----         void                                                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      Image width in pixels           - Success                                       */
/*      EPSMP_RAW_ERR_NOT_INITIALIZED                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Return image width in pixels.                                                   */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   epsmpRawGetWidth (

        void

){
/*** Has file been Initialized                                                          */
    if (0 == rawimgFP)
        return((EPS_INT32)EPSMP_RAW_ERR_NOT_INITIALIZED);

	return fileSize;
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpRawGetHeight()                                                 */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* ----         void                                                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      Image height in lines (pixels)      - Success                                   */
/*      EPSMP_RAW_ERR_NOT_INITIALIZED                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Return image height in lines (pixels)                                           */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   epsmpRawGetHeight (

        void

) {
/*** Has file been Initialized                                                          */
    if (0 == rawimgFP)
        return((EPS_INT32)EPSMP_RAW_ERR_NOT_INITIALIZED);

	return 1; /* dummy retrive */
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpRawGetBytesPerLine()                                           */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* ----         void                                                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      Image BPL in pixels             - Success                                       */
/*      EPSMP_RAW_ERR_NOT_INITIALIZED                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Return image BPL in pixels                                                      */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   epsmpRawGetBytesPerLine (

        void

) {
/*** Has file been Initialized                                                          */
    if (0 == rawimgFP)
        return((EPS_INT32)EPSMP_RAW_ERR_NOT_INITIALIZED);

	return 1; /* dummy retrive */
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpRawGetBytesPerPixel()                                          */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* ----         void                                                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      Current Bytes Per Pixel         - Success                                       */
/*      EPSMP_RAW_ERR_NOT_INITIALIZED                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Return Bytes Per Pixel.                                                         */
/*                                                                                      */
/*******************************************|********************************************/
EPS_UINT8 epsmpRawGetBytesPerPixel (

        void

) {
	return 1; /* dummy retrive */
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpRawGetBandSize()                                               */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* ----         void                                                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      Band size in bytes              - Success                                       */
/*      EPSMP_RAW_ERR_NOT_INITIALIZED                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Return band size in bytes                                                       */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   epsmpRawGetBandSize (

        void

) {
/*** Has file been Initialized                                                          */
    if (0 == rawimgFP)
        return((EPS_INT32)EPSMP_RAW_ERR_NOT_INITIALIZED);

    return 1;
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name: epsmpRawGetBandData()                                                 */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* dest         EPS_UINT8**         O: Read band data                                   */
/*                                                                                      */
/* Return value:                                                                        */
/*      Number of bands read            - Success                                       */
/*      EPSMP_RAW_ERR_NOT_INITIALIZED                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Read band data and return number of bands read.                                 */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   epsmpRawGetBandData (

        EPS_UINT8**     dest

) {

/*** Has file been Initialized                                                          */
    if (0 == rawimgFP)
        return((EPS_INT32)EPSMP_RAW_ERR_NOT_INITIALIZED);

	linesRead = (EPS_INT32)fread(retBandData, 1, EPS_RAWIMG_BUFFER_SIZE, rawimgFP);
	*dest = retBandData;

/*** Return to Caller                                                                   */
    return((EPS_INT32)linesRead);
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpRawGetBandWidth()                                              */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* ----         void                                                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      Band width in lines (pixels)        - Success                                   */
/*      EPSMP_RAW_ERR_NOT_INITIALIZED                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Return band width in lines (pixels)                                             */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   epsmpRawGetBandWidth (

        void

) {
/*** Has file been Initialized                                                          */
    if (0 == rawimgFP)
        return((EPS_INT32)EPSMP_RAW_ERR_NOT_INITIALIZED);

    return 1; /* dummy retrive */
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpRawGetBandHeight()                                             */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* ----         void                                                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      Band height in lines (pixels)       - Success                                   */
/*      EPSMP_RAW_ERR_NOT_INITIALIZED                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Return band height in lines (pixels)                                            */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   epsmpRawGetBandHeight (

        void

) {
/*** Has file been Initialized                                                          */
    if (0 == rawimgFP)
        return((EPS_INT32)EPSMP_RAW_ERR_NOT_INITIALIZED);

    return linesRead;
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpRawGetPaletteSize()                                            */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* ----         void                                                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      Palette size in bytes           - Success                                       */
/*      EPSMP_RAW_ERR_NOT_INITIALIZED                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Return palette size in bytes                                                    */
/*                                                                                      */
/*******************************************|********************************************/
EPS_UINT16   epsmpRawGetPaletteSize (

        void

) {
    return 0;
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpRawGetPaletteData()                                            */
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
EPS_UINT8*  epsmpRawGetPaletteData (

        void

) {
    return((EPS_UINT8*)NULL);
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name    epsmpRawRelease()                                                   */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* ----         void                                                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPSMP_RAW_ERR_NONE                  - Success                                   */
/*      EPSMP_RAW_ERR_NOT_INITIALIZED                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Release BMP Data.                                                               */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   epsmpRawRelease (

        void

) {
/*** Has file been Initialized                                                          */
    if (0 == rawimgFP)
        return((EPS_INT32)EPSMP_RAW_ERR_NOT_INITIALIZED);

	rawimgFP = 0;

	if(retBandData){
		free(retBandData);
		retBandData = NULL;
	}

    return((EPS_INT32)EPSMP_RAW_ERR_NONE);
}


EPS_INT32   epsmpRawReset (

        EPS_BOOL reverse

) {
	(void)reverse;

    if (0 == rawimgFP)
        return((EPS_INT32)EPSMP_RAW_ERR_NOT_INITIALIZED);

	fseek(rawimgFP, 0, SEEK_SET);
    return((EPS_INT32)EPSMP_RAW_ERR_NONE);
}
/*_________________________________  epsmp-rawimg.c   __________________________________*/

/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*       1         2         3         4         5         6         7         8        */
/*******************************************|********************************************/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/***** End of File *** End of File *** End of File *** End of File *** End of File ******/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
