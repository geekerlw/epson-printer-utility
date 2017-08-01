/*__________________________________  epsmp-image.h   __________________________________*/

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
/*                      Epson Image Functions External Definitions                      */
/*                                                                                      */
/*                                    Function Calls                                    */
/*                              --------------------------                              */
/*          EPS_ERR_CODE epsmpInitImage        (imageFileName, bandHeight, supportFunc, */
/*                                              reverse, imageBand );                   */
/*          EPS_ERR_CODE epsmpGetBandData      (imageBand);                             */
/*          EPS_ERR_CODE epsmpReleaseImage     (void     );                             */
/*          EPS_ERR_CODE epsmpResetImage       (imageBand, reverse );                   */
/*          EPS_ERR_CODE epsmpRGBtoGrayData    (imageBand);                             */
/*          EPS_ERR_CODE epsmpRGBtoGrayPalette (imageBand);                             */
/*                                                                                      */
/*******************************************|********************************************/
#ifndef __EPSON_IMAGE_H__
#define __EPSON_IMAGE_H__
#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------  Includes   -------------------------------------*/
/*******************************************|********************************************/
#include "../escpr-lib/inc/epson-typedefs.h"

/*------------------------------------- Data Types -------------------------------------*/
/*******************************************|********************************************/

/*----------------------------------  Generic Macros   ---------------------------------*/
/*******************************************|********************************************/

/*-----------------------------------  Definitions  ------------------------------------*/
/*******************************************|********************************************/
    /*** Error Codes                                                                    */
    /*** -------------------------------------------------------------------------------*/
#define EPSMP_IMAGE_ERR_NONE                        0   /* Routine / Action completed successfully  */
#define EPSMP_IMAGE_ERR_INVALID_ARG             -5200   /* Invalid Argument in call Error   */
#define EPSMP_IMAGE_ERR_FILE_NOT_OPEN           -5201
#define EPSMP_IMAGE_ERR_FATAL                   -5202
#define EPSMP_IMAGE_ERR_INIT_IMAGE              -5203
#define EPSMP_IMAGE_ERR_RELEASE_IMAGE           -5204
#define EPSMP_IMAGE_ERR_IMAGE_NOT_SUPPORTED     -5205
#define EPSMP_IMAGE_ERR_CLOSE_IMAGE				-5206

    /*** Image Types Supported                                                          */
    /*** -------------------------------------------------------------------------------*/
enum {
    EPSMP_BMP = 0,
    EPSMP_JPG, 
    EPSMP_JPGRAW, 
    EPSMP_PRN, 
    EPSMP_LAST_IMAGE_TYPE
};

/*-----------------------------  Function Type Declarations   --------------------------*/
/*******************************************|********************************************/
        /*** Image Attribute Function Definitions                                       */
        /*** ---------------------------------------------------------------------------*/
typedef EPS_INT32  (*IMAGE_INIT)                (FILE*, EPS_INT32, EPS_BOOL);
typedef EPS_INT32  (*IMAGE_GET_WIDTH)           (void                   );
typedef EPS_INT32  (*IMAGE_GET_HEIGHT)          (void                   );
typedef EPS_INT32  (*IMAGE_GET_BAND_HEIGHT)     (void                   );
typedef EPS_INT32  (*IMAGE_GET_BAND_WIDTH)      (void                   );
typedef EPS_INT32  (*IMAGE_GET_BYTES_PER_LINE)  (void                   );
typedef EPS_UINT8  (*IMAGE_GET_BYTES_PER_PIXEL) (void                   );
typedef EPS_UINT16 (*IMAGE_GET_PALETTE_SIZE)    (void                   );
typedef EPS_UINT8* (*IMAGE_GET_PALETTE_DATA)    (void                   );
typedef EPS_INT32  (*IMAGE_GET_BAND)            (EPS_UINT8**            );
typedef EPS_INT32  (*IMAGE_RELEASE)             (void                   );
typedef EPS_INT32  (*IMAGE_RESET)               (EPS_BOOL               );

/*---------------------------  Data Structure Declarations   ---------------------------*/
/*******************************************|********************************************/
    /*** Image Function Structure                                                       */
    /*** -------------------------------------------------------------------------------*/
