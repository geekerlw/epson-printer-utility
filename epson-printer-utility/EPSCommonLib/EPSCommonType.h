/*________________________________  epson-typedefs.h   _________________________________*/

/*       1         2         3         4         5         6         7         8        */
/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*******************************************|********************************************/
/*
*   Copyright (c) 2009  Seiko Epson Corporation                 All rights reserved.
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
/*                           Epson Common Type Definitions                              */
/*                                                                                      */
/*******************************************|********************************************/
#ifndef EPSON_TYPEDEFS_H_
#define EPSON_TYPEDEFS_H_

	/*---------------------------------  System Includes   ---------------------------------*/
	/*******************************************|********************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>

#ifdef GCOMSW_EPSON_SLEEP
#include <sys/timeb.h>
#endif


/*** Maximum Ink Cartridge Number                                                   */
/*** -------------------------------------------------------------------------------*/
#define EPS_INK_NUM                 20


/*** Buffer size for manufacter name and model name                                 */
/*** -------------------------------------------------------------------------------*/
#define EPS_NAME_BUFFSIZE           64
#define EPS_PRNID_BUFFSIZE          128
#define EPS_ADDR_BUFFSIZE           32

/*------------------------------- Define Basic Data Types ------------------------------*/
/*******************************************|********************************************/
typedef unsigned char  EPS_UINT8;       /* unsigned  8-bit  Min: 0          Max: 255        */
typedef unsigned short EPS_UINT16;      /* unsigned 16-bit  Min: 0          Max: 65535      */
typedef unsigned int   EPS_UINT32;      /* unsigned 32-bit  Min: 0          Max: 4294967295 */
typedef char           EPS_INT8;        /*   signed  8-bit  Min: -128       Max: 127        */
typedef short          EPS_INT16;       /*   signed 16-bit  Min: -32768     Max: 32767      */
typedef int            EPS_INT32;       /*   signed 32-bit  Min:-2147483648 Max: 2147483647 */
typedef float          EPS_FLOAT;       /*    float 32-bit  Min:3.4E-38     Max: 3.4E+38    */
typedef EPS_INT32      EPS_BOOL;        /* Boolean type                                     */
typedef EPS_INT32      EPS_ERR_CODE;    /* Error code for API's and routines                */
typedef int            EPS_SOCKET;      /* socket discripter                                */

#if defined(unix)||defined(__unix)
	typedef int            EPS_FILEDSC;     /* file discripter                                  */
#else
	typedef void*          EPS_FILEDSC;     /* file discripter                                  */
#endif

											/*-----------------------------------  Definitions  ------------------------------------*/
											/*******************************************|********************************************/

											/*----------------------------------- Data Structure -----------------------------------*/
											/*******************************************|********************************************/

											/*--------------------------------- Basic State Types ----------------------------------*/
											/*******************************************|********************************************/
#ifndef NULL
#define NULL            0                   /* Value of null                            */
#endif

#ifndef TRUE
#define TRUE            1                   /* Value of "true"                          */
#endif

#ifndef FALSE
#define FALSE           0                   /* Value of "false"                         */
#endif

											/*** USB                                                                            */
											/*** -------------------------------------------------------------------------------*/
											/* sign of invalid file discripter                                                  */
#define EPS_INVALID_FILEDSC        (EPS_FILEDSC)(-1)	  


											/*** Socket                                                                         */
											/*** -------------------------------------------------------------------------------*/
											/* sign of invalid socket                                                           */
#define EPS_INVALID_SOCKET        (EPS_SOCKET)(-1)	  

											/*** Socket function retrieve value                                                 */
											/*** -------------------------------------------------------------------------------*/
#define EPS_SOCKET_SUCCESS          (0)
#define EPS_SOCKET_ERROR            (-1)	  
#define EPS_SOCKET_TIMEOUT          (-2)

											/* protocol family                                                                  */
#define EPS_PF_INET				1		/* Internetwork protocol family					*/
											/* semantics                                                                        */
#define	EPS_SOCK_STREAM			1		/* Stream socket								*/
#define	EPS_SOCK_DGRAM			2		/* Datagram socket								*/
											/* protocol                                                                         */
#define EPS_PROTOCOL_TCP		1		/* TCP protocol									*/
#define EPS_PROTOCOL_UDP		2		/* UDP protocol									*/
											/* shutdown method                                                                  */
#define EPS_SHUTDOWN_SEND		1		/* Shutdown dir: send							*/
#define EPS_SHUTDOWN_RECV		2		/* Shutdown dir: receive						*/
#define EPS_SHUTDOWN_BOTH		3		/* Shutdown dir: both							*/

