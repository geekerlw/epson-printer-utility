/*___________________________________  epsmp-bmp.h   ___________________________________*/

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
#ifndef __EPSON_BMP_H__
#define __EPSON_BMP_H__
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
#define EPSMP_BMP_ERR_NONE                      0     /* Routine / Action completed successfully  */
#define EPSMP_BMP_ERR_MEMORY_ALLOCATION     -5000     /* Memory Allocation Error          */
#define EPSMP_BMP_ERR_INVALID_ARG           -5001     /* Invalid Argument in call Error   */
#define EPSMP_BMP_ERR_NOT_INITIALIZED       -5002
#define EPSMP_BMP_ERR_INVALID_FILE          -5003	
#define EPSMP_BMP_ERR_FILE_NOT_OPEN         -5004
#define EPSMP_BMP_ERR_INVALID_BAND_HEIGHT   -5005
#define EPSMP_BMP_ERR_NOT_A_BMP_FILE        -5006
#define EPSMP_BMP_ERR_UNSUPPORTED_BPP       -5007
#define EPSMP_BMP_ERR_COMPRESSION_ON        -5008
#define EPSMP_BMP_ERR_READING_DATA          -5009

/*---------------------------  Data Structure Declarations   ---------------------------*/
/*******************************************|********************************************/
        /*** BMP Data Sturctures                                                        */
        /*** ---------------------------------------------------------------------------*/
/* bitmap file header structure */
#if defined(unix) || defined(__unix) || defined(WINCE)
//#pragma pack (push,1)
	typedef struct tagBITMAPFILEHEADER {
		EPS_UINT16 bfType;
		EPS_UINT32 bfSize;
		EPS_UINT16 bfReserved1;
		EPS_UINT16 bfReserved2;
		EPS_UINT32 bfOffBits;
	} BITMAPFILEHEADER;

	/* bitmap file info structure */
	typedef struct tagBITMAPINFOHEADER{
		EPS_UINT32 biSize;
		EPS_UINT32 biWidth;
		EPS_UINT32 biHeight;
		EPS_UINT16 biPlanes;
		EPS_UINT16 biBitCount;
		EPS_UINT32 biCompression;
		EPS_UINT32 biSizeImage;
		EPS_UINT32 biXPelsPerMeter;
		EPS_UINT32 biYPelsPerMeter;
		EPS_UINT32 biClrUsed;
		EPS_UINT32 biClrImportant;
	} BITMAPINFOHEADER;
//#pragma pack(pop)
#endif //Windows has it declared in Wingdhi.h

/*---------------------------  Public Function Declarations  ---------------------------*/
/*******************************************|********************************************/
extern EPS_INT32  epsmpBmpInit             (FILE* fp, EPS_INT32, EPS_BOOL);
extern EPS_INT32  epsmpBmpGetWidth         (void					);
extern EPS_INT32  epsmpBmpGetHeight        (void					);
extern EPS_INT32  epsmpBmpGetBytesPerLine  (void					);
extern EPS_UINT8  epsmpBmpGetBytesPerPixel (void					);
extern EPS_INT32  epsmpBmpGetBandSize      (void					);
extern EPS_INT32  epsmpBmpGetBandData      (EPS_UINT8**				);
extern EPS_INT32  epsmpBmpGetBandWidth     (void					);
extern EPS_INT32  epsmpBmpGetBandHeight    (void					);
extern EPS_UINT16 epsmpBmpGetPaletteSize   (void					);
extern EPS_UINT8* epsmpBmpGetPaletteData   (void					);
extern EPS_INT32  epsmpBmpRotate           (const char*, const char*);
extern EPS_INT32  epsmpBmpRelease          (void					);
extern EPS_INT32  epsmpBmpReset            (EPS_BOOL				);

#ifdef __cplusplus
}
#endif
#endif	/* __EPSON_BMP_H__ */

/*___________________________________  epsmp-bmp.h   ___________________________________*/
  
/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*       1         2         3         4         5         6         7         8        */
/*******************************************|********************************************/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/***** End of File *** End of File *** End of File *** End of File *** End of File ******/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