typedef struct tag_EPSMP_IMAGE_FNS {
    IMAGE_INIT                  Init;
	IMAGE_GET_WIDTH             GetWidth;
    IMAGE_GET_HEIGHT            GetHeight;
    IMAGE_GET_BAND_HEIGHT       GetBandHeight;
    IMAGE_GET_BAND_WIDTH        GetBandWidth;
    IMAGE_GET_BYTES_PER_LINE    GetBytesPerLine;
    IMAGE_GET_BYTES_PER_PIXEL   GetBytesPerPixel;
    IMAGE_GET_PALETTE_SIZE      GetPaletteSize;
    IMAGE_GET_PALETTE_DATA      GetPaletteData;
    IMAGE_GET_BAND              GetBandData;
    IMAGE_RELEASE               Release;
    IMAGE_RESET                 Reset;
} EPSMP_IMAGE_FNS;

    /*** Band Rectangle                                                                 */
    /*** -------------------------------------------------------------------------------*/
typedef struct tag_EPSMP_IMAGE_RECT {
    EPS_INT32   top;
    EPS_INT32   left;
    EPS_INT32   bottom;
    EPS_INT32   right;
} EPSMP_IMAGE_RECT;

    /*** Generic Image Object                                                           */
    /*** -------------------------------------------------------------------------------*/
typedef struct tag_EPSMP_IMAGE_BAND {
    EPS_INT32           imageType;          /* Image file type                          */
    EPS_UINT8*          bandData;           /* Pointer to image/raster data             */
    EPSMP_IMAGE_RECT    rect;               /* Rect define position of image            */
    EPS_INT32           imageWidth;         /* Width  of image/raster in pixels         */
    EPS_INT32           imageHeight;        /* Height of image/raser in lines (pixels)  */
    EPS_INT32           bandWidth;          /* Width  of image/raster in pixels         */
    EPS_INT32           bandHeight;         /* Height of image/raser in lines (pixels)  */
    EPS_INT32           bpl;                /* Bytes-Per-Line for image/raster in bytes */
    EPS_UINT8           bpp;                /* Bytes-Per-Pixel for image/raster         */
    EPS_UINT16          paletteSize;        /* Size RGB color palette (3*num.colors)    */
    EPS_UINT8*          paletteData;        /* RGB color palette data                   */
//    void*               ext;                /* Pointer to External/Extension data       */
} EPSMP_IMAGE_BAND;

/*---------------------------  Public Function Declarations  ---------------------------*/
/*******************************************|********************************************/
EPS_ERR_CODE epsmpPrepareImage     (const EPS_INT8*, char*, EPS_UINT32, EPS_UINT32, 
									EPS_UINT32, EPS_UINT32, EPS_BOOL);
EPS_INT32    epsmpSelectImageType  (const EPS_INT8*, EPS_UINT32                         );

EPS_ERR_CODE epsmpInitImage        (const EPS_INT8*, const EPS_INT32, EPS_UINT32, 
									EPS_BOOL, EPSMP_IMAGE_BAND*                         );
EPS_ERR_CODE epsmpGetBandData      (EPSMP_IMAGE_BAND*                                   );
EPS_ERR_CODE epsmpReleaseImage     (void                                                );
EPS_ERR_CODE epsmpResetImage       (EPSMP_IMAGE_BAND*, EPS_BOOL                         );
EPS_ERR_CODE epsmpRGBtoGrayData    (EPSMP_IMAGE_BAND*                                   );
EPS_ERR_CODE epsmpRGBtoGrayPalette (EPSMP_IMAGE_BAND*                                   );
EPS_ERR_CODE epsmpRotateImage	   (EPS_UINT32, EPS_UINT32, const char *, char *        );
EPS_INT32	 epsmpIsImageFile	   (const char* fileName                                );

#ifdef __cplusplus
}
#endif
#endif	/* __EPSON_IMAGE_H__ */

/*__________________________________  epsmp-image.h   __________________________________*/
  
/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*       1         2         3         4         5         6         7         8        */
/*******************************************|********************************************/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/***** End of File *** End of File *** End of File *** End of File *** End of File ******/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