/*-----------------------  Error Return Value of ESC/P-R Lib API  ----------------------*/
/*******************************************|********************************************/
enum EPS_RUN_TIME_ERROR {
    /*** No Error                                                                       */
    /*** -------------------------------------------------------------------------------*/
    EPS_ERR_NONE                            =     0,    /* Routine / Action completed successfully          */

    /*** Global Error                                                                   */
    /*** -------------------------------------------------------------------------------*/
    EPS_ERR_OPR_FAIL                        = -1000,    /* Generic / Internal Error                         */
    EPS_ERR_MEMORY_ALLOCATION               = -1001,    /* Memory Allocation Error                          */
	EPS_ERR_PRINTER_ERR_OCCUR               = -1003,    /* The error occurred by Printer                    */

	EPS_ERR_PROTOCOL_NOT_SUPPORTED          = -1010,    /* Unsupported function Error (protocol)            */
    EPS_ERR_NEED_BIDIRECT                   = -1011,    /* Need Bi-Directional Communication Mode           */
    EPS_ERR_INVALID_CALL                    = -1012,    /* Invalid function call                            */
    EPS_ERR_INVALID_VERSION                 = -1013,    /* Invalid structure version                        */
	EPS_ERR_INVALID_DATATYPE                = -1014,    /* Invalid data type                                */
	EPS_ERR_LANGUAGE_NOT_SUPPORTED          = -1015,    /* Unsupported function Error (language)            */

    /*** Library Status Error                                                           */
    /*** -------------------------------------------------------------------------------*/
	EPS_ERR_LIB_INTIALIZED                  = -1050,    /* Lib already initialized                          */
    EPS_ERR_LIB_NOT_INITIALIZED             = -1051,    /* Lib not initialized                              */
    EPS_ERR_JOB_NOT_INITIALIZED             = -1052,    /* Start page before start job                      */
    EPS_ERR_JOB_NOT_CLOSED                  = -1053,    /* Started new job before closing open job          */
    EPS_ERR_PAGE_NOT_INITIALIZED            = -1054,    /* Close page before start page                     */
    EPS_ERR_PAGE_NOT_CLOSED                 = -1055,    /* Started new page before closing open page        */

	/*** I/O Errors                                                                     */
    /*** -------------------------------------------------------------------------------*/
    EPS_ERR_COMM_ERROR                      = -1100,    /* Communication Error                              */
    EPS_ERR_NOT_OPEN_IO                     = -1101,    /* Could not open      I/O port/channel             */
    EPS_ERR_NOT_CLOSE_IO                    = -1102,    /* Could not close     I/O port/channel             */
    EPS_ERR_2ND_OPEN_IO                     = -1103,    /* Second Open I/O port                             */

    /*** epsInitDriver() Error                                                          */
    /*** -------------------------------------------------------------------------------*/
    EPS_ERR_INV_ARG_COMMMODE                = -1200,    /* Invalid argument "commMode"                      */
    EPS_ERR_INV_FNCP_NULL                   = -1201,    /* Invalid function pointer structure               */
	EPS_ERR_INV_FNCP_FINDCALLBACK           = -1202,    /* Invalid function pointer "findCallback"          */
    EPS_ERR_INV_FNCP_MEMALLOC               = -1203,    /* Invalid function pointer "memAlloc"              */
    EPS_ERR_INV_FNCP_MEMFREE                = -1204,    /* Invalid function pointer "memFree"               */
    EPS_ERR_INV_FNCP_SLEEP                  = -1205,    /* Invalid function pointer "sleep"                 */
    EPS_ERR_INV_FNCP_GETTIME                = -1206,    /* Invalid function pointer "getTime" and "lockSync" */

	EPS_ERR_INV_FNCP_OPENPORTAL             = -1211,    /* Invalid function pointer "openPortal"            */
    EPS_ERR_INV_FNCP_CLOSEPORTAL            = -1212,    /* Invalid function pointer "closePortal"           */
    EPS_ERR_INV_FNCP_READPORTAL             = -1213,    /* Invalid function pointer "readPortal"            */
    EPS_ERR_INV_FNCP_WRITEPORTAL            = -1214,    /* Invalid function pointer "writePortal"           */
    EPS_ERR_INV_FNCP_FINDFIRST			    = -1215,    /* Invalid function pointer "findFirst"             */
    EPS_ERR_INV_FNCP_FINDNEXT		        = -1216,    /* Invalid function pointer "findNext"              */
    EPS_ERR_INV_FNCP_FINDCLOSE	            = -1217,    /* Invalid function pointer "findClose"             */
    EPS_ERR_INV_FNCP_GETDEVICEID	        = -1218,    /* Invalid function pointer "findClose"             */
    EPS_ERR_INV_FNCP_SOFTRESET	            = -1219,    /* Invalid function pointer "findClose"             */

