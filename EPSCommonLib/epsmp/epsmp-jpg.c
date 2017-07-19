/*___________________________________  epsmp-jpg.c   ___________________________________*/

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
/*                                 Epson JPEG Functions                                 */
/*                                                                                      */
/*                                    Core Function Calls                               */
/*                                --------------------------                            */
/*                  EPS_INT32  epsmpJpgInit            (File, bandHeight, reverse);     */
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
#ifdef GCOMSW_JPEG_SUPPORT

/*------------------------------------  Includes   -------------------------------------*/
/*******************************************|********************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>     /*** ##########      Not recommended by Visual C++ IDE          */
#include "jpeglib.h"
#include "transupp.h"	/* Used only for rotation */
#include "epsmp-bmp.h"
#include "epsmp-jpg.h"

#include "epson-escpr-mem.h"	//memory debug utility

/*------------------------------------  Definition   -----------------------------------*/
/*******************************************|********************************************/

/*---------------------------  Data Structure Declarations   ---------------------------*/
/*******************************************|********************************************/

        /*** Error Sturctures                                                           */
        /*** ---------------------------------------------------------------------------*/
struct my_error_mgr {
  struct    jpeg_error_mgr pub;             /* "public" fields                          */
  jmp_buf   setjmp_buffer;                  /* for return to caller                     */
};
typedef struct my_error_mgr * my_error_ptr;

/*------------------------------  Local Global Variables  ------------------------------*/
/*******************************************|********************************************/
static EPS_INT32 thisCurrentLine = -1;

    /*** JPEG Image Attributes                                                          */
    /*** ------------------------------------------------------------------------------ */
static EPS_UINT8* retBandData;
static EPS_INT32 retBandHeight;

    /*** Misc                                                                           */
    /*** ------------------------------------------------------------------------------ */
struct jpeg_decompress_struct   cinfo;
struct my_error_mgr             jerr;
EPS_INT32                       row_stride;
JSAMPARRAY                      buffer;

struct repeatablInfo{
    FILE*       fp;
    EPS_INT32   inBandHeight;
} g_repInfo;