    EPS_ERR_INV_FNCP_NETSOCKET              = -1221,    /* Invalid function pointer "socket"                */
    EPS_ERR_INV_FNCP_NETCLOSE               = -1222,    /* Invalid function pointer "close"                 */
    EPS_ERR_INV_FNCP_NETCONNECT             = -1223,    /* Invalid function pointer "connect"               */
    EPS_ERR_INV_FNCP_NETSHUTDOWN            = -1224,    /* Invalid function pointer "shutdown"              */
    EPS_ERR_INV_FNCP_NETBIND                = -1225,    /* Invalid function pointer "bind"                  */
    EPS_ERR_INV_FNCP_NETLISTEN              = -1226,    /* Invalid function pointer "listen"                */
    EPS_ERR_INV_FNCP_NETACCEPT              = -1227,    /* Invalid function pointer "accept"                */
    EPS_ERR_INV_FNCP_NETSEND                = -1228,    /* Invalid function pointer "send"                  */
    EPS_ERR_INV_FNCP_NETSENDTO              = -1229,    /* Invalid function pointer "sendTo"                */
    EPS_ERR_INV_FNCP_NETRECEIVE             = -1230,    /* Invalid function pointer "receive"               */
    EPS_ERR_INV_FNCP_NETRECEIVEFROM         = -1231,    /* Invalid function pointer "receiveFrom"           */
    EPS_ERR_INV_FNCP_NETGETSOCKNAME         = -1232,    /* Invalid function pointer "getsockname"           */
    EPS_ERR_INV_FNCP_NETSETMULTITTL         = -1233,    /* Invalid function pointer "setMulticastMember"    */
    EPS_ERR_INV_FNCP_NETSETBROADCAST        = -1234,    /* Invalid function pointer "setBroadcast"          */

    /*** epsFindPrinter(), epsProbePrinter() Error                                      */
    /*** -------------------------------------------------------------------------------*/
    EPS_ERR_PRINTER_NOT_FOUND               = -1300,    /* printer not found                                */
	EPS_ERR_INV_ARG_PROBEINFO               = -1301,    /* Invalid argument probe infomation                */
	EPS_ERR_INV_ARG_UNK_METHOD              = -1302,    /* Invalid value "method"                           */
    EPS_ERR_INV_ARG_PRINTER_ID              = -1303,    /* Invalid format "identify"                        */
    EPS_ERR_INV_ARG_PRINTER_ADDR            = -1304,    /* Invalid format "address"                         */
    EPS_ERR_FIND_NOT_STARTED                = -1305,    /* Cancel find before start find                    */
    EPS_ERR_PRINTER_NOT_USEFUL              = -1306,    /* not useful printer found                         */

	/*** epsSetPrinter() Error                                                          */
    /*** -------------------------------------------------------------------------------*/
	EPS_ERR_INV_ARG_PRINTER                 = -1350,    /* Invalid Argument "printer"                       */
	EPS_ERR_PRINTER_NOT_SET                 = -1351,    /* Target printer is not specified                  */
	EPS_ERR_INV_PRINT_LANGUAGE              = -1352,    /* Invalid Argument "printer.language"              */

    /*** Page Attribution Error (epsStartJob(), epsGetPrintableArea())                 */
    /*** -------------------------------------------------------------------------------*/
    EPS_ERR_INV_MEDIA_SIZE                  = -1400,    /* Invalid Media Size                               */
    EPS_ERR_INV_MEDIA_TYPE                  = -1401,    /* Invalid Media Type                               */
    EPS_ERR_INV_BORDER_MODE                 = -1402,    /* Invalid Border Mode                              */
    EPS_ERR_INV_PRINT_QUALITY               = -1403,    /* Invalid Print Quality                            */
    EPS_ERR_INV_COLOR_MODE                  = -1404,    /* Invalid Color Mode                               */
    EPS_ERR_INV_INPUT_RESOLUTION            = -1405,    /* Invalid Input Resolution                         */
    EPS_ERR_INV_PRINT_DIRECTION             = -1406,    /* Invalid Print Direction                          */
    EPS_ERR_INV_COLOR_PLANE                 = -1407,    /* Invalid Color Plane                              */
    EPS_ERR_INV_PALETTE_SIZE                = -1408,    /* Invalid Palette Size                             */
    EPS_ERR_INV_PALETTE_DATA                = -1409,    /* Invalid Palette Data                             */
    EPS_ERR_INV_BRIGHTNESS                  = -1410,    /* Invalid Brightness                               */
    EPS_ERR_INV_CONTRAST                    = -1411,    /* Invalid Contrast                                 */
    EPS_ERR_INV_SATURATION                  = -1412,    /* Invalid Saturation                               */
    /*EPS_ERR_INV_MEDIAOPTION                 = -1413,     Invalid mediaOption                              */
    EPS_ERR_INV_TOP_MARGIN                  = -1420,    /* Invalid Top Magirn                               */
    EPS_ERR_INV_LEFT_MARGIN                 = -1421,    /* Invalid Left Margin                              */
    EPS_ERR_INV_BOTTOM_MARGIN               = -1422,    /* Invalid Bottom Margin                            */
    EPS_ERR_INV_RIGHT_MARGIN                = -1423,    /* Invalid Right Margin                             */
    EPS_ERR_MARGIN_OVER_PRINTABLE_WIDTH     = -1424,    /* Invalid Margin Setting (Width)                   */
    EPS_ERR_MARGIN_OVER_PRINTABLE_HEIGHT    = -1425,    /* Invalid Margin Setting (Height)                  */
    EPS_ERR_INV_PAPER_SOURCE                = -1426,    /* Invalid Paper source                             */
    EPS_ERR_INV_DUPLEX                      = -1427,    /* Invalid duplex                                   */
    EPS_ERR_INV_FEED_DIRECTION              = -1428,    /* Invalid feed direction                           */

	/*EPS_ERR_INV_APF_FLT                     = -1430,     Invalid APF Filter                               */
	EPS_ERR_INV_APF_ACT                     = -1431,    /* Invalid APF Auto Corretion                       */
	EPS_ERR_INV_APF_SHP                     = -1432,    /* Invalid APF Sharpness                            */
	EPS_ERR_INV_APF_RDE                     = -1433,    /* Invalid APF Redeye                               */
	EPS_ERR_INV_CD_INDIM                    = -1434,    /* Invalid CD Inside Diameter                       */
	EPS_ERR_INV_CD_OUTDIM                   = -1435,    /* Invalid CD Outside Diameter                      */
	EPS_ERR_INV_CMDTYPE                     = -1436,    /* Invalid Command Type                             */

    EPS_ERR_INV_ARG_JOB_ATTRIB              = -1450,    /* Invalid argument "jobAttr"                       */
	EPS_ERR_INV_ARG_PAGE_ATTRIB             = -1451,    /* Invalid argument "thisPage"                      */

	/*** epsStartJob() / epsStartPage() Error                                           */
    /*** -------------------------------------------------------------------------------*/
    /*EPS_ERR_CANNOT_PRINT                    = -1500,*//* Cannot start printing                            */

	/*** epsPrintBand() Error                                                           */
    /*** -------------------------------------------------------------------------------*/
    EPS_ERR_INV_ARG_DATA                    = -1600,    /* Invalid argument "data"                          */
    EPS_ERR_INV_ARG_WIDTH_PIXELS            = -1601,    /* Invalid argument "widthPixels"                   */
    /* EPS_ERR_INV_ARG_BYTES_PER_LINE          = -1602,     Invalid argument "bytesPerLine"                 */
    EPS_ERR_INV_ARG_HEIGHT_PIXELS           = -1603,    /* Invalid argument "heightPixels"                  */
	EPS_ERR_INV_ARG_DATASIZE                = -1610,    /* Invalid argument "dataSize"                      */

    /*** epsCancelJob() Error                                                           */
    /*** -------------------------------------------------------------------------------*/
    EPS_ERR_CAN_NOT_RESET                   = -1650,    /* Failed to reset printer                          */

    /*** epsGetStatus() / epsGetInkInfo() Error                                         */
    /*** -------------------------------------------------------------------------------*/
    EPS_ERR_INV_ARG_STATUS                  = -1700,    /* Invalid argument "status"                        */
    EPS_ERR_INV_ARG_INKINFO                 = -1710,    /* Invalid argument "inkInfo"                       */

	/*** epsGetSupportedMedia() Error                                                   */
    /*** -------------------------------------------------------------------------------*/
    EPS_ERR_INV_ARG_SUPPORTED_MEDIA         = -1750,    /* Invalid Argument "supportedMedia"                */