/*--------------------------------  Local Functions   ----------------------------------*/
/*******************************************|********************************************/
static void my_error_exit (j_common_ptr);

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpJpgInit()                                                      */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* fp           FILE*               Image File Pointer                                  */
/* bandHeight   EPS_INT32           Image Band Height                                   */
/* reverse      EPS_INT32           get band reverse                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPSMP_JPG_ERR_NONE                  - Success                                   */
/*      EPSMP_JPG_ERR_MEMORY_ALLOCATION                                                 */
/*      EPSMP_JPG_ERR_NOT_INITIALIZED                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Initialize JPEG file.                                                           */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   epsmpJpgInit (

        FILE*       fp,
        EPS_INT32   inBandHeight,
		EPS_BOOL	reverse

){

/*** Declare Variable Local to Routine                                                  */
	(void)reverse;

/*** Initialize Global/Local Variables                                                  */
    thisCurrentLine = 0;
    retBandData     = NULL;

/*** Has Jpg file been Initialized                                                      */
    if (!fp) {
        thisCurrentLine = -1;
        return((EPS_INT32)EPSMP_JPG_ERR_INVALID_FILE);
    }

    cinfo.err           = jpeg_std_error(&(jerr.pub));
    jerr.pub.error_exit = my_error_exit;

    if (setjmp(jerr.setjmp_buffer)) {
        thisCurrentLine = -1;
        return((EPS_INT32)EPSMP_JPG_ERR_NONE);
    }
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src        (&cinfo, fp);
    jpeg_read_header      (&cinfo, TRUE);

	jpeg_start_decompress (&cinfo);
  
    row_stride = cinfo.output_width * cinfo.output_components;
    buffer     = (*(cinfo.mem->alloc_sarray))
                                    ((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

    retBandHeight = inBandHeight;

    retBandData   = malloc(epsmpJpgGetBandSize() * retBandHeight);
    if (retBandData == NULL) {
        jpeg_destroy_decompress(&cinfo);
		thisCurrentLine = -1;
        return (EPS_INT32)EPSMP_JPG_ERR_MEMORY_ALLOCATION;
    }

	g_repInfo.fp = fp;
	g_repInfo.inBandHeight =inBandHeight;
/*** Return to Caller                                                                   */
    return((EPS_INT32)EPSMP_JPG_ERR_NONE);

}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpJpgGetWidth()                                                  */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* ----         void                                                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      Image width in pixels           - Success                                       */
/*      EPSMP_JPG_ERR_NOT_INITIALIZED                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Return image width in pixels.                                                   */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   epsmpJpgGetWidth (

        void

){

/*** Declare Variable Local to Routine                                                  */

/*** Initialize Local Variables                                                         */

/*** Has Jpg file been Initialized                                                      */
    if (thisCurrentLine == -1)
        return((EPS_INT32)EPSMP_JPG_ERR_NOT_INITIALIZED);

/*** Return to Caller                                                                   */
    return((EPS_INT32)cinfo.output_width);
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpJpgGetHeight()                                                 */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* ----         void                                                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      Image height in lines (pixels)      - Success                                   */
/*      EPSMP_JPG_ERR_NOT_INITIALIZED                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Return image height in lines (pixels)                                           */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   epsmpJpgGetHeight (

        void

){

/*** Declare Variable Local to Routine                                                  */

/*** Initialize Local Variables                                                         */

/*** Has Jpg file been Initialized                                                      */
    if (thisCurrentLine == -1)
        return((EPS_INT32)EPSMP_JPG_ERR_NOT_INITIALIZED);

/*** Return to Caller                                                                   */
    return((EPS_INT32)cinfo.output_height);
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpJpgGetBytesPerLine()                                           */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* ----          void                                                                   */
/*                                                                                      */
/* Return value:                                                                        */
/*      Image BPL in pixels             - Success                                       */
/*      EPSMP_JPG_ERR_NOT_INITIALIZED                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Return image BPL in pixels                                                      */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   epsmpJpgGetBytesPerLine (

        void

){

/*** Declare Variable Local to Routine                                                  */

/*** Initialize Local Variables                                                         */

/*** Has Jpg file been Initialized                                                      */
    if (thisCurrentLine == -1)
        return((EPS_INT32)EPSMP_JPG_ERR_NOT_INITIALIZED);

/*** Return to Caller                                                                   */
/*    return((EPS_INT32)((cinfo.output_width * 3 + 3) & 0xFFFFFFFC));*/
	return (EPS_INT32)(cinfo.output_width * 3);
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpJpgGetBytesPerPixel()                                          */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* ----         void                                                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      Current Bytes Per Pixel         - Success                                       */
/*      EPSMP_JPG_ERR_NOT_INITIALIZED                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Return Bytes Per Pixel.                                                         */
/*                                                                                      */
/*******************************************|********************************************/
EPS_UINT8   epsmpJpgGetBytesPerPixel (

        void

){

/*** Declare Variable Local to Routine                                                  */

/*** Initialize Local Variables                                                         */

/*** Return to Caller                                                                   */
    return((EPS_UINT8)3);
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpJpgGetBandSize()                                               */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* ----          void                                                                   */
/*                                                                                      */
/* Return value:                                                                        */
/*      Band size in bytes              - Success                                       */
/*      EPSMP_JPG_ERR_NOT_INITIALIZED                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Return band size in bytes                                                       */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   epsmpJpgGetBandSize (

        void

){

/*** Declare Variable Local to Routine                                                  */

/*** Initialize Local Variables                                                         */

/*** Has Jpg file been Initialized                                                      */
    if (thisCurrentLine == -1)
        return((EPS_INT32)EPSMP_JPG_ERR_NOT_INITIALIZED);

/*** Return to Caller                                                                   */
/*    return((EPS_INT32)((cinfo.output_width * 3 + 3) & 0xFFFFFFFC));*/
	return (EPS_INT32)(cinfo.output_width * 3);
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpJpgGetBandData()                                               */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* buf          EPS_UINT8**         O: Read band data                                   */
/*                                                                                      */
/* Return value:                                                                        */
/*      Number of bands read            - Success                                       */
/*      EPSMP_JPG_ERR_NOT_INITIALIZED                                                   */
/*      EPSMP_JPG_ERR_READING_DATA                                                      */
/*                                                                                      */
/* Description:                                                                         */
/*      Read band data and return number of bands read.                                 */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   epsmpJpgGetBandData (

        EPS_UINT8**     buf

){

/*** Declare Variable Local to Routine                                                  */
EPS_INT32   retStatus;                      /* Return status of internal calls          */
EPS_INT32   idx;
EPS_INT32   readLines;

/*** Initialize Local Variables                                                         */
    readLines = 0;

/*** Has Jpg file been Initialized                                                      */
    if (thisCurrentLine == -1)
        return((EPS_INT32)EPSMP_JPG_ERR_NOT_INITIALIZED);

/*** Read Data from JPEG file                                                           */
    for ( idx = 0; 
         (idx < epsmpJpgGetBandHeight()) && 
          ((EPS_INT32)cinfo.output_scanline < epsmpJpgGetHeight());
          idx++) {

        retStatus = jpeg_read_scanlines(&cinfo, buffer, 1);
        if (retStatus < 0)
            return((EPS_INT32)EPSMP_JPG_ERR_READING_DATA);

        memcpy(retBandData + idx*epsmpJpgGetBandSize(), buffer[0], row_stride);
        thisCurrentLine++;
        readLines++;
    }
    
    *buf = retBandData;

/*** Return to Caller                                                                   */
    return((EPS_INT32)readLines);
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpJpgGetBandWidth()                                              */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* ----         void                                                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      Band width in lines (pixels)        - Success                                   */
/*      EPSMP_JPG_ERR_NOT_INITIALIZED                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Return band width in lines (pixels)                                             */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   epsmpJpgGetBandWidth (

        void

){

/*** Declare Variable Local to Routine                                                  */

/*** Initialize Local Variables                                                         */

/*** Has Jpg file been Initialized                                                      */
    if (thisCurrentLine == -1)
        return((EPS_INT32)EPSMP_JPG_ERR_NOT_INITIALIZED);

/*** Return to Caller                                                                   */
    return((EPS_INT32)epsmpJpgGetWidth());
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpJpgGetBandHeight()                                             */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* ----         void                                                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      Band height in lines (pixels)       - Success                                   */
/*      EPSMP_JPG_ERR_NOT_INITIALIZED                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Return band height in lines (pixels)                                            */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   epsmpJpgGetBandHeight (

        void

){

/*** Declare Variable Local to Routine                                                  */

/*** Initialize Local Variables                                                         */

/*** Has Jpg file been Initialized                                                      */
    if (thisCurrentLine == -1)
        return((EPS_INT32)EPSMP_JPG_ERR_NOT_INITIALIZED);

/*** Return to Caller                                                                   */
    return((EPS_INT32)retBandHeight);
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpJpgGetPaletteSize()                                            */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* ----         void                                                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      Palette size in bytes           - Success                                       */
/*      EPSMP_JPG_ERR_NOT_INITIALIZED                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Return palette size in bytes                                                    */
/*                                                                                      */
/*******************************************|********************************************/
EPS_UINT16   epsmpJpgGetPaletteSize (

        void

){

/*** Declare Variable Local to Routine                                                  */

/*** Initialize Local Variables                                                         */


/*** Return to Caller                                                                   */
    return((EPS_UINT16)0);
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpJpgGetPaletteData()                                            */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* ----         void                                                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      NULL                            - Success                                       */
/*                                                                                      */
/* Description:                                                                         */
/*      Return NULL.                                                                    */
/*                                                                                      */
/*******************************************|********************************************/
EPS_UINT8*  epsmpJpgGetPaletteData(

        void

) {

/*** Declare Variable Local to Routine                                                  */

/*** Initialize Local Variables                                                         */

/*** Has Jpg file been Initialized                                                      */
    if (thisCurrentLine == -1)
        return((EPS_UINT8*)NULL);

/*** Return to Caller                                                                   */
    return((EPS_UINT8*)NULL);
}


/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpJpgRotate()										            */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* inFileName   const char*          Name of file with jpg                              */
/* outFileName  const char*          Name of Conversion result file with jpg            */
/*                                                                                      */
/* Return value:                                                                        */
/*      NULL                            - Success                                       */
/*                                                                                      */
/* Description:                                                                         */
/*      Image rotatin 90deg.                                                            */
/*                                                                                      */
/*******************************************|********************************************/
EPS_ERR_CODE epsmpJpgRotate (const char* inFileName, const char* outFileName)
{

	jpeg_transform_info transformoption; /* image transformation options */
	JCOPY_OPTION copyoption = JCOPYOPT_ALL;	/* -copy switch */

	struct jpeg_decompress_struct srcinfo;
	struct jpeg_compress_struct dstinfo;
	struct jpeg_error_mgr jsrcerr, jdsterr;

	jvirt_barray_ptr * src_coef_arrays;
	jvirt_barray_ptr * dst_coef_arrays;
	FILE * input_file;
	FILE * output_file;
	EPS_INT8*	buf;
	
	memset(&transformoption, 0x00, sizeof(transformoption));
	transformoption.trim = TRUE;
	transformoption.force_grayscale = FALSE;
	transformoption.transform = JXFORM_NONE;
	transformoption.transform = JXFORM_ROT_90;

	/* Initialize the JPEG decompression object with default error handling. */
	srcinfo.err = jpeg_std_error(&jsrcerr);
	jpeg_create_decompress(&srcinfo);

	/* Initialize the JPEG compression object with default error handling. */	
	dstinfo.err = jpeg_std_error(&jdsterr);
	dstinfo.err->trace_level = 0;  //For memory usage.
	jpeg_create_compress(&dstinfo);
	jsrcerr.trace_level = jdsterr.trace_level;
	//dstinfo.mem->max_memory_to_use = 5000000;  // Set max memory to use
	srcinfo.mem->max_memory_to_use = dstinfo.mem->max_memory_to_use;
	buf = malloc(dstinfo.mem->max_memory_to_use);
	memset(buf, 0, dstinfo.mem->max_memory_to_use);

	/* Open the input file. */
	if ((input_file = fopen(inFileName, "rb")) == NULL) {
		free(buf);
		return EPSMP_JPG_ERR_INVALID_FILE;
	}
	if ((output_file = fopen(outFileName, "wb")) == NULL) {
		free(buf);
		return EPSMP_JPG_ERR_INVALID_FILE;
	}

	/* Specify data source for decompression */
	jpeg_stdio_src(&srcinfo, input_file);

	/* Enable saving of extra markers that we want to copy */
	//jcopy_markers_setup(&srcinfo, copyoption);

	/* Read file header */
	(void) jpeg_read_header(&srcinfo, TRUE);

	/* Any space needed by a transform option must be requested before
	* jpeg_read_coefficients so that memory allocation will be done right.
	*/
	jtransform_request_workspace(&srcinfo, &transformoption);

	/* Read source file as DCT coefficients */
	src_coef_arrays = jpeg_read_coefficients(&srcinfo);

	/* Initialize destination compression parameters from source values */
	jpeg_copy_critical_parameters(&srcinfo, &dstinfo);

	/* Adjust destination parameters if required by transform options;
	* also find out which set of coefficient arrays will hold the output.
	*/
	dst_coef_arrays = jtransform_adjust_parameters(&srcinfo, &dstinfo,
						 src_coef_arrays,
						 &transformoption);

	/* Specify data destination for compression */
	jpeg_stdio_dest(&dstinfo, output_file);

	/* Start compressor (note no image data is actually written here) */
	jpeg_write_coefficients(&dstinfo, dst_coef_arrays);

	/* Copy to the output file any extra markers that we want to preserve */
	jcopy_markers_execute(&srcinfo, &dstinfo, copyoption);

	/* Execute image transformation, if any */
	jtransform_execute_transform(&srcinfo, &dstinfo,
					src_coef_arrays,
					&transformoption);

	/* Finish compression and release memory */
	jpeg_finish_compress(&dstinfo);
	jpeg_destroy_compress(&dstinfo);
	(void) jpeg_finish_decompress(&srcinfo);
	jpeg_destroy_decompress(&srcinfo);

	/* Close files, if we opened them */
	if (input_file != stdin){
		fclose(input_file);
	}
	if (output_file != stdout){
		fclose(output_file);
	}

	free(buf);

	return EPSMP_JPG_ERR_NONE;
}


/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpJpgGetSize()										            */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* inFileName   const char*          Name of file with jpg                              */
/* pFileSize    EPS_UINT32*          file size.                                         */
/* pHeight      EPS_UINT32*          Image Height.                                      */
/* pWidth       EPS_UINT32*          Image Width.                                       */
/*                                                                                      */
/* Return value:                                                                        */
/*      NULL                            - Success                                       */
/*                                                                                      */
/* Description:                                                                         */
/*      Get the Jpeg File Size and Image Size.                                          */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32 epsmpJpgGetSize (const char* inFileName,  
								EPS_UINT32 *pFileSize, EPS_UINT32 *pHeight, EPS_UINT32 *pWidth)
{
	EPS_INT32 ret = EPSMP_JPG_ERR_NONE;
	FILE*	fp;
	unsigned char    bufMkr[2], bufLen[2];
	long    nLen = 0;

	*pFileSize = *pWidth = *pHeight = 0;

	if ((fp = fopen(inFileName, "rb")) == NULL) {
		return EPSMP_JPG_ERR_INVALID_FILE;
	}

	/* check the SOI maker */
	if(fread(bufMkr, 1, 2, fp) != 2){		
		ret = EPSMP_JPG_ERR_INVALID_FILE;
		goto epsmpJpgGetSize_END;
	}
	if( !(bufMkr[0] == 0xFF && bufMkr[1] == 0xD8) ){
		ret = EPSMP_JPG_ERR_INVALID_FILE;
		goto epsmpJpgGetSize_END;
	}

	/* find SOFn Marker */
	do{
		if(fread(bufMkr, 1, 2, fp) < 2){	/* read marker */
			break;
		}
		if( bufMkr[0] != 0xFF ){
			ret = EPSMP_JPG_ERR_INVALID_FILE;
			break;
		}

		if(fread(bufLen, 1, 2, fp) < 2){	/* read segment lenght */
			ret = EPSMP_JPG_ERR_INVALID_FILE;
			break;
		}

		switch(bufMkr[1]){
		case 0xC0: case 0xC1: case 0xC2: 
		case 0xC3: case 0xC5: case 0xC6: 
		case 0xC7: case 0xC9: case 0xCA: 
		case 0xCB: case 0xCD: case 0xCE: case 0xCF: /* SOF Marker */
			fgetc(fp);	/* skip Component num */

			/* get height */
			if(fread(bufLen, 1, 2, fp) < 2){	
				ret = EPSMP_JPG_ERR_INVALID_FILE;
				break;
			}
			*pHeight = (bufLen[0] << 8) + bufLen[1];

			/* get width */
			if(fread(bufLen, 1, 2, fp) < 2){	
				ret = EPSMP_JPG_ERR_INVALID_FILE;
				break;
			}
			*pWidth = (bufLen[0] << 8) + bufLen[1];

			break;

		default:
			nLen = (bufLen[0] << 8) + bufLen[1];
			fseek(fp, nLen-2, SEEK_CUR); /* skip */
			break;
		}

	}while(EPSMP_JPG_ERR_NONE == ret && 0 == *pWidth && 0 == *pHeight);
	
	/* get file size */
	fseek(fp, 0, SEEK_END);
	*pFileSize = ftell(fp);

epsmpJpgGetSize_END:

	fclose(fp);

	return ret;
}


/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpJpgReduction()										            */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* inFileName   const char*          Name of file with jpg                              */
/* outFileName  const char*          Name of Conversion result file with jpg            */
/* quality      EPS_INT32            down quality. allowed value is 1 - 100.            */
/* fraction     EPS_INT32            Reduct fraction. allowed value is 2, 4, or 8.      */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPSMP_JPG_ERR_NONE                  - Success                                   */
/*      EPSMP_JPG_ERR_INVALID_PARAM                                                     */
/*      EPSMP_JPG_ERR_INVALID_FILE                                                      */
/*      EPSMP_JPG_ERR_READING_DATA                                                      */
/*                                                                                      */
/* Description:                                                                         */
/*      down quality 1% - 100%. Image Reduction 1/2, 1/4 or 1/8.                        */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32 epsmpJpgReduction (const char* inFileName, const char* outFileName, 
								EPS_INT32 quality, EPS_INT32 fraction)
{
	EPS_INT32 retStatus = EPSMP_JPG_ERR_NONE; /* Return status of internal calls      */

	JCOPY_OPTION copyoption = JCOPYOPT_ALL;	/* -copy switch */
	struct jpeg_decompress_struct srcinfo;
	struct jpeg_compress_struct dstinfo;
	FILE * input_file;
	FILE * output_file;

	if(quality < 0 || quality > 100){
		return EPSMP_JPG_ERR_INVALID_PARAM;
	}
	if( 1 != fraction && 2 != fraction && 4 != fraction && 8 != fraction ){
		return EPSMP_JPG_ERR_INVALID_PARAM;
	}

	memset(&srcinfo, 0x00, sizeof(srcinfo));	
	memset(&dstinfo, 0x00, sizeof(dstinfo));	

	/* Initialize the JPEG decompression object with default error handling. */
    srcinfo.err         = jpeg_std_error(&(jerr.pub));
    jerr.pub.error_exit = my_error_exit;
    if (setjmp(jerr.setjmp_buffer)) {
        thisCurrentLine = -1;
        return((EPS_INT32)EPSMP_JPG_ERR_NONE);
    }
	jpeg_create_decompress(&srcinfo);

	/* Initialize the JPEG compression object with default error handling. */	
    dstinfo.err         = jpeg_std_error(&(jerr.pub));
    jerr.pub.error_exit = my_error_exit;
    if (setjmp(jerr.setjmp_buffer)) {
        thisCurrentLine = -1;
        return((EPS_INT32)EPSMP_JPG_ERR_NONE);
    }
	jpeg_create_compress(&dstinfo);

	/* Open the input file. */
	if ((input_file = fopen(inFileName, "rb")) == NULL) {
		retStatus = EPSMP_JPG_ERR_INVALID_FILE;
		goto epsmpJpgReduction_END;
	}
	if ((output_file = fopen(outFileName, "wb")) == NULL) {
		retStatus = EPSMP_JPG_ERR_INVALID_FILE;
		goto epsmpJpgReduction_END;
	}

	/* Specify data source for decompression */
	jpeg_stdio_src(&srcinfo, input_file);

	/* Enable saving of extra markers that we want to copy */
	jcopy_markers_setup(&srcinfo, copyoption);

	/* Read file header */
	(void) jpeg_read_header(&srcinfo, TRUE);
	/*srcinfo.scale_denom = fraction;*/

	/* Initialize destination compression parameters from source values */
	jpeg_copy_critical_parameters(&srcinfo, &dstinfo);
	dstinfo.image_height /= fraction;
 	dstinfo.image_width /= fraction;
	dstinfo.input_components = 3;
	dstinfo.in_color_space = JCS_RGB;
	jpeg_set_quality( &dstinfo, quality, TRUE );

	/* Specify data destination for compression */
	jpeg_stdio_dest(&dstinfo, output_file);

	jpeg_start_decompress (&srcinfo);
	jpeg_start_compress( &dstinfo, TRUE );

    /* Copy to the output file any extra markers that we want to preserve */
	jcopy_markers_execute(&srcinfo, &dstinfo, copyoption);

	row_stride = srcinfo.output_width * srcinfo.output_components;
    buffer     = (*(srcinfo.mem->alloc_sarray))
                                    ((j_common_ptr)&srcinfo, JPOOL_IMAGE, row_stride, 1);

    while( srcinfo.output_scanline < srcinfo.output_height ){
		if (jpeg_read_scanlines(&srcinfo, buffer, 1) < 0){
			retStatus = EPSMP_JPG_ERR_READING_DATA;
			goto epsmpJpgReduction_END;
		}
		jpeg_write_scanlines( &dstinfo, buffer, 1 );
    }

epsmpJpgReduction_END:
	/* Finish compression and release memory */
	jpeg_finish_compress(&dstinfo);
	jpeg_destroy_compress(&dstinfo);
	(void) jpeg_finish_decompress(&srcinfo);
	jpeg_destroy_decompress(&srcinfo);

	/* Close files, if we opened them */
	if (input_file != stdin){
		fclose(input_file);
	}
	if (output_file != stdout){
		fclose(output_file);
	}

	return retStatus;
}


/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpJpgReductionSize()									            */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* inFileName   const char*          Name of file with jpg                              */
/* outFileName  const char*          Name of Conversion result file with jpg            */
/* outFileSize  EPS_INT32            Target size of file.                               */
/* fraction     EPS_INT32            Reduct fraction. allowed value is 2, 4, or 8.      */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPSMP_JPG_ERR_NONE                  - Success                                   */
/*      EPSMP_JPG_ERR_INVALID_PARAM                                                     */
/*      EPSMP_JPG_ERR_INVALID_FILE                                                      */
/*      EPSMP_JPG_ERR_READING_DATA                                                      */
/*                                                                                      */
/* Description:                                                                         */
/*      Reduction of size of jpeg file.                                                 */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32 epsmpJpgReductionSize (const char* inFileName, const char* outFileName, 
								EPS_UINT32 outFileSize, EPS_INT32 fraction)
{
	EPS_INT32 retStatus = EPSMP_JPG_ERR_NONE; /* Return status of internal calls        */
	FILE*     fp = NULL;
	EPS_INT32 fileSize = 0;
	EPS_INT32 ratio = 100;
	EPS_INT32 ratioHi = 100;
	EPS_INT32 ratioLow = 0;
	double    resRate = .0;

	fp = fopen(inFileName, "r");
	if(fp){
		if( 0 == fseek(fp, 0, SEEK_END) ){
			fileSize = ftell(fp);
			fseek(fp, 0, SEEK_SET);
		}
		fclose(fp);

		ratio = outFileSize * 100 / fileSize;
		if(ratio > 100)ratio = 75;	/* DEFAULT. = IJG Recommendation */

		do{
			/*printf("ratio = %d ", ratio);*/
			retStatus = epsmpJpgReduction(inFileName, outFileName, ratio, fraction);
			if(EPSMP_JPG_ERR_NONE != retStatus){
				break;
			}

			fp = fopen(outFileName, "r");
			if( 0 == fseek(fp, 0, SEEK_END) ){
				fileSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
			}
			fclose(fp);

			resRate = (double)fileSize / outFileSize;
			/*printf("=> %lf%%\n", resRate);*/
			if( resRate < 1.0 && resRate > 0.98 ){ /* 98% */
				break;
			} else{
				if(resRate >= 1.0 ){
					if(ratioHi > ratio){
						ratioHi = ratio;
					}

					ratio = (EPS_INT32)(ratio / resRate);
					ratio = (EPS_INT32)(ratio * 0.9);
					if(ratio >= ratioHi){
						ratio = ratioHi - 1;
					} else if(ratio <= ratioLow){
						ratio = (ratioHi + ratioLow) / 2;
					}
				} else{ /* It's too little */
					if(ratioLow < ratio){
						ratioLow = ratio;
					}

					ratio = ratio + (EPS_INT32)((ratioHi - ratio) * (1 - resRate));
					ratio = ratio + (ratioHi - ratio) / 2;
					if(ratio <= ratioLow){
						ratio = ratioLow + 1;
					}
				}

				if(ratioHi <= ratioLow + 1){
					if(resRate > 1.0 ){
						retStatus = epsmpJpgReduction(inFileName, outFileName, ratioLow, fraction);
					}
					break;
				}
			}

		}while(EPSMP_JPG_ERR_NONE == retStatus);

	} else{
		retStatus = EPSMP_JPG_ERR_INVALID_PARAM;
	}

	return retStatus;
}


/*******************************************|********************************************/
/*                                                                                      */
/* Function name    epsmpJpgRelease()                                                   */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* ----         void                                                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPSMP_JPG_ERR_NONE                  - Success                                   */
/*      EPSMP_JPG_ERR_NOT_INITIALIZED                                                   */
/*                                                                                      */
/* Description:                                                                         */
/*      Release BMP Data.                                                               */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   epsmpJpgRelease (

        void

){

/*** Declare Variable Local to Routine                                                  */

/*** Initialize Local Variables                                                         */

/*** Release buffers                                                                    */
	if (retBandData != NULL){
        free(retBandData);
		retBandData = NULL;
	}

/*** Has Jpg file been Initialized                                                      */
    if (thisCurrentLine == -1)
        return((EPS_INT32)EPSMP_JPG_ERR_NOT_INITIALIZED);

/*** Clean allocated buffers                                                            */
    jpeg_destroy_decompress(&cinfo);

/*** Return to Caller                                                                   */
    thisCurrentLine = -1;
    return((EPS_INT32)EPSMP_JPG_ERR_NONE);
}


EPS_INT32   epsmpJpgReset (

        EPS_BOOL reverse

) {
	(void)reverse;
#if 0
//	jpeg_finish_decompress(&cinfo);
//	jpeg_start_decompress (&cinfo);
	cinfo.output_scanline = 0;
	cinfo.global_state = 205/*DSTATE_START*/;

    thisCurrentLine = 0;
    return((EPS_INT32)EPSMP_JPG_ERR_NONE);
#else
	epsmpJpgRelease();
    if (0 == g_repInfo.fp)
        return((EPS_INT32)EPSMP_JPG_ERR_NOT_INITIALIZED);

	fseek(g_repInfo.fp, 0, SEEK_SET);

	return epsmpJpgInit(g_repInfo.fp, g_repInfo.inBandHeight, FALSE);
#endif
}


/*******************************************|********************************************/
/* Function name:   epsmpBmp2Jpg()					                                    */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:			Type:               Description:                                    */
/* bmpFileName   	EPS_INT8*			name of bitmap image file						*/
/* jpgFileName   	EPS_INT8*			name of jpeg image file			    			*/
/*                                                                                      */
/* Return value:                                                                        */
/*      EPS_ERR_NONE					- Success                                       */
/*      SMP_ERR_FILE					- Failure                                       */
/*                                                                                      */
/* Description:                                                                         */
/*      Creates tempBmp.bmp and attempts to write bmp Header and data to file.          */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32 epsmpBmp2Jpg(const EPS_INT8* bmpFileName, const EPS_INT8* jpgFileName)
{
	EPS_INT32 ret;
	FILE *fpBmp, *fpJpg;
	EPS_UINT8*   pBmp;
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
	int row_stride;		/* physical row width in image buffer */


/*** Open Bitmap File                                                                    */
    fpBmp = fopen(bmpFileName, "rb");
	if(fpBmp == NULL){
        return (EPS_ERR_CODE)EPSMP_JPG_ERR_INVALID_FILE;
	}

	ret = epsmpBmpInit(fpBmp, 1, FALSE);
	if(EPSMP_BMP_ERR_NONE != ret){
		return ret;
	}

/*** Open Jpeg File                                                                     */
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

    fpJpg = fopen(jpgFileName, "wb");
	if(fpJpg == NULL){
        return (EPS_ERR_CODE)EPSMP_JPG_ERR_INVALID_FILE;
	}

	jpeg_stdio_dest(&cinfo, fpJpg);

	cinfo.image_width = epsmpBmpGetWidth();
	cinfo.image_height = epsmpBmpGetHeight();
	cinfo.input_components = 3;		/* # of color components per pixel */
	cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */

	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, 75, TRUE);
	jpeg_start_compress(&cinfo, TRUE);
	row_stride = epsmpBmpGetBandSize();

	while (cinfo.next_scanline < cinfo.image_height) {
		epsmpBmpGetBandData(&pBmp);
		row_pointer[0] = pBmp;
		(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	jpeg_finish_compress(&cinfo);
	fclose(fpJpg);
	jpeg_destroy_compress(&cinfo);


	epsmpBmpRelease();
	fclose(fpBmp);

	return ret;
}


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%                                             %%%%%%%%%%%%%%%%%%%%%*/
/*--------------------             Internal Processing             ---------------------*/
/*--------------------                     for                     ---------------------*/
/*--------------------                  Processing                 ---------------------*/
/*%%%%%%%%%%%%%%%%%%%%                                             %%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*******************************************|********************************************/
/*                                                                                      */
/* Function name: my_error_exit()                                                       */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* cinfo        j_common_ptr                                                            */
/*                                                                                      */
/* Return value:                                                                        */
/*      None                                                                            */
/*                                                                                      */
/* Description:                                                                         */
/*      Jpeg error                                                                      */
/*                                                                                      */
/*******************************************|********************************************/
static void my_error_exit (

        j_common_ptr    cinfo

) {

/*** Declare Variable Local to Routine                                                  */

/*** Initialize Local Variables                                                         */

/*** cinfo->err really points to a my_error_mgr struct, so coerce pointer               */
    my_error_ptr myerr = (my_error_ptr) cinfo->err;

/*** Always display the message.                                                        */
/*** We could postpone this until after returning, if we chose.                         */
    (*cinfo->err->output_message) (cinfo);

/*** Return control to the setjmp point                                                 */
    longjmp(myerr->setjmp_buffer, 1);

}


#endif /* def GCOMSW_JPEG_SUPPORT */
/*___________________________________  epsmp-jpg.c   ___________________________________*/

/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*       1         2         3         4         5         6         7         8        */
/*******************************************|********************************************/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/***** End of File *** End of File *** End of File *** End of File *** End of File ******/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