    /*** epsGetPrintableArea() Error                                                    */
    /*** -------------------------------------------------------------------------------*/
    EPS_ERR_INV_ARG_PRINTABLE_WIDTH         = -1800,    /* Invalid argument "printableWidth"                */
    EPS_ERR_INV_ARG_PRINTABLE_HEIGHT        = -1801,    /* Invalid argument "printableHeight"               */

    /*** epsSetAdditionalData() Error                                                   */
    /*** -------------------------------------------------------------------------------*/
    EPS_ERR_INV_ARG_ADDDATA                 = -2001,    /* Invalid argument "data"                          */
    EPS_ERR_INV_ARG_QRSOURCE                = -2002,    /* Invalid argument "data.qrcode.source"            */
    EPS_ERR_INV_ARG_QRXPOS	                = -2003,    /* Invalid argument "data.qrcode.xPos"              */
    EPS_ERR_INV_ARG_QRYPOS	                = -2004,    /* Invalid argument "data.qrcode.yPos"              */
    EPS_ERR_QRSOURCE_TOO_LAGE               = -2005     /* source is too lage to convert QR code            */
};

/*---------------------------  Definition of Printer Status  ---------------------------*/
/*******************************************|********************************************/

/*** Printer Status                                                                 */
/*** -------------------------------------------------------------------------------*/
enum EPS_PRINTER_STATUS {
	EPS_PRNST_IDLE = 0,	/* Idle (Enable Start Job)      */
	EPS_PRNST_PRINTING,    /* Printing                     */
						   /* EPS_PRNST_OFFLINE						   ,	Offline                      */
						   EPS_PRNST_BUSY,    /* Busy (Disable Start Job)     */
						   EPS_PRNST_CANCELLING,	/* Cancellation processing      */
						   EPS_PRNST_ERROR								        /* Printer has an error         */
};

    /*** Printer Error                                                                  */
    /*** -------------------------------------------------------------------------------*/
enum EPS_PRINTER_ERROR {
    EPS_PRNERR_NOERROR                          = 0,
    EPS_PRNERR_GENERAL                             ,
    EPS_PRNERR_FATAL                               ,
    EPS_PRNERR_INTERFACE                           ,
    EPS_PRNERR_COVEROPEN                           ,
/*  EPS_PRNERR_LEVERPOSITION                       ,*/  /* Not supported by 2006 Model  */
    EPS_PRNERR_PAPERJAM                            ,
    EPS_PRNERR_INKOUT                              ,
    EPS_PRNERR_PAPEROUT                            ,
/*  EPS_PRNERR_INITIALIZESETTING                   ,*/  /* Not supported by 2006 Model  */
/*  EPS_PRNERR_UNKNOWN                             ,*/  /* Not supported by 2006 Model  */
/*  EPS_PRNERR_PAPERCHANGE_UNCOMP                  ,*/  /* Not supported by 2006 Model  */
/*  EPS_PRNERR_PAPERSIZE                           ,*/  /* Not supported by 2006 Model  */  
/*  EPS_PRNERR_RIBBONJAM                           ,*/  /* Not supported by 2006 Model  */
    EPS_PRNERR_SIZE_TYPE_PATH                      ,
/*  EPS_PRNERR_PAPERTHICKLEVER                     ,*/  /* Not supported by 2006 Model  */
/*  EPS_PRNERR_PAPERFEED                           ,*/  /* Not supported by 2006 Model  */
/*  EPS_PRNERR_SIMMCOPY                            ,*/  /* Not supported by 2006 Model  */
	EPS_PRNERR_SERVICEREQ                          ,    /* EPS_PRNERR_INKOVERFLOW1      */
/*  EPS_PRNERR_WAITTEAROFFRETURN                   ,*/  /* Not supported by 2006 Model  */
    EPS_PRNERR_DOUBLEFEED                          ,
/*  EPS_PRNERR_HEADHOT                             ,*/  /* Not supported by 2006 Model  */
/*  EPS_PRNERR_PAPERCUTMIS                         ,*/  /* Not supported by 2006 Model  */
/*  EPS_PRNERR_HOLDLEVERRELEASE                    ,*/  /* Not supported by 2006 Model  */
/*  EPS_PRNERR_NOT_CLEANING                        ,*/  /* Not supported by 2006 Model  */
/*  EPS_PRNERR_PAPERCONFIG                         ,*/  /* Not supported by 2006 Model  */
/*  EPS_PRNERR_PAPERSLANT                          ,*/  /* Not supported by 2006 Model  */
/*  EPS_PRNERR_CLEANINGNUMOVER                     ,*/  /* Not supported by 2006 Model  */
    EPS_PRNERR_INKCOVEROPEN                        ,
/*  EPS_PRNERR_LFP_INKCARTRIDGE                    ,*/  /* Not supported by 2006 Model  */
/*  EPS_PRNERR_CUTTER                              ,*/  /* Not supported by 2006 Model  */
/*  EPS_PRNERR_CUTTERJAM                           ,*/  /* Not supported by 2006 Model  */
/*  EPS_PRNERR_INKCOLOR                            ,*/  /* Not supported by 2006 Model  */
/*  EPS_PRNERR_CUTTERCOVEROPEN                     ,*/  /* Not supported by 2006 Model  */
/*  EPS_PRNERR_LFP_INKLEVERRELEASE                 ,*/  /* Not supported by 2006 Model  */
/*  EPS_PRNERR_LFP_NOMAINTENANCETANK1              ,*/  /* Not supported by 2006 Model  */
/*  EPS_PRNERR_CARTRIDGECOMBINATION                ,*/  /* Not supported by 2006 Model  */
/*  EPS_PRNERR_LFP_COMMAND                         ,*/  /* Not supported by 2006 Model  */
/*  EPS_PRNERR_LEARCOVEROPEN                       ,*/  /* Not supported by 2006 Model  */
/*  EPS_PRNERR_MULTICENSORGAIN                     ,*/  /* Not supported by 2006 Model  */
/*  EPS_PRNERR_NOT_AUTOADJUST                      ,*/  /* Not supported by 2006 Model  */
/*  EPS_PRNERR_FAILCLEANING                        ,*/  /* Not supported by 2006 Model  */
    EPS_PRNERR_NOTRAY                              ,
    EPS_PRNERR_CARDLOADING                         ,
    EPS_PRNERR_CARTRIDGEOVERFLOW                   ,
/*  EPS_PRNERR_LFP_NOMAINTENANCETANK2              ,*/  /* Not supported by 2006 Model  */
/*  EPS_PRNERR_INKOVERFLOW2                        ,*/  /* Not supported by 2006 Model  */
    EPS_PRNERR_BATTERYVOLTAGE                      ,
    EPS_PRNERR_BATTERYTEMPERATURE                  ,
    EPS_PRNERR_BATTERYEMPTY                        ,
    EPS_PRNERR_SHUTOFF                             ,
    EPS_PRNERR_NOT_INITIALFILL                     ,
    EPS_PRNERR_PRINTPACKEND                        ,
/*  EPS_PRNERR_ABNORMALHEAT                        ,*/  /* Not supported by 2006 Model  */
	EPS_PRNERR_SCANNEROPEN						   ,
    EPS_PRNERR_CDRGUIDEOPEN                        ,

	/* append for 2008 Model  */
	EPS_PRNERR_CDDVDCONFIG                         ,
	EPS_PRNERR_CDREXIST_MAINTE                     ,

	/* append for 2012 Model  */
	EPS_PRNERR_FEEDERCLOSE							,

	/* append for 2013 Model  */
	EPS_PRNERR_3DMEDIA_FACE                         ,   /* Improper insert face at 3D printing media  */
	EPS_PRNERR_3DMEDIA_DIRECTION                    ,   /* Improper insert direction at 3D printing media  */
	EPS_PRNERR_MANUALFEED_SET_PAPER					,   /* Set a paper into Manual Feed */
	EPS_PRNERR_MANUALFEED_SET_PAPER_NOLCD			,
	EPS_PRNERR_MANUALFEED_FAILED					,   /* Non feed at Manual Feed */
	EPS_PRNERR_MANUALFEED_FAILED_NOLCD				,
	EPS_PRNERR_MANUALFEED_EXCESSIVE					,   /* Excessive inserting paper on Manual Feed */
	EPS_PRNERR_MANUALFEED_EXCESSIVE_NOLCD			,

	/* Status Error                                                                     */
    EPS_PRNERR_BUSY								= 100,
    EPS_PRNERR_FACTORY								,
    /* Communication Error                                                              */
    EPS_PRNERR_COMM									,
	/* Ink Error                                                                        */
    EPS_PRNERR_CEMPTY								,
    EPS_PRNERR_CFAIL								,
    /* Printer Condition                                                                */
    EPS_PRNERR_TRAYCLOSE							,							
	EPS_PRNERR_CDGUIDECLOSE                         ,   /* CDR guide close              */ 
/*    EPS_PRNERR_OVERHEAT								   OVERHEAT is not an error     */
    EPS_PRNERR_JPG_LIMIT	                        ,   /* Jpeg print data size limit   */	
	EPS_PRNERR_DISABEL_CLEANING                     ,   /* can not start Head Cleaning  */
};

/*** Ink Cartridge Error                                                            */
/*** -------------------------------------------------------------------------------*/
#define EPS_INK_NOTPRESENT                       (-1)
#define EPS_INK_FAIL                             (-2)
#define EPS_INK_NOTAVAIL                         (-3)
#define EPS_INK_NOREAD                           (-4)

/*** Ink Cartridge Status                                                           */
/*** -------------------------------------------------------------------------------*/
#define EPS_INK_ST_NORMAL                        (0)
#define EPS_INK_ST_LOW                           (1)
#define EPS_INK_ST_END                           (2)
#define EPS_INK_ST_NOTPRESENT                    EPS_INK_NOTPRESENT
#define EPS_INK_ST_FAIL                          EPS_INK_FAIL
#define EPS_INK_ST_NOTAVAIL                      EPS_INK_NOTAVAIL
#define EPS_INK_ST_NOREAD                        EPS_INK_NOREAD

typedef struct _tagEPS_PAPER_COUNT_ {
	EPS_INT32	color;
	EPS_INT32	monochrome;
	EPS_INT32	blank;
	EPS_INT32	adf;
	EPS_INT32	color_borderless;
	EPS_INT32	monochrome_borderless;
} EPS_PAPER_COUNT;


/*** Printer Status                                                                 */
/*** -------------------------------------------------------------------------------*/
typedef struct _tagEPS_STATUS_ {
	EPS_INT32           printerStatus;
	EPS_INT32           errorCode;
	EPS_BOOL			jobContinue;
	EPS_PAPER_COUNT 	paper_count;
} EPS_STATUS;

/*** Ink infomation                                                                 */
/*** -------------------------------------------------------------------------------*/
typedef struct _tagEPS_INK_INFO_ {
	EPS_INT32           number;
	EPS_INT32           colors[EPS_INK_NUM];
	EPS_INT32           remaining[EPS_INK_NUM];
	EPS_INT32           status[EPS_INK_NUM];
	EPS_BOOL			showInkInfo;
	EPS_BOOL			showInkLow;
} EPS_INK_INFO;

/*** Printer Infomation                                                             */
/*** -------------------------------------------------------------------------------*/
typedef struct _tagEPS_PRINTER_ {
	EPS_UINT32			supportFunc;
	EPS_INT8            manufacturerName[EPS_NAME_BUFFSIZE];
	EPS_INT8            modelName[EPS_NAME_BUFFSIZE];
	EPS_INT8			friendlyName[EPS_NAME_BUFFSIZE];
	EPS_INT8            location[EPS_ADDR_BUFFSIZE];
	EPS_INT8		    printerID[EPS_PRNID_BUFFSIZE];
	/* ver 4.2 */
	EPS_INT32			protocol;			    /* Communication protocol               */
												/* ver 5.0 */
	EPS_UINT32			language;				/* print language */
	EPS_INT8            macAddress[EPS_ADDR_BUFFSIZE];
	/* ver 5.2 */
	EPS_INT8            serialNo[EPS_ADDR_BUFFSIZE];
} EPS_PRINTER;

/*** Probe printer parameter                                                        */
/*** -------------------------------------------------------------------------------*/
typedef struct tagEPS_PROBE_ {
	EPS_INT32   version;                    /* this version                             */
	EPS_INT32   method;
	EPS_UINT32  timeout;
	union {
		EPS_INT8	identify[EPS_PRNID_BUFFSIZE];
		struct {
			EPS_INT32   protocol;
			EPS_INT8	address[EPS_ADDR_BUFFSIZE];
		} addr;
	} dsc;
}EPS_PROBE;

typedef struct _tagAPP_PRINTER_NODE_ {
	EPS_PRINTER*					printer;
	struct _tagAPP_PRINTER_NODE_* 	next;
}APP_PRINTER_NODE;

typedef struct _tagAPP_PRINTER_LIST_ {
	EPS_INT32			num;	/* printer count of espFindPrinter result   */
	APP_PRINTER_NODE*	root;	/* printer list of espFindPrinter result    */
}APP_PRINTER_LIST;

typedef struct tagAPP_IPID_SEARCH_ATTRIBS {
	volatile EPS_BOOL		searchFinished;
	volatile EPS_ERR_CODE	retState;
	EPS_UINT32				timeout;
	EPS_PROBE				probe;
	APP_PRINTER_LIST		appPrinterList;
} APP_SEARCH_ATTRIBS;

/*** print protocol                                                                 */
/*** -------------------------------------------------------------------------------*/
#define EPS_PROTOCOL_INVALID	0x00000000	/* Invalid                                  */
#define EPS_PROTOCOL_USB		0x00000010  /* USB                                      */
/*#define EPS_PROTOCOL_UPNP		0x00000020     UPnP                                     */
#define EPS_PROTOCOL_LPR		0x00000040  /* LPR                                      */
#define EPS_PROTOCOL_RAW		0x00000080  /* RAW Port                                 */
#define EPS_PROTOCOL_NET        (EPS_PROTOCOL_LPR | EPS_PROTOCOL_RAW)	/* All Network  */
#define EPS_PROTOCOL_ALL        (EPS_PROTOCOL_USB | EPS_PROTOCOL_NET)	/* All          */

#define EPS_COMM_USB_UNID   (EPS_COMM_UNID | EPS_PROTOCOL_USB )  /* USB Uni-Directional Communication  */
#define EPS_COMM_USB_BID    (EPS_COMM_BID  | EPS_PROTOCOL_USB )  /* USB Bi-Directional Communication   */
#define EPS_COMM_LPR_UNID   (EPS_COMM_UNID | EPS_PROTOCOL_LPR )  /* LPR Uni-Directional Communication  */
#define EPS_COMM_LPR_BID    (EPS_COMM_BID  | EPS_PROTOCOL_LPR )  /* LPR Bi-Directional Communication   */
#define EPS_COMM_RAW_UNID   (EPS_COMM_UNID | EPS_PROTOCOL_RAW )  /* RAW Uni-Directional Communication  */
#define EPS_COMM_RAW_BID    (EPS_COMM_BID  | EPS_PROTOCOL_RAW )  /* RAW Bi-Directional Communication   */
#define EPS_COMM_NET_UNID   (EPS_COMM_UNID | EPS_PROTOCOL_NET )  /* Net Uni-Directional Communication  */
#define EPS_COMM_NET_BID    (EPS_COMM_BID  | EPS_PROTOCOL_NET )  /* Net Bi-Directional Communication   */
#define EPS_COMM_ALL_UNID   (EPS_COMM_USB_UNID | EPS_COMM_NET_UNID)  /* USB & Net Uni-Directional Communication */
#define EPS_COMM_ALL_BID    (EPS_COMM_USB_BID  | EPS_COMM_NET_BID)  /* USB & Net Bi-Directional Communication */

/*** print language                                                                  */
/*** -------------------------------------------------------------------------------*/
#define EPS_LANG_UNKNOWN			(0)			
#define EPS_LANG_ESCPR				(1)			/* ESC/P-R */
#define EPS_LANG_ESCPAGE			(2)			/* ESC/Page(Mono) */
#define EPS_LANG_ESCPAGE_COLOR		(3)			/* ESC/Page-Color */

/*** Ink Color                                                                      */
/*** -------------------------------------------------------------------------------*/
enum EPS_COLOR {
	EPS_COLOR_BLACK,
	EPS_COLOR_CYAN,
	EPS_COLOR_MAGENTA,
	EPS_COLOR_YELLOW,
	EPS_COLOR_LIGHTCYAN,
	EPS_COLOR_LIGHTMAGENTA,
	EPS_COLOR_LIGHTYELLOW,
	EPS_COLOR_DARKYELLOW,
	EPS_COLOR_LIGHTBLACK,
	EPS_COLOR_RED,
	EPS_COLOR_VIOLET,
	EPS_COLOR_MATTEBLACK,
	EPS_COLOR_LIGHTLIGHTBLACK,
	EPS_COLOR_PHOTOBLACK,
	EPS_COLOR_CLEAR,
	EPS_COLOR_GRAY,
	EPS_COLOR_UNKNOWN,

	/* add ver5.0*/
	EPS_COLOR_BLACK2,
	EPS_COLOR_ORANGE,
	EPS_COLOR_GREEN,
	EPS_COLOR_WHITE,
	EPS_COLOR_CLEAN
};

#endif /* for EPSON_TYPEDEFS_H_                                                         */

/*________________________________  epson-typedefs.h   _________________________________*/

/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*       1         2         3         4         5         6         7         8        */
/*******************************************|********************************************/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/***** End of File *** End of File *** End of File *** End of File *** End of File ******/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
