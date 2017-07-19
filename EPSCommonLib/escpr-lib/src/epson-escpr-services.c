/*____________________________   epson-escpr-services.c   ______________________________*/

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
/*                      Epson ESC/P-R Lib Internal Service Routines                     */
/*                                                                                      */
/*                                Public Function Calls                                 */
/*                              --------------------------                              */
/*              EPS_ERR_CODE serAnalyzeStatus       (Status, StatusInfo         );      */
/*              void         serDelayThread         (Milliseconds               );      */
/*              EPS_ERR_CODE serSleep               (Milliseconds               );      */
/*              EPS_INT32    serGetInkError         (pStatInfo,  pNotify        );      */
/*              EPS_INT32    _SP_ChangeSpec_UpdatePMReply(printer,  orgPmString	);      */
/*              EPS_ERR_CODE _SP_ChangeSpec_DraftOnly(printer,  jobAtter        );      */
/*                                                                                      */
/*******************************************|********************************************/

/*------------------------------------  Includes   -------------------------------------*/
/*******************************************|********************************************/
#include "epson-escpr-err.h"
#include "epson-escpr-mem.h"
#include "epson-escpr-media.h"
#include "epson-escpr-services.h"
#include "err.h"
#ifdef GCOMSW_CMD_ESCPAGE_S
#include "epson-escpage-s.h"
#endif
#ifdef GCOMSW_CMD_PCL
#include "epson-pcl.h"
#endif
/*------------------------------------  Definition   -----------------------------------*/
/*******************************************|********************************************/
    /*** Index of printing quality                                                      */
    /*** -------------------------------------------------------------------------------*/
#define Q_DRAFT                         0
#define Q_NORMAL                        1
#define Q_HIGH                          2

#ifdef EPS_LOG_MODULE_SER
#define EPS_LOG_MODULE	EPS_LOG_MODULE_SER
extern EPS_CMN_FUNC    epsCmnFnc;
#else
#define EPS_LOG_MODULE	0
#endif


/*----------------------------  ESC/P-R Lib Global Variables  --------------------------*/
/*******************************************|********************************************/

/*------------------------------  Local Global Variables  ------------------------------*/
/*******************************************|********************************************/
typedef	struct _tagSP_OBSERVER
{
	const EPS_PRINTER_INN	 *printer;	         /* current printer                     */
    EPS_UINT8                colorPlane;         /* Image color plane                   */
}SP_OBSERVER;

static SP_OBSERVER	g_observer = {0};

/*---------------------------  Special PM String table   -------------------------------*/
/*******************************************|********************************************/
typedef	struct _tagSP_PM_STRINGS
{
	EPS_UINT32		 id;	/* ID */
	const EPS_UINT8* res;	/* PM String */
	EPS_UINT32		 len;	/* length */
}SP_PM_STRINGS;

const EPS_UINT8 spm_E300[] = {0x40, 0x42, 0x44, 0x43, 0x20, 0x50, 0x4D, 0x0D, 0x0A, 0x53, 0x0F, 0x54, 0x26, 0x82, 0x2F, 0x54,
							0x0B, 0x82, 0x2F, 0x54, 0x2B, 0x82, 0x2F, 0x2F, 0x53, 0x0A, 0x54, 0x26, 0x82, 0x2F, 0x54, 0x0B,
							0x82, 0x2F, 0x54, 0x2B, 0x82, 0x2F, 0x2F, 0x53, 0x10, 0x54, 0x0B, 0x82, 0x2F, 0x54, 0x08, 0x02, 
							0x2F, 0x2F, 0x53, 0x23, 0x54, 0x0B, 0x82, 0x2F, 0x2F, 0x0D, 0x0A};
const EPS_UINT8 spm_E500[] = {0x40, 0x42, 0x44, 0x43, 0x20, 0x50, 0x4D, 0x0D, 0x0A, 0x53, 0x0F, 0x54, 0x26, 0x82, 0x2F, 0x54,
							0x0B, 0x82, 0x2F, 0x54, 0x2B, 0x82, 0x2F, 0x2F, 0x53, 0x0A, 0x54, 0x26, 0x82, 0x2F, 0x54, 0x0B,
							0x82, 0x2F, 0x54, 0x2B, 0x82, 0x2F, 0x2F, 0x53, 0x2B, 0x54, 0x0B, 0x82, 0x2F, 0x2F, 0x53, 0x10,
							0x54, 0x0B, 0x82, 0x2F, 0x54, 0x08, 0x02, 0x2F, 0x2F, 0x53, 0x23, 0x54, 0x0B, 0x82, 0x2F, 0x2F,
							0x0D, 0x0A};
const EPS_UINT8 spm_PM200[] = {0x40, 0x42, 0x44, 0x43, 0x20, 0x50, 0x4D, 0x0D, 0x0A, 0x53, 0x0A, 0x54, 0x26, 0x82, 0x2F, 0x54,
                            0x0B, 0x82, 0x2F, 0x54, 0x2B, 0x82, 0x2F, 0x2F, 0x53, 0x10, 0x54, 0x08, 0x02, 0x2F, 0x2F, 0x0D, 
							0x0A};
const EPS_UINT8 spm_PM240[] = {0x40, 0x42, 0x44, 0x43, 0x20, 0x50, 0x4D, 0x0D, 0x0A, 0x53, 0x0A, 0x54, 0x26, 0x82, 0x2F, 0x54, 
                            0x0B, 0x82, 0x2F, 0x54, 0x2B, 0x82, 0x2F, 0x2F, 0x53, 0x2B, 0x54, 0x0B, 0x82, 0x2F, 0x2F, 0x53, 
							0x10, 0x54, 0x08, 0x42, 0x2F, 0x2F, 0x0D, 0x0A};

const SP_PM_STRINGS	spPMStrTbl[] = 
{
	{ EPS_PMS_E300,	 spm_E300,  sizeof(spm_E300)	},
	{ EPS_PMS_E500,  spm_E500,  sizeof(spm_E500)	},
	{ EPS_PMS_PM200, spm_PM200, sizeof(spm_PM200)	},
	{ EPS_PMS_PM240, spm_PM240, sizeof(spm_PM240)	}
};

const EPS_UINT32 EPS_SPM_STRINGS = sizeof(spPMStrTbl) / sizeof(SP_PM_STRINGS);

/*--------------------------------  Local Functions   ----------------------------------*/
/*******************************************|********************************************/
static EPS_INT32    _SP_LoadPMString                (EPS_UINT32, EPS_UINT8*, EPS_UINT32 );
static EPS_INT32    _pmFindSfield                   (EPS_UINT8, EPS_UINT8*, EPS_UINT8**, EPS_UINT8**);
static EPS_UINT8*   _pmScanTfield                   (EPS_UINT8, EPS_UINT8*              );
static EPS_INT16    _pmAppendTfield                 (EPS_UINT8*, EPS_UINT8*             );
static void         _pmValidateRemoveDelimiter      (EPS_UINT8*, EPS_UINT8*, EPS_INT32  );
static EPS_INT16    _pmValidateRemoveUnknownSfield  (EPS_UINT8*, EPS_UINT8*             );
static void         _pmCorrectUnknownTfield         (EPS_UINT8*, EPS_UINT8*             );
static void         _pmCorrectDupulicatedFields     (EPS_UINT8*, EPS_UINT8*             );
static void         _pmAdjustQuality                (EPS_UINT8*                         );

/*-----------------------------------  Debug Dump  -------------------------------------*/
/*******************************************|********************************************/
#define _DEBUG_BIN_STATUS_          0       /* 0: OFF    1: ON                          */

#if _DEBUG_BIN_STATUS_ || _VALIDATE_SUPPORTED_MEDIA_DATA_
#define SerDebugPrintf(a)  EPS_DBGPRINT( a )
#else
#define SerDebugPrintf(a)
#endif

/*------------------------------------  Debug Dump   -----------------------------------*/
/*******************************************|********************************************/
    /*** ALL Debug Dump Switch for <epson-escpr-service.c>                              */
    /*** -------------------------------------------------------------------------------*/
#define _ESCPR_DEBUG_SP             0       /* 0: OFF    1: ON                          */
#define _ESCPR_DEBUG_SP_VERBOSE     0       /* 0: OFF    1: ON                          */

    /*** _ESCPR_DEBUG_SP --- Definition of << DUMP_PMREPLY() >>                         */
    /*** -------------------------------------------------------------------------------*/
#if _ESCPR_DEBUG_SP | _VALIDATE_SUPPORTED_MEDIA_DATA_

typedef enum _DUMP_TYPE {
    DUMP_HEX = 0,
    DUMP_ASCII,
    DUMP_S_TAG_ONLY,
} DUMP_TYPE;

char* str[] ={
                 "DUMP_HEX",
                 "DUMP_ASCII",
                 "DUMP_S_TAG_ONLY",
             };

static void print_PMREPLY(EPS_UINT8* pm, DUMP_TYPE type, EPS_INT8* msg)
{
    EPS_UINT8* p = pm;
    EPS_INT16 col = 0;

    if(*p != 'S') {
        if(type != DUMP_HEX) {
            return; /* do not anything */
        }

        /* Anyway if type is DUMP_HEX then dump it */
    }

    printf("%s\r\n", msg);
    printf("PM REPLY DUMP [TYPE:%s]\r\n", str[type]);

    if(type == DUMP_HEX) {
        while(!((*p == 0x0D) && (*(p+1) == 0x0A))) {
            printf("0x%02X ",   *p++);

            if((++col % 10) == 0) {
                printf("\r\n");
            }
        }

    } else {
        while(*p == 'S') {
            printf("%c ",   *p++);
            printf("%02d\r\n", *p++);
            while(*p == 'T') {
                printf("  %c",     *p++);
                printf("  %02d",   *p++);
                printf("  [0x%02X]", *p++);
                printf("  %c\r\n",     *p++);
            }
            printf("%c\r\n",     *p++);

            if(type == DUMP_S_TAG_ONLY) {
                break;
            }

            if ((*p == 0x0D) && (*(p+1) == 0x0A)) {
                break;
            }
        }

    }

    if(type != DUMP_S_TAG_ONLY) {
        printf("0x%02X ",   *p++);
        printf("0x%02X ",   *p);
    }

    printf("\r\nEND\r\n");

}
#endif

#if _ESCPR_DEBUG_SP
#define DUMP_PMREPLY(a) print_PMREPLY a
#else
#define DUMP_PMREPLY(a)
#endif

    /*** _ESCPR_DEBUG_SP_VERBOSE                                                        */
    /***    --- Definition of << VERBOSE_DUMP_PMREPLY() >>  and << verbose_dbprint() >> */
    /*** -------------------------------------------------------------------------------*/
#if _ESCPR_DEBUG_SP_VERBOSE
#define VERBOSE_DUMP_PMREPLY(a) print_PMREPLY a
#define verbose_dbprint(a)      EPS_DBGPRINT(a)
#else
#define VERBOSE_DUMP_PMREPLY(a)
#define verbose_dbprint(a)  
#endif

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%                                             %%%%%%%%%%%%%%%%%%%%%*/
/*--------------------              Public Functions               ---------------------*/
/*%%%%%%%%%%%%%%%%%%%%                                             %%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   serParseDeviceID()                                                  */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:            Type:           Description:                                        */
/* deviceIDString	EPS_INT8*       I: DeviceID string                                  */
/* manufacturer		EPS_INT8*       I: Manufacturer Name                                */
/* modelName		EPS_INT8*       I: Model Name                                       */
/*                                                                                      */
/* Return value:                                                                        */
/*      1                       - Success                                               */
/*      -1                      - Bad format or Not Supported device                    */
/*      0                       - unknown                                               */
/*                                                                                      */
/* Description:                                                                         */
/*      Get the manufacturer name and modelName from DeviceID string.                   */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32     serParseDeviceID (

		EPS_INT8*			deviceIDString,
		EPS_INT32           strlength,
        EPS_INT8*			manufacturer,
        EPS_INT8*			modelName,
        EPS_INT32*			cmdLevel,
		EPS_UINT32*			lang,
        EPS_UINT32*			errorID

){
//    debug_msg("start call function epson-escpr-services.c/serParseDeviceID\n");
    EPS_INT8*       i = NULL;
    EPS_INT8*       j = NULL;
    EPS_INT8*       k = NULL;
    EPS_INT8*       p = NULL;
	EPS_BOOL		found = FALSE;

	EPS_LOG_FUNCIN;

/*** Delete the data of the device ID length from string                                */
	if(strlength < 2){
		EPS_RETURN( -1 );
	}
    if(deviceIDString[0] == 0x00 || deviceIDString[1] == 0x00){
        deviceIDString += 2;
	}
/*** ================================================================================== */
/*** Step 1: Get Manufacturer Name                                                      */
/*** ================================================================================== */
    if((i = strstr(deviceIDString, "MFG:")) != NULL) {
	    i += 4;
	} else if((i = strstr(deviceIDString, "MANUFACTURER:")) != NULL) {
	    i += 13;
	} else{
        /* Invalid device ID. */
		EPS_RETURN( -1 );
	}
    j  = i;
    
    while(*j != ';')
    {
        j++;
        
        /* Emergency exit condition to prevent an infinite loop scenario; if we hit a   */
        /* carriage return, we've run too far                                           */
        if(*j == 0x0D || *j == 0x00) {
            EPS_RETURN( -1 );
        }
    }
    /* Null-terminate the MFG substring. */
    *j = 0;
    
	if((strncmp(i, "EPSON", 5) != 0) && (strncmp(i, "Epson", 5) != 0) ){
	    *j = ';';
		EPS_RETURN( -1 );
	}

	if(NULL != manufacturer){
		/* Make sure the manufacturer name is not longer than 64 bytes. */
		if(strlen(i) < 64) {
			strcpy(manufacturer, i);    /* If the name is OK, copy the whole string as-is */
		}
		else {
			memcpy(manufacturer, i, 63);    /* If the name is longer than 64 bytes, only copy */
		}                                   /* 63 bytes and leave the 64th as null terminator */
	}

    /* Return the string to its original format. */
    *j = ';';
    

/*** ================================================================================== */
/*** Step 2: Get Model Name                                                             */
/*** ================================================================================== */
	if(NULL != modelName){
		if((i = strstr(deviceIDString, "MDL:")) != NULL) {
			i += 4;
		} else if((i = strstr(deviceIDString, "MODEL:")) != NULL) {
			i += 6;
		} else{
			/* Invalid device ID. */
			EPS_RETURN( -1 );
		}
		j  = i;
    
		while (*j != ';')
		{
			j++;
        
			/* Emergency exit condition to prevent an infinite loop scenario; if we hit a   */
			/* carriage return, we've run too far                                           */
			if(*j == 0x0D || *j == 0x00){
				/* Invalid device ID. */
				EPS_RETURN( -1 );
			}
		}
    
		/* Null-terminate the MDL substring. */
		*j = 0;
    
		/* Make sure the model name is not longer than 64 bytes. */
		if(strlen(i) < 64) {
			strcpy(modelName, i);   /* If the name is OK, copy the whole string as-is */
		}
		else {
			memcpy(modelName, i, 63);   /* If the name is longer than 64 bytes, only copy */
										/* 63 bytes, leaving the 64th as null terminator  */
		}
	
		*j = ';';
	}


/*** ================================================================================== */
/*** Step 3: Check for ESC/PR Support                                                   */
/*** ================================================================================== */

    if((i = strstr(deviceIDString, "CMD:")) == NULL) {
		EPS_RETURN( 0 );
	}
	i += 4;

    if((j = strstr(i, ";")) == NULL) {
		EPS_RETURN( -1 );
	}
	*j = '\0';

	while( NULL != i ){
		if((k = strstr(i, ",")) != NULL) {
			*k = '\0';
		}

//		if((p = strstr(i, "ESCPR")) != NULL) {
	if(((p = strstr(i, "ESCPR")) != NULL) || ((p = strstr(i, "ESCPL2")) != NULL)) {
			found = TRUE;
			if(NULL != lang){
				*lang = EPS_LANG_ESCPR;
			}
			if(NULL != cmdLevel){
				sscanf(p+5, "%d", cmdLevel);
			}
			break;
#if 0
#ifdef GCOMSW_CMD_ESCPAGE
		} else if(strstr(i, "ESCPAGECOLOR") != NULL) {
			found = TRUE;
			if(NULL != lang){
				*lang = EPS_LANG_ESCPAGE_COLOR;
			}
			if(NULL != cmdLevel){
				*cmdLevel = 1;
			}
			break;

		} else if(strstr(i, "ESCPAGE") != NULL) {
			if(strstr(i, "ESCPAGES") == NULL) {
				found = TRUE;
				if(NULL != lang){
					*lang = EPS_LANG_ESCPAGE;
				}
				if(NULL != cmdLevel){
					*cmdLevel = 1;
				}
				/* break; ESC/Page Color takes priority */
#ifdef GCOMSW_CMD_ESCPAGE_S
			} else{
				if(TRUE == pageS_ParseLang(i, cmdLevel, lang)){
					found = TRUE;
					break;
				}
#endif
#endif
			}

#ifdef GCOMSW_CMD_PCL
		} else if( (strstr(i, "PCLXL") != NULL) && (strlen(i) == 5) ){
			if(NULL != lang){
				*lang = EPS_LANG_PCL;
			}
			if(NULL != cmdLevel){
				*cmdLevel = 1;
			}
			found = TRUE;
			break;
#endif

#endif /* GCOMSW_CMD_ESCPAGE */
		}

		if(k){
			*k = ',';
			i = k+1;
			if(i >= j){
				break;
			}
		} else{
			break;
		}
	}
	if(k)*k = ',';

	*j = ';';

	if(FALSE == found){
		EPS_RETURN( -1 );
	}

/*** ================================================================================== */
/*** Step 4: Error Group ID                                                             */
/*** ================================================================================== */
	if(NULL != errorID){
		if((i = strstr(deviceIDString, "ELG:")) != NULL) {
			i += 4;
		} else{
			EPS_RETURN( 1 );
		}
		j  = i;
    
		while (*j != ';')
		{
			j++;
			if(*j == 0x0D || *j == 0x00){
				EPS_RETURN( 1 );
			}
		}
    
		/* Null-terminate the MDL substring. */
		*j = 0;
    
		if(strlen(i) <= 4) {
			sscanf(i, "%x", errorID);
		}
		*j = ';';
	}
//    debug_msg("Exit call function epson-escpr-services.c/serParseDeviceID\n");

    EPS_RETURN( 1 );
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   serCheckLang()		                                                */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* langString   EPS_INT8*           I: Print lang string                                */
/* lang         EPS_UINT32*         O: lang id                                          */
/*                                                                                      */
/* Return value:                                                                        */
/*      TRUE                        - Success                                           */
/*      FALSE                       - Not Supported device                              */
/*                                                                                      */
/* Description:                                                                         */
/*      Get the printer status and analyze the status string.                           */
/*                                                                                      */
/*******************************************|********************************************/
EPS_BOOL    serCheckLang (

        EPS_INT8*		langString,
        EPS_UINT32*		lang

){
	if( (strncmp(langString, "ESC-P/R", 7) == 0) ){
		*lang = EPS_LANG_ESCPR;
		return TRUE;
#ifdef GCOMSW_CMD_PCL
	} else if( (strcmp(langString, "PCL 6") == 0) ){
		*lang = EPS_LANG_PCL;
		return TRUE;
#endif
	}

	return FALSE;
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   serAnalyzeStatus()                                                  */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* Status       EPS_INT8*           I: Printer Status string                            */
/* egID         EPS_UINT32          I: printer error group id                           */
/* pstInfo      EPS_STATUS_INFO*    O: Printer Status information                       */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPS_ERR_NONE                    - Success                                       */
/*      EPS_ERR_COMM_ERROR              - Communication Error                           */
/*                                                                                      */
/* Description:                                                                         */
/*      Get the printer status and analyze the status string.                           */
/*                                                                                      */
/*******************************************|********************************************/
EPS_ERR_CODE    serAnalyzeStatus (

        EPS_INT8*           Status,
		EPS_UINT32          egID,
        EPS_STATUS_INFO*    pstInfo

){
    EPS_UINT8*  Field;
    EPS_UINT8*  EndField;
    EPS_INT32   i;
    EPS_INT32   Min;
    
    EPS_UINT8   Header;
    EPS_UINT8   ParameterByte;
    EPS_UINT8*  param;
    EPS_INT8*   Ink;
    EPS_INT8    Parameter[128];
    EPS_UINT8   InkCartridgeType = 0;
	EPS_BOOL	mediaErr = FALSE;
    EPS_UINT8   mediaErrNum, jamErrNum;
	EPS_BOOL	showInkInfo = TRUE;
	EPS_BOOL	showInkLow  = TRUE;


	EPS_LOG_FUNCIN;

	if((Field = (EPS_UINT8*)strstr(Status,"ST2")) == NULL ){
        SerDebugPrintf(("EPS SER: Get Stat -> ST not found [%s]\r\n",Status));
        EPS_RETURN( EPS_ERR_COMM_ERROR );
    }
    
	memset(pstInfo, 0, sizeof(EPS_STATUS_INFO));
    pstInfo->nState = EPS_ST_IDLE;
    pstInfo->nError = EPS_PRNERR_NOERROR;
    pstInfo->nWarn  = EPS_PRNWARN_NONE;
	mediaErrNum = jamErrNum = 0;

// Initialize Papaer count

	pstInfo->paper_count.color = -1;
	pstInfo->paper_count.monochrome = -1;
	pstInfo->paper_count.blank = -1;
	pstInfo->paper_count.adf = -1;
	pstInfo->paper_count.color_borderless = -1;
	pstInfo->paper_count.monochrome_borderless = -1;

#if _DEBUG_BIN_STATUS_
	EPS_DUMP(Status, 256);    
#endif
    Field    = Field + 5;
    EndField = Field + (2+(*Field)+ ((*(Field+1))*256) );
    Field    = Field + 2;

    while ( Field < EndField ) {
    
        Header        = (EPS_UINT8) Field[0];
        ParameterByte = (EPS_UINT8) Field[1];
        memcpy(Parameter, Field+2, (EPS_INT8)ParameterByte );

        param = Field + 2;        
        Field = Field + 2 + ParameterByte;
        
        switch( Header ) {
            case 0x01: /* ST */
                switch( Parameter[0] ) {
                    case 0x00: pstInfo->nState = EPS_ST_ERROR;               break;
                    case 0x01: pstInfo->nState = EPS_ST_SELF_PRINTING;       break;
                    case 0x02: pstInfo->nState = EPS_ST_BUSY;                break;
                    case 0x03: pstInfo->nState = EPS_ST_WAITING;             break;
                    case 0x04: pstInfo->nState = EPS_ST_IDLE;                break;
                /*  case 0x05: pstInfo->nState = EPS_ST_PAUSE;               break;*/    /* Not supported by 2006 Model */
                /*  case 0x06: pstInfo->nState = EPS_ST_INKDRYING            break;*/    /* Not supported by 2006 Model */
                    case 0x07: pstInfo->nState = EPS_ST_CLEANING;            break;
                    case 0x08: pstInfo->nState = EPS_ST_FACTORY_SHIPMENT;    break;
                /*  case 0x09: pstInfo->nState = EPS_ST_MOTOR_DRIVE_OFF;     break;*/    /* Not supported by 2006 Model */
                    case 0x0A: pstInfo->nState = EPS_ST_SHUTDOWN;            break;
                /*  case 0x0B: pstInfo->nState = EPS_ST_WAITPAPERINIT;       break;*/    /* Not supported by 2006 Model */
                /*  case 0x0C: pstInfo->nState = EPS_ST_INIT_PAPER;          break;*/    /* Not supported by 2006 Model */
                    default:   
						pstInfo->nState = EPS_ST_ERROR;               
						if(pstInfo->nError == EPS_PRNERR_NOERROR){
							pstInfo->nError = EPS_PRNERR_GENERAL;
						}
						break;
                }
                break;

            case 0x02: /* ER */
				EPS_DBGPRINT(("* ERR 0x%02X *\n", Parameter[0]))
                switch(Parameter[0]) {
                    case 0x00: pstInfo->nError = EPS_PRNERR_FATAL;                  break;
                    case 0x01: pstInfo->nError = EPS_PRNERR_INTERFACE;              break;
                    case 0x02: 
						if ( obsIsA3Model(EPS_MDC_STATUS) == TRUE ) {
							pstInfo->nError = EPS_PRNERR_CDRGUIDEOPEN;
						} else{
							pstInfo->nError = EPS_PRNERR_COVEROPEN;
						}
						break;
                /*  case 0x03: pstInfo->nError = EPS_PRNERR_LEVERPOSITION;          break;*/    /* Not supported by 2006 Model */
                    case 0x04: pstInfo->nError = EPS_PRNERR_PAPERJAM;               break;
                    case 0x05: pstInfo->nError = EPS_PRNERR_INKOUT;					break;
                    case 0x06: pstInfo->nError = EPS_PRNERR_PAPEROUT;               break;
                /*  case 0x07: pstInfo->nError = EPS_PRNERR_INITIALIZESETTING;      break;*/    /* Not supported by 2006 Model */
                /*  case 0x08: pstInfo->nError = EPS_PRNERR_UNKNOWN;                break;*/    /* Not supported by 2006 Model */
                    case 0x09: pstInfo->nError = EPS_PRNERR_CDRGUIDEOPEN/*EPS_PRNERR_PAPERCHANGE_UNCOMP*/;     break; /* supported by 2012 Model */
                    case 0x0A: pstInfo->nError = EPS_PRNERR_SIZE_TYPE_PATH/*EPS_PRNERR_PAPERSIZE*/;break;/* supported by 2008 Model */
                /*  case 0x0B: pstInfo->nError = EPS_PRNERR_RIBBONJAM;              break;*/    /* Not supported by 2006 Model */
                    case 0x0C: pstInfo->nError = EPS_PRNERR_SIZE_TYPE_PATH;         break;
                /*  case 0x0D: pstInfo->nError = EPS_PRNERR_PAPERTHICKLEVER;        break;*/    /* Not supported by 2006 Model */
                /*  case 0x0E: pstInfo->nError = EPS_PRNERR_PAPERFEED;              break;*/    /* Not supported by 2006 Model */
                /*  case 0x0F: pstInfo->nError = EPS_PRNERR_SIMMCOPY;               break;*/    /* Not supported by 2006 Model */
                    case 0x10: pstInfo->nError = EPS_PRNERR_SERVICEREQ;             break;
                /*  case 0x11: pstInfo->nError = EPS_PRNERR_WAITTEAROFFRETURN;      break;*/    /* Not supported by 2006 Model */
                    case 0x12: pstInfo->nError = EPS_PRNERR_DOUBLEFEED;             break;
                /*  case 0x13: pstInfo->nError = EPS_PRNERR_HEADHOT;                break;*/    /* Not supported by 2006 Model */
                /*  case 0x14: pstInfo->nError = EPS_PRNERR_PAPERCUTMIS;            break;*/    /* Not supported by 2006 Model */
                /*  case 0x15: pstInfo->nError = EPS_PRNERR_HOLDLEVERRELEASE;       break;*/    /* Not supported by 2006 Model */
                /*  case 0x16: pstInfo->nError = EPS_PRNERR_NOT_CLEANING;           break;*/    /* Not supported by 2006 Model */
                    case 0x17: pstInfo->nError = EPS_PRNERR_SIZE_TYPE_PATH/*EPS_PRNERR_PAPERCONFIG*/; break; /* supported by 2012 Model */
                /*  case 0x18: pstInfo->nError = EPS_PRNERR_PAPERSLANT;             break;*/    /* Not supported by 2006 Model */
                /*  case 0x19: pstInfo->nError = EPS_PRNERR_CLEANINGNUMOVER;        break;*/    /* Not supported by 2006 Model */
                    case 0x1A: pstInfo->nError = EPS_PRNERR_INKCOVEROPEN;           break;
                /*  case 0x1B: pstInfo->nError = EPS_PRNERR_LFP_INKCARTRIDGE;       break;*/    /* Not supported by 2006 Model */
                /*  case 0x1C: pstInfo->nError = EPS_PRNERR_CUTTER;                 break;*/    /* Not supported by 2006 Model */
                /*  case 0x1D: pstInfo->nError = EPS_PRNERR_CUTTERJAM;              break;*/    /* Not supported by 2006 Model */
                /*  case 0x1E: pstInfo->nError = EPS_PRNERR_INKCOLOR;               break;*/    /* Not supported by 2006 Model */
                /*  case 0x1F: pstInfo->nError = EPS_PRNERR_CUTTERCOVEROPEN;        break;*/    /* Not supported by 2006 Model */
                /*  case 0x20: pstInfo->nError = EPS_PRNERR_LFP_INKLEVERRELEASE;    break;*/    /* Not supported by 2006 Model */
                /*  case 0x22: pstInfo->nError = EPS_PRNERR_LFP_NOMAINTENANCETANK1; break;*/    /* Not supported by 2006 Model */
                /*  case 0x23: pstInfo->nError = EPS_PRNERR_CARTRIDGECOMBINATION;   break;*/    /* Not supported by 2006 Model */
                /*  case 0x24: pstInfo->nError = EPS_PRNERR_LFP_COMMAND;            break;*/    /* Not supported by 2006 Model */
                /*  case 0x25: pstInfo->nError = EPS_PRNERR_LEARCOVEROPEN;          break;*/    /* Not supported by 2006 Model */
                    case 0x25: pstInfo->nError = EPS_PRNERR_COVEROPEN;              break;
                /*  case 0x26: pstInfo->nError = EPS_PRNERR_MULTICENSORGAIN;        break;*/    /* Not supported by 2006 Model */
                /*  case 0x27: pstInfo->nError = EPS_PRNERR_NOT_AUTOADJUST;         break;*/    /* Not supported by 2006 Model */
                /*  case 0x28: pstInfo->nError = EPS_PRNERR_FAILCLEANING;           break;*/    /* Not supported by 2006 Model */
                    case 0x29: pstInfo->nError = EPS_PRNERR_NOTRAY;                 break;
                    case 0x2A: pstInfo->nError = EPS_PRNERR_CARDLOADING;            break;
                    case 0x2B: 
						if ( obsIsA3Model(EPS_MDC_STATUS) == TRUE ) {
							pstInfo->nError = EPS_PRNERR_CDRGUIDEOPEN;
						} else {
							pstInfo->nError = EPS_PRNERR_CDDVDCONFIG;							/* supported by 2008 Model */
						}
						break;
                    case 0x2C: pstInfo->nError = EPS_PRNERR_CARTRIDGEOVERFLOW;      break;
                /*  case 0x2D: pstInfo->nError = EPS_PRNERR_LFP_NOMAINTENANCETANK2; break;*/    /* Not supported by 2006 Model */
                /*  case 0x2E: pstInfo->nError = EPS_PRNERR_INKOVERFLOW2;           break;*/    /* Not supported by 2006 Model */
                    case 0x2F: pstInfo->nError = EPS_PRNERR_BATTERYVOLTAGE;         break;
                    case 0x30: pstInfo->nError = EPS_PRNERR_BATTERYTEMPERATURE;     break;
                    case 0x31: pstInfo->nError = EPS_PRNERR_BATTERYEMPTY;           break;
                    case 0x32: pstInfo->nError = EPS_PRNERR_SHUTOFF;                break;      /* Not supported by 2006 Model */
                    case 0x33: pstInfo->nError = EPS_PRNERR_NOT_INITIALFILL;        break;      /* Not supported by 2006 Model */
                    case 0x34: pstInfo->nError = EPS_PRNERR_PRINTPACKEND;           break;      /* Not supported by 2006 Model */
                /*  case 0x35: pstInfo->nError = EPS_PRNERR_ABNORMALHEAT;           break;*/    /* Not supported by 2006 Model */
                    case 0x37:
						if ( obsIsA3Model(EPS_MDC_STATUS) == TRUE ) {
							pstInfo->nError = EPS_PRNERR_COVEROPEN;
						} else if(0x0D04 == egID){
							pstInfo->nError = EPS_PRNERR_COVEROPEN;
						} else{
							pstInfo->nError = EPS_PRNERR_SCANNEROPEN;
						}
						break;
                    case 0x38:
						if ( obsIsA3Model(EPS_MDC_STATUS) == TRUE ) {
							pstInfo->nError = EPS_PRNERR_CDDVDCONFIG;
						} else if(0x0D00 <= egID && 0x0D05 >= egID){
							pstInfo->nError = EPS_PRNERR_TRAYCLOSE;
						} else{
							pstInfo->nError = EPS_PRNERR_CDRGUIDEOPEN;
						}
						break;
                    case 0x44: pstInfo->nError = EPS_PRNERR_CDRGUIDEOPEN;			break;
					case 0x45: pstInfo->nError = EPS_PRNERR_CDREXIST_MAINTE;		break;
					case 0x46: pstInfo->nError = EPS_PRNERR_TRAYCLOSE;				break;
					case 0x47: pstInfo->nError = EPS_PRNERR_INKOUT;					break;		/* BlackPrint Error */

					case 0x4C: pstInfo->nError = EPS_PRNERR_PAPEROUT;				break;
					case 0x4D: pstInfo->nError = EPS_PRNERR_CDDVDCONFIG;			break;
					case 0x4E: pstInfo->nError = EPS_PRNERR_FEEDERCLOSE;			break;
					case 0x55: mediaErr = TRUE;  									break;
						
					default:   
						pstInfo->nError = EPS_PRNERR_GENERAL;                
						break;
                }
                break;

            case 0x04: /* WR */
                /* ESC/P-R Lib does not notified the warning to application, */
                /* so warning analysis dose not need to be done completely.  */
				pstInfo->nWarn = EPS_PRNWARN_NONE;
				for(i = 0; i < ParameterByte; i++){
					if( Parameter[i] >= 0x10 && Parameter[i] <= 0x1A ){
						/* Ink Low Warning */
						pstInfo->nWarn |= EPS_PRNWARN_INKLOW;
						pstInfo->nColorStatus[ Parameter[i]-0x10 ] = EPS_INK_ST_LOW;
					} else if( Parameter[i] >= 0x51 && Parameter[i] <= 0x5A ){
						pstInfo->nWarn |= EPS_PRNWARN_DISABLE_CLEAN;
						pstInfo->nColorStatus[ Parameter[i]-0x51 ] = EPS_INK_ST_LOW;
					} else if( Parameter[i] >= 0x71 && Parameter[i] <= 0x7A ){
						pstInfo->nColorStatus[ Parameter[i]-0x71 ] = EPS_INK_ST_LOW;
					/*} else if( Parameter[i] == 0x44 ){	not use
						pstInfo->nWarn |= EPS_PRNWARN_COLOR_INKOUT;*/
					}
				}
                break;

			case 0x0A: /* Characteristic status code */
				#define HIDE_INKINFO (0x01 << 0)
				#define HIDE_INKLOW  (0x01 << 1)

				switch(param[0]){
					case 0x10:
						showInkInfo = (param[1] & HIDE_INKINFO) ? FALSE : TRUE;
						showInkLow  = (param[1] & HIDE_INKLOW)  ? FALSE : TRUE;
						break;
					case 0x11:
						showInkInfo = (param[1] & HIDE_INKINFO) ? FALSE : TRUE;					
						break;
				}
				break;

            case 0x0F: /* INK */
                Ink = Parameter;
                
                pstInfo->nInkError = EPS_INKERR_NONE;

                if (Ink[1] >= 0x40) {
                    InkCartridgeType = MI_CARTRIDGE_ONE;
                } else {
                    InkCartridgeType = MI_CARTRIDGE_INDEP;
                                }
                
                for( pstInfo->nInkNo=0, i=0; i<EPS_INK_NUM; i++ ) {
                    pstInfo->nColorType[i] = EPS_COLOR_UNKNOWN;
                    pstInfo->nColor[i]     = EPS_INK_NOTAVAIL;
                    
                    if( Ink > Parameter+ParameterByte-Parameter[0] )
                        continue;
                    
                    pstInfo->nInkNo++;

                    switch( Ink[2] ) {
                        
                        case 0x00:    pstInfo->nColorType[i] = EPS_COLOR_BLACK;        break;
                        case 0x01:    pstInfo->nColorType[i] = EPS_COLOR_CYAN;         break;
                        case 0x02:    pstInfo->nColorType[i] = EPS_COLOR_MAGENTA;      break;
                        case 0x03:    pstInfo->nColorType[i] = EPS_COLOR_YELLOW;       break;
                        case 0x04:    pstInfo->nColorType[i] = EPS_COLOR_LIGHTCYAN;    break;
                        case 0x05:    pstInfo->nColorType[i] = EPS_COLOR_LIGHTMAGENTA; break;
                        case 0x06:    pstInfo->nColorType[i] = EPS_COLOR_LIGHTYELLOW;  break;
                        case 0x07:    pstInfo->nColorType[i] = EPS_COLOR_DARKYELLOW;   break;
                        case 0x08:    pstInfo->nColorType[i] = EPS_COLOR_LIGHTBLACK;   break;
                        case 0x09:    pstInfo->nColorType[i] = EPS_COLOR_RED;          break;
                        case 0x0A:    pstInfo->nColorType[i] = EPS_COLOR_VIOLET;       break;
                        case 0x0B:    pstInfo->nColorType[i] = EPS_COLOR_CLEAR;        break;
                        case 0x0C:    pstInfo->nColorType[i] = EPS_COLOR_LIGHTLIGHTBLACK;    break;
                        case 0x0D:    pstInfo->nColorType[i] = EPS_COLOR_ORANGE;		break;
                        case 0x0E:    pstInfo->nColorType[i] = EPS_COLOR_GREEN;			break;
                        default:     
							pstInfo->nColorType[i] = EPS_COLOR_UNKNOWN;      
							break;
                    }

                    switch( Ink[3] ) {
                        case 'w' :
                        case 'r' :
                            pstInfo->nColor[i] = EPS_INK_FAIL;
							pstInfo->nColorStatus[i] = EPS_INK_ST_FAIL;
                            ( pstInfo->nColorType[i]==EPS_COLOR_BLACK ) ? 
                                (pstInfo->nInkError=EPS_INKERR_CFAILB) : (pstInfo->nInkError=EPS_INKERR_CFAILC);
                            break;
                        case 'n' :
                            pstInfo->nColor[i] = EPS_INK_NOTPRESENT;
							pstInfo->nColorStatus[i] = EPS_INK_ST_NOTPRESENT;
                            ( pstInfo->nColorType[i]==EPS_COLOR_BLACK ) ?
                                (pstInfo->nInkError = EPS_INKERR_CEMPTYB) : (pstInfo->nInkError = EPS_INKERR_CEMPTYC);
                            break;
                        case 'i' :
                            pstInfo->nColor[i] = EPS_INK_NOREAD;
							pstInfo->nColorStatus[i] = EPS_INK_ST_NOREAD;
                            break;
                        case 'g' :
                            pstInfo->nColor[i] = EPS_INK_NOTAVAIL;
							pstInfo->nColorStatus[i] = EPS_INK_ST_NOTAVAIL;
                            break;
                        default:
                            if((Ink[3] >= 0) && (Ink[3] <= 100)) {
                                
                               pstInfo->nColor[i] = serInkLevelNromalize( Ink[3] );
                                
                                if( pstInfo->nColor[i] == 0) {
                                    if( pstInfo->nColorType[i]==EPS_COLOR_BLACK ){
                                        (pstInfo->nInkError = EPS_INKERR_INKENDB);
									} else{
                                        (pstInfo->nInkError = EPS_INKERR_INKENDC);
									}
									if(EPS_PRNERR_INKOUT == pstInfo->nError){
										pstInfo->nColorStatus[i] = EPS_INK_ST_END;
									}
                               		 			}

                            } else {
                                pstInfo->nColor[i] = EPS_INK_FAIL;
                                if( pstInfo->nColorType[i]==EPS_COLOR_BLACK )
                                    (pstInfo->nInkError=EPS_INKERR_CFAILB);
                                else
                                    (pstInfo->nInkError=EPS_INKERR_CFAILC);
                          				  }
                            break;
                    		}

                    Ink = Ink + Parameter[0];
              			  }
                
                if( InkCartridgeType == MI_CARTRIDGE_ONE ){
                    switch(pstInfo->nInkError) {
                        case EPS_INKERR_INKENDB:
                        case EPS_INKERR_INKENDC:
                            pstInfo->nInkError = EPS_INKERR_INKENDALL;
                            break;
                        case EPS_INKERR_CFAILB:
                        case EPS_INKERR_CFAILC:
                            pstInfo->nInkError = EPS_INKERR_CFAILALL;
                            break;
                        case EPS_INKERR_CEMPTYB:
                        case EPS_INKERR_CEMPTYC:
                            pstInfo->nInkError = EPS_INKERR_CEMPTYALL;
                            break;
                    }
                }
                break;
            case 0x13: /* CANCEL REQUEST by Printer cancel botton */
                switch((EPS_UINT8)Parameter[0]) {
                    case 0x81:
                        pstInfo->nCancel = EPS_CAREQ_CANCEL;
                        break;
                    default:
                        pstInfo->nCancel = EPS_CAREQ_NOCANCEL;
                        break;
                }
                break;
            case 0x18: /* Stacker(tray) open status */
                switch((EPS_UINT8)Parameter[0]) {
                    case 0x02:  /* Closed*/
                        pstInfo->nPrepare = EPS_PREPARE_TRAYCLOSED;
                        break;
                    case 0x03: /* Open*/
                        pstInfo->nPrepare = EPS_PREPARE_TRAYOPENED;
                        break;
                }
                break;
            case 0x1C: /* Temperature information */
                switch((EPS_UINT8)Parameter[0]) {
                    case 0x01: /* The printer temperature is higher than 40C*/
                        pstInfo->nPrepare = EPS_PREPARE_OVERHEAT;
                        break;
                    case 0x00: /* The printer temperature is lower than 40C*/
                        pstInfo->nPrepare = EPS_PREPARE_NORMALHEAT;
                        break;
                }
                break;

            case 0x35: /* Paper Jam error information */
				jamErrNum = (EPS_UINT8)Parameter[0];
                break;

            case 0x36: /* Paper Count information */
				if((ParameterByte&0xff) != 0x14 && (ParameterByte&0xff) != 0x18 && (ParameterByte&0xff) != 0x20) break;
				
			//color printed count
				pstInfo->paper_count.color = ((Parameter[8])&0xff) + (Parameter[9]&0xff)*256 + (Parameter[10]&0xff)*(256^2) + (Parameter[11]&0xff)*(256^3);

			//mono printed count
				pstInfo->paper_count.monochrome = ((Parameter[12])&0xff) + (Parameter[13]&0xff)*256 + (Parameter[14]&0xff)*(256^2) + (Parameter[15]&0xff)*(256^3);

			//blank printed count
				pstInfo->paper_count.blank = ((Parameter[16])&0xff) + (Parameter[17]&0xff)*256 + (Parameter[18]&0xff)*(256^2) + (Parameter[19]&0xff)*(256^3);

				switch(ParameterByte&0xff){
				case 0x18:
			//adf scanned count
				pstInfo->paper_count.adf = ((Parameter[20])&0xff) + (Parameter[21]&0xff)*256 + (Parameter[22]&0xff)*(256^2) + (Parameter[23]&0xff)*(256^3);				
					break;
				case 0x20:
			//adf scanned count
				pstInfo->paper_count.adf = ((Parameter[20])&0xff) + (Parameter[21]&0xff)*256 + (Parameter[22]&0xff)*(256^2) + (Parameter[23]&0xff)*(256^3);	

			//color borderless printed count
				pstInfo->paper_count.color_borderless = ((Parameter[24])&0xff) + (Parameter[25]&0xff)*16 + (Parameter[26]&0xff)*(16^2) + (Parameter[27]&0xff)*(16^3);	

			//mono borderless printed count
				pstInfo->paper_count.monochrome_borderless = ((Parameter[28])&0xff) + (Parameter[29]&0xff)*256 + (Parameter[30]&0xff)*(256^2) + (Parameter[31]&0xff)*(256^3);	

					break;
				}
				break;
			case 0x3B: /* Media */
				mediaErrNum = (EPS_UINT8)Parameter[0];
                break;

            default:
                break;
        }
    }

	if( TRUE == mediaErr && EPS_PRNERR_NOERROR == pstInfo->nError){
		switch( mediaErrNum ){
		case 0x01:	pstInfo->nError = EPS_PRNERR_3DMEDIA_FACE;		break;
		case 0x02:	pstInfo->nError	= EPS_PRNERR_3DMEDIA_DIRECTION;	break;
		default: break;
		}
	} else if ( /*EPS_PRNERR_PAPERJAM == pstInfo->nError ||*/ EPS_PRNERR_PAPEROUT == pstInfo->nError ){
		switch( jamErrNum ){
		case 0x90:	
			if(0x0D04 == egID){
				pstInfo->nError = EPS_PRNERR_MANUALFEED_SET_PAPER_NOLCD;
			} else{
				pstInfo->nError = EPS_PRNERR_MANUALFEED_SET_PAPER;
			}
			break;
		case 0x91:
			if(0x0D04 == egID){
				pstInfo->nError	= EPS_PRNERR_MANUALFEED_FAILED_NOLCD;
			} else{
				pstInfo->nError	= EPS_PRNERR_MANUALFEED_FAILED;
			}
			break;
		case 0x92:
			if(0x0D04 == egID){
				pstInfo->nError	= EPS_PRNERR_MANUALFEED_EXCESSIVE_NOLCD;
			} else{
				pstInfo->nError	= EPS_PRNERR_MANUALFEED_EXCESSIVE;
			}
			break;
		default: break;
		}
	}

	if( EPS_CAREQ_CANCEL == pstInfo->nCancel ){
		EPS_DBGPRINT(("*** Cancel Request (ignore error) ***\n"))
		pstInfo->nState = EPS_ST_WAITING;
		pstInfo->nError = EPS_PRNERR_NOERROR;
	}

#if _DEBUG_BIN_STATUS_
    SerDebugPrintf(("***** ST = %d\r\n", pstInfo->nState));
    SerDebugPrintf(("***** ER = %d\r\n", pstInfo->nError));
    SerDebugPrintf(("***** WR = %d\r\n", pstInfo->nWarn));
    SerDebugPrintf(("***** CA = %d\r\n", pstInfo->nCancel));
    SerDebugPrintf(("***** INK NUM = %d\r\n", pstInfo->nInkNo));
    for(i = 0; i < pstInfo->nInkNo; i++){
    SerDebugPrintf(("***** INK = %d\t%d\r\n", pstInfo->nColorType[i], pstInfo->nColor[i]));
    }
#endif
	
	if(InkCartridgeType == MI_CARTRIDGE_ONE) {
        Min = pstInfo->nColor[0];
        for(i = 1; i < pstInfo->nInkNo; i++){
            Min = ( (Min <= pstInfo->nColor[i]) ? Min : pstInfo->nColor[i] );
        }
        for(i = 0; i < pstInfo->nInkNo; i++){
            pstInfo->nColor[i] = Min;
        }
    }

	pstInfo->showInkLow = showInkLow;
	pstInfo->showInkInfo = showInkInfo;

    EPS_RETURN( EPS_ERR_NONE );
}


#define EPS_INK_NORMALIZE_LEVEL (5)
EPS_INT32    serInkLevelNromalize (

		EPS_INT32 level

){
	EPS_INT32 norm = 0;
	if (EPS_INK_NORMALIZE_LEVEL > 3 && (level >= 1) && (level <= 3)) {
		norm = 1;
	} else{
		norm = (level / EPS_INK_NORMALIZE_LEVEL) * EPS_INK_NORMALIZE_LEVEL;
		if(level % EPS_INK_NORMALIZE_LEVEL){
			norm += EPS_INK_NORMALIZE_LEVEL;
		}
	}
	return norm;
}
 

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   serGetSerialNo()                                                    */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* cdBuff       EPS_INT8*           I: 'cd' command reply                               */
/* buflen       EPS_INT32           I: 'cd' data length                                 */
/* serialNo     EPS_INT8*           O: serial number                                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPS_ERR_NONE                    - Success                                       */
/*                                                                                      */
/* Description:                                                                         */
/*      parse 'cd' command reply to get serial number string.                           */
/*                                                                                      */
/*******************************************|********************************************/
EPS_ERR_CODE    serGetSerialNo (

        EPS_INT8*       cdBuff,
		EPS_INT32       buflen,
        EPS_INT8*       serialNo

){
//    debug_msg("start call fucntion epson-escpr-services.c/serGetSerialNo \n");
	EPS_ERR_CODE ret = EPS_ERR_NONE;
	EPS_INT8*	p = NULL;
	EPS_INT32   datalen = 0;
	EPS_UINT8   formVer = 0;

#define EPS_GET_SERIAL(POS, LEN)				\
		if(datalen >= POS+LEN){					\
			strncpy(serialNo, p+POS, LEN);		\
		} else{									\
			ret = EPS_ERR_COMM_ERROR;			\
		}

	EPS_LOG_FUNCIN

	/* verify */
	if( buflen < 14 ){
		EPS_RETURN( EPS_ERR_COMM_ERROR)
	}
	if( 0 != memcmp(cdBuff+buflen-2, ";\x0C", 2) ){
		EPS_RETURN( EPS_ERR_COMM_ERROR)
	}

	p = strstr(cdBuff, "cd:");
	if( NULL == p || buflen-(p-cdBuff) < 6 ){
		EPS_RETURN( EPS_ERR_COMM_ERROR)
	}
	p += 5;	/* skip head */

	if( 0 == strncmp(p, "NA", 2) || 0 == strncmp(p, "BUSY", 4)){
		EPS_RETURN( EPS_ERR_COMM_ERROR)
	}

	datalen = (EPS_INT32)*p + ((EPS_INT32)*(p+1) << 8);
	p += 2;	/* skip lenght field */

	/* get version */
	formVer = 0x07;
	if( buflen-(EPS_INT32)(p-cdBuff) > datalen+2 &&		/* 2 = ";\x0C" */
		*(cdBuff+buflen-4) == '/'){
		formVer = *(cdBuff+buflen-3);
	}
	
	/* get serial number */
	/* serialNo buffer size is EPS_ADDR_BUFFSIZE */
	switch(formVer){
	case 0x07:
		/* EPS_GET_SERIAL(31, 18) not product serial */
		break;
	case 0x08:
	case 0x10:
		EPS_GET_SERIAL(31, 10)
		break;

	case 0x11:
	default:
		EPS_GET_SERIAL(46, 10)
		break;
	}

	EPS_DBGPRINT(("Ser : %s\n", serialNo));

//    debug_msg("Exit call fucntion epson-escpr-services.c/serGetSerialNo \n");
	EPS_RETURN( ret )
}


EPS_ERR_CODE    serGetSerialNoFormST (

        EPS_INT8*       Status,
        EPS_INT8*       serialNo,
		EPS_INT32       fieldSize

){
    EPS_UINT8*  Field;
    EPS_UINT8*  EndField;
    
    EPS_UINT8   Header;
    EPS_UINT8   ParameterByte;

	EPS_LOG_FUNCIN;

	memset(serialNo, 0, fieldSize);

	if((Field = (EPS_UINT8*)strstr(Status,"ST2")) == NULL ){
        SerDebugPrintf(("EPS SER: Get Stat -> ST not found [%s]\r\n",Status));
        EPS_RETURN( EPS_ERR_COMM_ERROR );
    }

	Field    = Field + 5;
    EndField = Field + (2+(*Field)+ ((*(Field+1))*256) );
    Field    = Field + 2;
    
    while ( Field < EndField ) {
        Header        = (EPS_UINT8) Field[0];
        ParameterByte = (EPS_UINT8) Field[1];

		if( 0x40 == Header ) {
			if( fieldSize > ParameterByte){
				memcpy(serialNo, Field+2, Min(ParameterByte, fieldSize-1));
			    EPS_RETURN( EPS_ERR_NONE );
			}
			break;
		}
        
        Field = Field + 2 + ParameterByte;
    }

    EPS_RETURN( EPS_ERR_OPR_FAIL );
}


/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   serDelayThread()                                                    */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* milliseconds EPS_UINT32          I: Sleep Period in microseconds                     */
/*                                                                                      */
/* Return value:                                                                        */
/*      None                                                                            */
/*                                                                                      */
/* Description:                                                                         */
/*      Wait <milliseconds>.                                                            */
/*      If OS sleep function is used, change the unit of sleep time from milliseconds   */
/*      to microseconds.                                                                */
/*                                                                                      */
/*******************************************|********************************************/
void    serDelayThread (

        EPS_UINT32		milliseconds,
		EPS_CMN_FUNC*	epsCmnFnc

){

#ifdef GCOMSW_EPSON_SLEEP
    if (epsCmnFnc->sleep == serSleep) {
        epsCmnFnc->sleep((EPS_UINT32)milliseconds); /* Ignore return value of sleep func */
    } else{
        epsCmnFnc->sleep(milliseconds * 1000);      /* Ignore return value of sleep func */
    }
#else
    epsCmnFnc->sleep(milliseconds * 1000);          /* Ignore return value of sleep func */
#endif /* GCOMSW_EPSON_SLEEP */

}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   serSleep()                                                          */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* sleepPeriod  EPS_UINT32          I: Sleep Period in milliseconds                     */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPS_ERR_NONE                    - Success                                       */
/*                                                                                      */
/* Description:                                                                         */
/*      ESC/P-R Lib original sleep function.                                            */
/*      This function is used when "epsCmnFnc.sleep = NULL".                            */
/*                                                                                      */
/*******************************************|********************************************/
#ifdef GCOMSW_EPSON_SLEEP
EPS_ERR_CODE    serSleep (

        EPS_UINT32  sleepPeriod             /* Sleep Period in milliseconds             */

){

/*** Declare Variable Local to Routine                                                  */
    EPS_INT32    idx;                       /* General loop/index varaible              */
    EPS_INT32    endx;
    struct timeb sleepM;

/*** Initialize Local Variables                                                         */
    endx = sleepPeriod*printJob.sleepSteps;
    if (printJob.sleepSteps <= 0) endx = sleepPeriod/(-printJob.sleepSteps);

/*** Sleep for about the requested sleepPeriod                                          */
    for (idx = 1; idx < endx; idx++) {ftime(&sleepM); } 

/*** Return to Caller                                                                   */
    return((EPS_ERR_CODE)EPS_ERR_NONE);

}
#endif /* GCOMSW_EPSON_SLEEP */


/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   serGetInkError()                                                    */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* pStatInfo    EPS_STATUS_INFO*    I: Printer Status Information                       */
/* pNotify      EPS_INT32*          O: Notification Code                                */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPS_ERR_NONE                    - Success                                       */
/*      EPS_ERR_OPR_FAIL                - Failed to operate                             */
/*                                                                                      */
/* Description:                                                                         */
/*      Set notification code about ink error                                           */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32    serGetInkError (

        EPS_STATUS_INFO*    pStatInfo,
        EPS_INT32*          pNotify

){
    SerDebugPrintf(("EPS SER : In Get Ink Error\r\n"));

    SerDebugPrintf(("EPS SER : In SP CTG %d,%d,%d,%d,%d,%d,%d,%d\r\n",
                                pStatInfo->nColor[0],
                                pStatInfo->nColor[1],
                                pStatInfo->nColor[2],
                                pStatInfo->nColor[3],
                                pStatInfo->nColor[4],
                                pStatInfo->nColor[5],
                                pStatInfo->nColor[6],
                                pStatInfo->nColor[7]));

    switch(pStatInfo->nInkError){
/*** Ink End                                                                            */
        case EPS_INKERR_INKENDALL:
        case EPS_INKERR_INKENDB:
        case EPS_INKERR_INKENDC:
            *pNotify  = EPS_PRNERR_INKOUT;
            break;

/*** Ink Fail                                                                           */
        case EPS_INKERR_CFAILALL:
        case EPS_INKERR_CFAILB:
        case EPS_INKERR_CFAILC:
            *pNotify = EPS_PRNERR_CFAIL;
            break;

/*** Ink Empty                                                                          */
        case EPS_INKERR_CEMPTYALL:
        case EPS_INKERR_CEMPTYB:
        case EPS_INKERR_CEMPTYC:
            *pNotify = EPS_PRNERR_CEMPTY;
            break;

        case EPS_INKERR_NONE:
            break;

        default:
            return EPS_ERR_OPR_FAIL;
    }
    
    return EPS_ERR_NONE;
}


/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   _SP_ChangeSpec_UpdatePMReply()                                      */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* printer      EPS_PRINTER_INN*    I/O: Pointer to a printer infomation                */
/* orgPmString  EPS_UINT8*          I: String of the pm command reply                   */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPS_ERR_NONE                    - Success                                       */
/*      EPS_ERR_SP_INVALID_POINTER      - Input pointer error                           */
/*      EPS_ERR_SP_INVALID_HEADER       - pm string header error                        */
/*      EPS_ERR_SP_INVALID_TERMINATOR   - pm string terminator error                    */
/*      EPS_ERR_SP_NO_VALID_FIELD       - pm string field error                         */
/*                                                                                      */
/* Description:                                                                         */
/*      - Invalid formats       : Delete                                                */
/*      - Unknown 'S' field     : Delete                                                */
/*      - Unknown 'T' field     : Replace to PGPP-Premium Glossy Photo Paper(id:0x0b)   */
/*                                field If PGPP aleady exist its 'S' field then just    */
/*                                combine the mode property                             */
/*      - Duplicated 'S' fields : Merge together                                        */
/*      - Duplicated 'T' fields : Merge together and combine each mode properties       */
/*      - Only DRAFT mode exist : Add NORMAL mode to its print quality property         */
/*                                                                                      */
/* NOTE:                                                                                */
/*      Be sure that the pData is a pointers that a starting address of 512 bytes       */
/*      buffer should be assigned or memory acces violation should be occured.          */
/*                                                                                      */
/*******************************************|********************************************/
EPS_ERR_CODE     _SP_ChangeSpec_UpdatePMReply (

		EPS_PRINTER_INN*    printer,
        EPS_UINT8*		    orgPmString,
        EPS_INT32		    bufSize

){

/*** Declare Variable Local to Routine                                                  */
    EPS_UINT8* pBefore = NULL;
    EPS_UINT8* pAfter  = NULL;
    EPS_UINT8* pSrc    = NULL;
    EPS_UINT8* pDes    = NULL;
    EPS_UINT8  tempPmString[EPS_PM_MAXSIZE];    /* Retrieved PM data from printer       */
    EPS_UINT8  paperSize;
    EPS_UINT8  paperType;
#if _VALIDATE_SUPPORTED_MEDIA_DATA_
	EPS_UINT8  PmStringForCompare[EPS_PM_MAXSIZE];    /* Retrieved PM data from printer       */
#endif    
    static const EPS_UINT8 PM_REPLY_HEADER[EPS_PM_HEADER_LEN] = {
      /*  @     B     D     C   <SP>    P     M   <CR>  <LF> */
        0x40, 0x42, 0x44, 0x43, 0x20, 0x50, 0x4D, 0x0D, 0x0A
    };
        
    EPS_INT16 idx;

	EPS_LOG_FUNCIN;

/*** Validate input parameters                                                          */
    if(orgPmString == NULL) {
        SerDebugPrintf(("_SP_ChangeSpec_UpdatePMReply > EPS_ERR_SP_INVALID_POINTER\r\n"));
        EPS_RETURN( EPS_ERR_SP_INVALID_POINTER );
    }

    if(memcmp(orgPmString, PM_REPLY_HEADER, EPS_PM_HEADER_LEN) != 0) {
        SerDebugPrintf(("_SP_ChangeSpec_UpdatePMReply > EPS_ERR_SP_INVALID_HEADER\r\n"));
        EPS_RETURN( EPS_ERR_SP_INVALID_HEADER );
    }

    for(idx = EPS_PM_HEADER_LEN; idx <= (EPS_PM_MAXSIZE-EPS_PM_TERMINATOR_LEN); idx++) {
        if(orgPmString[idx]== 0x0D && orgPmString[idx+1] == 0x0A) {
            break;
        }
    }

    if(idx > (EPS_PM_MAXSIZE-EPS_PM_TERMINATOR_LEN)) {
        SerDebugPrintf(("_SP_ChangeSpec_UpdatePMReply > EPS_ERR_SP_INVALID_TERMINATOR\r\n"));
        EPS_RETURN( EPS_ERR_SP_INVALID_TERMINATOR );
    }

/*** Initialize Local Variables                                                         */
    memset(tempPmString, 0x00, EPS_PM_MAXSIZE);
#if _VALIDATE_SUPPORTED_MEDIA_DATA_
    memset(PmStringForCompare, 0x00, EPS_PM_MAXSIZE);
#endif

    /* Initialize pm data state */
    memset(printer->pmData.pmString, 0x00, EPS_PM_MAXSIZE);
    printer->pmData.state = EPS_PM_STATE_NOT_FILTERED;

/*** Correct PM REPLY following 7 steps                                                 */
/*** ---------------------------------------------------------------------------------- */
/*** STEP 1 : Replace Built-in resource. Because PM REPLY of the following printers     */
/***          is insufficient,                                                          */
/*** ---------------------------------------------------------------------------------- */
	if(        (strcmp(printer->modelName, "E-300" ) == 0) ){
		bufSize = _SP_LoadPMString(EPS_PMS_E300, orgPmString, EPS_PM_MAXSIZE);

	} else if( (strcmp(printer->modelName, "E-500" ) == 0) ||
		       (strcmp(printer->modelName, "E-700" ) == 0) ) {
        bufSize = _SP_LoadPMString(EPS_PMS_E500, orgPmString, EPS_PM_MAXSIZE);

	} else if( (strcmp(printer->modelName, "PictureMate PM 200" ) == 0) ||
		       (strcmp(printer->modelName, "PictureMate PM 210" ) == 0) ){
        bufSize = _SP_LoadPMString(EPS_PMS_PM200, orgPmString, EPS_PM_MAXSIZE);

	} else if( (strcmp(printer->modelName, "PictureMate PM 240" ) == 0) ||
		       (strcmp(printer->modelName, "PictureMate PM 250" ) == 0) ||
			   (strcmp(printer->modelName, "PictureMate PM 280" ) == 0) ){
        bufSize = _SP_LoadPMString(EPS_PMS_PM240, orgPmString, EPS_PM_MAXSIZE);
	}
	if(bufSize <= 0){
		return bufSize;
	}

	
	/* Use work pointers to call each filter functions */
    pBefore = orgPmString + EPS_PM_HEADER_LEN;             /* position of first tab 'S' */
    pAfter  = tempPmString;
	bufSize -= EPS_PM_HEADER_LEN;

/*** ---------------------------------------------------------------------------------- */
/*** STEP 2 : Remove <CR><LF> on the way                                                */
/*** ---------------------------------------------------------------------------------- */
    pSrc = pBefore;
    pDes = pAfter;

    DUMP_PMREPLY((pSrc, DUMP_HEX, "< ORIGINAL >"));

    _pmValidateRemoveDelimiter(pDes, pSrc, bufSize);

#if _VALIDATE_SUPPORTED_MEDIA_DATA_
	if(memcmp(pBefore, pAfter, EPS_PM_DATA_LEN) != 0){
		printf("!!!!!!!!! PM reply data modified on STEP 2. !!!!!!!!!\nRemove <CR><LF> on the way\n\n");
	}
#endif
    /* Update orgPmString                                                               */
	memcpy(pBefore, pAfter, EPS_PM_DATA_LEN);

    VERBOSE_DUMP_PMREPLY((pDes, DUMP_ASCII, "< STEP 1 PASSED >"));
	
/*** ---------------------------------------------------------------------------------- */
/*** STEP 3 : Copy only valid fields to reply buffer and remove unknown 'S' from reply  */
/*** ---------------------------------------------------------------------------------- */
    pSrc = pBefore;
    pDes = pAfter;

    DUMP_PMREPLY((pSrc, DUMP_HEX, "< ORIGINAL >"));

	if(_pmValidateRemoveUnknownSfield(pDes, pSrc) == 0) {
        SerDebugPrintf(("_SP_ChangeSpec_UpdatePMReply > EPS_ERR_SP_NO_VALID_FIELD\r\n"));
        EPS_RETURN( EPS_ERR_SP_NO_VALID_FIELD );
    }

#if _VALIDATE_SUPPORTED_MEDIA_DATA_
	if(memcmp(pBefore, pAfter, EPS_PM_DATA_LEN) != 0){
		printf("!!!!!!!!! PM reply data modified on STEP 3. !!!!!!!!!\n\n\n");
	    print_PMREPLY(pAfter, DUMP_HEX, "< Filterd >");
	}
#endif
    /* Update orgPmString                                                               */
	memcpy(pBefore, pAfter, EPS_PM_DATA_LEN);

    VERBOSE_DUMP_PMREPLY((pDes, DUMP_ASCII, "< STEP 1 PASSED >"));

/*** ---------------------------------------------------------------------------------- */
/*** STEP 4 : Correct unknown 'T' fields                                                */
/*** ---------------------------------------------------------------------------------- */
    pSrc = pBefore;
    pDes = pAfter;

    _pmCorrectUnknownTfield(pDes, pSrc);
#if _VALIDATE_SUPPORTED_MEDIA_DATA_
	if(memcmp(pBefore, pAfter, EPS_PM_DATA_LEN) != 0){
		printf("!!!!!!!!! PM reply data modified on STEP 4. !!!!!!!!!\n\n\n");
	    print_PMREPLY(pAfter, DUMP_HEX, "< Filterd >");
	}
#endif

    /* Update orgPmString                                                               */
	memcpy(pBefore, pAfter, EPS_PM_DATA_LEN);

	VERBOSE_DUMP_PMREPLY((pDes, DUMP_ASCII, "< STEP 2 PASSED >"));

/*** ---------------------------------------------------------------------------------- */
/*** STEP 5 : Merge duplicated fields                                                   */
/*** ---------------------------------------------------------------------------------- */
    pSrc = pBefore;
    pDes = pAfter;

    _pmCorrectDupulicatedFields(pDes, pSrc);
#if _VALIDATE_SUPPORTED_MEDIA_DATA_
	if(memcmp(pBefore, pAfter, EPS_PM_DATA_LEN) != 0){
		printf("!!!!!!!!! PM reply data modified on STEP 5. !!!!!!!!!\n\n\n");
	    print_PMREPLY(pAfter, DUMP_HEX, "< Filterd >");
	}
#endif

    /* Update orgPmString                                                               */
	memcpy(pBefore, pAfter, EPS_PM_DATA_LEN);

	VERBOSE_DUMP_PMREPLY((pDes, DUMP_ASCII, "< STEP 3 PASSED >"));

    /* Now, Service Pack retains filtered data its original quality properties */
    /* within the inner buffer g_PMinfo.data */
    /* This data would be referenced whenever it is required to compare its originality */
    DUMP_PMREPLY((orgPmString, DUMP_ASCII, \
                  "< FILTERED (Retained within SP-same printer's caps) >"));


/*** ---------------------------------------------------------------------------------- */
/*** STEP 6 : Delete the paper type "CD/DVD label" from the pm string when "Stylus      */
/***          Photo R380" or "Stylus Photo RX580" is used.                              */
/*** ---------------------------------------------------------------------------------- */
	if ((strcmp(printer->modelName, "Stylus Photo R380" ) == 0) ||
        (strcmp(printer->modelName, "Stylus Photo RX580") == 0)    ) {

	    pSrc = pBefore;
        pDes = pAfter;
        paperSize = 0xFF;
        paperType = 0xFF;

        while (*pSrc == 'S') {
            paperSize = *(pSrc + 1);        /* Save the media size                      */

            *pDes++ = *pSrc++;            /* set 'S'                                  */
            *pDes++ = *pSrc++;            /* set the meida size                       */

            while (*pSrc == 'T') {
                paperType = *(pSrc + 1);    /* Save the media type                      */
                
                if ((paperSize == EPS_MSID_LETTER) && (paperType == EPS_MTID_CDDVD)) {
                    pSrc += 4;              /* Move to next 'T'                         */
                } else{
                    *pDes++ = *pSrc++;    /* set 'T'                                  */
                    *pDes++ = *pSrc++;    /* set the media type                       */
                    *pDes++ = *pSrc++;    /* set the printing mode info               */
                    *pDes++ = *pSrc++;    /* set '/'                                  */
                }
            }
            if (*pSrc == '/') {
                *pDes++ = *pSrc++;        /* set '/'                                  */
            }

            /* check for string termination                                                 */
            if ((*pSrc == 0xD) && (*(pSrc+1) == 0xA)) {
                *pDes++ = *pSrc++;
                *pDes++ = *pSrc++;
                break;
            }
        }

        /* Update orgPmString                                                               */
        memcpy(pBefore, pAfter, EPS_PM_DATA_LEN);
    }

/*** ---------------------------------------------------------------------------------- */
/*** STEP 7 : Adjust quality properties to the formal in order to return to the driver. */
/***          it dose not change the filtered data through previous steps retained      */
/***          within Service Pack. but just change the buffer asigned as parameter.     */
/***          (in this case orgPmString)                                                */
/***          after duplicating the filtered data to it.                                */
/*** ---------------------------------------------------------------------------------- */
    /* set filterd value "printer->pmData.pmString" */
    memset(printer->pmData.pmString, 0x00, EPS_PM_MAXSIZE);
    memcpy(printer->pmData.pmString, orgPmString, EPS_PM_MAXSIZE);

	printer->pmData.state = EPS_PM_STATE_FILTERED;
#if _VALIDATE_SUPPORTED_MEDIA_DATA_
	memcpy(PmStringForCompare, orgPmString, EPS_PM_DATA_LEN);
#endif

    /* If Draft Quality is only supported, add Normal Quality */
    _pmAdjustQuality(orgPmString);
#if _VALIDATE_SUPPORTED_MEDIA_DATA_
	if(memcmp(PmStringForCompare, orgPmString, EPS_PM_DATA_LEN) != 0){
		printf("!!!!!!!!! PM reply data modified on STEP 7. !!!!!!!!!\n\n\n");
	    print_PMREPLY(PmStringForCompare + EPS_PM_HEADER_LEN, DUMP_HEX, "< Origin >");
	    print_PMREPLY(orgPmString + EPS_PM_HEADER_LEN, DUMP_HEX, "< Filterd >");
	}
#endif
	
	DUMP_PMREPLY((orgPmString+EPS_PM_HEADER_LEN, DUMP_ASCII, \
                  "< FILTERED (Returned data to the driver-adjusted quality properties) >"));

/*** Return to caller                                                                   */
    EPS_RETURN( EPS_ERR_NONE );
}


/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   _SP_ChangeSpec_DraftOnly()                                          */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* printer      EPS_PRINTER_INN*    I/O: Pointer to a printer infomation                */
/* jobAtter     EPS_JOB_ATTRIB*     I: Data structure containing page attribut settings */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPS_ERR_NONE                    - Success                                       */
/*                                                                                      */
/* Description:                                                                         */
/*      If the quality mode which is not supported by printer is assigned, replace it   */
/*      to printer's support mode.                                                      */
/*                                                                                      */
/*******************************************|********************************************/
EPS_ERR_CODE     _SP_ChangeSpec_DraftOnly (

		EPS_PRINTER_INN*    printer,
        EPS_JOB_ATTRIB*     jobAtter          /* Print Attributes for this Job         */

){
/*** Declare Variable Local to Routine                                                  */
    EPS_UINT8  mediaSizeID;
    EPS_UINT8  mediaTypeID;
    EPS_UINT8* pPMinfo;
    EPS_UINT8* pS_begin;
    EPS_UINT8* pS_end;
    EPS_UINT8* pTfield;
    EPS_UINT8  quality[3];      /* Q_DRAFT / Q_NORMAL / Q_HIGH                          */

	EPS_LOG_FUNCIN;

/*** Validate input parameters                                                          */
    if(printer->pmData.state != EPS_PM_STATE_FILTERED) {
        SerDebugPrintf(("ChangeSpec_DraftOnly : PM info not initialized\r\n"));
        /* it is not able to hadle this situation so do nothing */
        EPS_RETURN( EPS_ERR_NONE ); 
    }

/*** Initialize Global/Local Variables                                                  */
    mediaSizeID = (EPS_UINT8)jobAtter->mediaSizeIdx;
    mediaTypeID = (EPS_UINT8)jobAtter->mediaTypeIdx;
    pPMinfo     = NULL;
    pS_begin    = NULL;
    pS_end      = NULL;
    pTfield     = NULL;
	memset(quality, 0, sizeof(quality));

/*** Change quality                                                                     */
    /* Refer the data retained within Service Pack. */
    /* "printer->pmData.pmString" has the header.   */
    pPMinfo = (EPS_UINT8*)(printer->pmData.pmString + EPS_PM_HEADER_LEN);

    /* S field start postion */
    if(_pmFindSfield(mediaSizeID, pPMinfo, &pS_begin, &pS_end) < 0) {
        SerDebugPrintf(("ChangeSpec_DraftOnly : cannot find mediaSizeID(%d)\r\n", mediaSizeID));
        /* it is not able to hadle this situation so do nothing */
        EPS_RETURN( EPS_ERR_NONE ); 
    };

    VERBOSE_DUMP_PMREPLY((pS_begin, DUMP_S_TAG_ONLY, 
            "< ChangeSpec_DraftOnly : retained S field info >"));

    /* Fetch the T field */
    if((pTfield = _pmScanTfield(mediaTypeID, pS_begin)) == NULL) {
        SerDebugPrintf(("ChangeSpec_DraftOnly : cannot find mediaTypeID(%d)\r\n", mediaTypeID));
        /* it is not able to hadle this situation so do nothing */
        EPS_RETURN( EPS_ERR_NONE ); 
    }

    /* Quality should be assigned to the only supported mode */
    verbose_dbprint((" >> adjusted PrintQuality : %d -> ", jobAtter->printQuality));

	if(!((*(pTfield+2) & 0x07) &   /* Printer's support mode actually */
        (jobAtter->printQuality))) { /* Upper layer(driver) assigned mode */

        /* The quality mode which is not supported by printer is assigned */
        /* Replace it to printer's support mode */ 
        switch(*(pTfield+2) & 0x07) {
            case 0x01: /* 0 0 1 : Draft  only       */
                quality[Q_DRAFT]  = EPS_MQID_DRAFT;
                quality[Q_NORMAL] = EPS_MQID_DRAFT;
                quality[Q_HIGH]   = EPS_MQID_DRAFT;
                break;
            case 0x02: /* 0 1 0 : Normal only       */
                quality[Q_DRAFT]  = EPS_MQID_NORMAL;
                quality[Q_NORMAL] = EPS_MQID_NORMAL;
                quality[Q_HIGH]   = EPS_MQID_NORMAL;
                break;
            case 0x04: /* 1 0 0 : High   only       */
                quality[Q_DRAFT]  = EPS_MQID_HIGH;
                quality[Q_NORMAL] = EPS_MQID_HIGH;
                quality[Q_HIGH]   = EPS_MQID_HIGH;
                break;
            case 0x03: /* 0 1 1 : Normal and Draft  */
                quality[Q_DRAFT]  = EPS_MQID_DRAFT;
                quality[Q_NORMAL] = EPS_MQID_NORMAL;
                quality[Q_HIGH]   = EPS_MQID_NORMAL;
                break;
            case 0x05: /* 1 0 1 : High   and Draft  */
                quality[Q_DRAFT]  = EPS_MQID_DRAFT;
                quality[Q_NORMAL] = EPS_MQID_HIGH;
                quality[Q_HIGH]   = EPS_MQID_HIGH;
                break;
            case 0x06: /* 1 1 0 : High   and Normal */
                quality[Q_DRAFT]  = EPS_MQID_NORMAL;
                quality[Q_NORMAL] = EPS_MQID_NORMAL;
                quality[Q_HIGH]   = EPS_MQID_HIGH;
                break;
            case 0x07: /* 1 1 1 : Anything possible */
                break;
            default: 
                break;
        }

        /* Now, the value of quality array of index which is same as PrintQuality is valid */
		switch(jobAtter->printQuality) {
			case EPS_MQID_DRAFT:
				jobAtter->printQuality= quality[Q_DRAFT];
				break;
			case EPS_MQID_NORMAL:
				jobAtter->printQuality= quality[Q_NORMAL];
				break;
			case EPS_MQID_HIGH:
				jobAtter->printQuality= quality[Q_HIGH];
				break;
		}
    }

    verbose_dbprint(("%d\r\n", jobAtter->printQuality));

    EPS_RETURN( EPS_ERR_NONE );
}


/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   serAppendMedia()                                                    */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:                Description:                                       */
/* pMedia       EPS_SUPPORTED_MEDIA* I/O: supported media structuer                     */
/*                                                                                      */
/* Return value:                                                                        */
/*      none                                                                            */
/*                                                                                      */
/* Description:                                                                         */
/*      Append Media attribute from PM reply info.                                      */
/*                                                                                      */
/*******************************************|********************************************/
void     serAppendMedia (

        EPS_SUPPORTED_MEDIA*  pMedia

){
    EPS_INT32       idxSize, idxType;
    EPS_BOOL        foundCD = FALSE;

	EPS_LOG_FUNCIN;

	for(idxSize = 0; idxSize < pMedia->numSizes; idxSize++){
		/*** Append "CD/DVD Label" rayout to plain, CDDVD ***/
		for(idxType = 0; idxType < pMedia->sizeList[idxSize].numTypes; idxType++){

			if( EPS_IS_CDDVD( pMedia->sizeList[idxSize].typeList[idxType].mediaTypeID ) ){
				/* Set "CD/DVD Label" rayout to CDDVD */
				pMedia->sizeList[idxSize].typeList[idxType].layout = EPS_MLID_CDLABEL;

				/* paperSource is only CD tray */
				pMedia->sizeList[idxSize].typeList[idxType].paperSource = EPS_MPID_CDTRAY;
				
				foundCD = TRUE;
			} else if(EPS_MTID_MINIPHOTO == pMedia->sizeList[idxSize].typeList[idxType].mediaTypeID){
				/* Append "16 Division" rayout to Photo Stickers */
				pMedia->sizeList[idxSize].typeList[idxType].layout |= EPS_MLID_DIVIDE16;
			}
		}

		if(foundCD){
			for(idxType = 0; idxType < pMedia->sizeList[idxSize].numTypes; idxType++){
				if(EPS_MTID_PLAIN == pMedia->sizeList[idxSize].typeList[idxType].mediaTypeID ){
					/* Append "CD/DVD Label" rayout to plain */
					pMedia->sizeList[idxSize].typeList[idxType].layout |= EPS_MLID_CDLABEL;
					break;
				}
			}
			foundCD = FALSE;
		}
	}

	EPS_RETURN_VOID
}


EPS_UINT32  serComplementPaperSource (

        EPS_INT32  mediaSizeID

){
	switch(mediaSizeID){
	case EPS_MSID_A4:
	case EPS_MSID_LETTER:
	case EPS_MSID_LEGAL:
		return EPS_MPID_FRONT1;
		break;
	default:
		if(EPS_MSID_KAKU_2 <= mediaSizeID){
			return EPS_MPID_FRONT1;
		}
	}

	return EPS_MPID_FRONT2;
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%                                             %%%%%%%%%%%%%%%%%%%%%*/
/*--------------------               Local Functions               ---------------------*/
/*%%%%%%%%%%%%%%%%%%%%                                             %%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   _SP_LoadPMString()                                                  */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* resouceID    EPS_UINT32          I: PM resouce ID                                    */
/* pString      EPS_INT8*           O: PM string                                        */
/* bufSize      EPS_INT32           I: pString size                                     */
/*                                                                                      */
/* Return value:                                                                        */
/* EPS_INT32    buffer size or error                                                    */
/*                                                                                      */
/* Description:                                                                         */
/*      Load special PM string. Because a part of model is insufficient PM reply info.  */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32  _SP_LoadPMString (

        EPS_UINT32  resouceID,
        EPS_UINT8*  pString,
		EPS_UINT32  bufSize

){
    EPS_UINT32 i = 0;

	for (i = 0; i < EPS_SPM_STRINGS; i++){
		if (spPMStrTbl[i].id == resouceID){
			if(bufSize < spPMStrTbl[i].len){
				return EPS_ERR_OPR_FAIL;
			}
			memcpy(pString, spPMStrTbl[i].res, spPMStrTbl[i].len);
			return spPMStrTbl[i].len;
		}
	}

	return EPS_ERR_OPR_FAIL;
}


/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   _pmFindSfield()                                                     */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* id           EPS_UINT8           I: Media Size ID                                    */
/* pSrc         EPS_UINT8*          I: pm String                                        */
/* pStart       EPS_UINT8**         O: Start Pointer of 'S' fields                      */
/* pEnd         EPS_UINT8**         O: End Pointer of 'S' fields                        */
/*                                                                                      */
/* Return value:                                                                        */
/*      Length of founded 'S' fields    - Success                                       */
/*      -1                              - There is NOT the Media Size ID in pm string   */
/*                                                                                      */
/* Description:                                                                         */
/*      Find a 'S' field that includes the <id> in <pSrc> and save its starting('S')    */
/*      and ending pointer('/') to <pStart> and <pEnd>.                                 */
/*      <pSrc> should be a complete PM REPLY format that start with 'S' and terminate   */
/*      at "0x0D 0x0A".                                                                 */
/*                                                                                      */
/*******************************************|********************************************/
static EPS_INT32    _pmFindSfield (

        EPS_UINT8   id,
        EPS_UINT8*  pSrc,
        EPS_UINT8** pStart,
        EPS_UINT8** pEnd

){
	EPS_LOG_FUNCIN;

    while (*pSrc != 0xD || *(pSrc+1) != 0xA) {

        *pStart = NULL;
        *pEnd   = NULL;

        /* find 'S' */
        while(*pSrc == 'S') {
            if(id == *(pSrc+1)) {
                *pStart = pSrc;
            }

            pSrc += 2;

            while(*pSrc == 'T') {
                pSrc += 4;
            }

            /* Found id */
            if(*pStart != NULL) {
                *pEnd = pSrc;
                EPS_RETURN( (EPS_INT32)(*pEnd - *pStart)+1 );
            }

            /* next 'S' */
            pSrc++;
        }

		if(*pSrc == 'M' || *pSrc == 'R'){
			pSrc += 6;
		}
    }

    EPS_RETURN( (-1) );
}


/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   _pmScanTfield()                                                     */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* id           EPS_UINT8           Media Type ID                                       */
/* pSfield      EPS_UINT8*          Pointer to 'S' field                                */
/*                                                                                      */
/* Return value:                                                                        */
/*      Pointer to 'T' on the pSfield   - Success                                       */
/*      NULL                            - There is NOT 'T' in the pSfield               */
/*                                                                                      */
/* Description:                                                                         */
/*      Find 'T' field that includs the <id>.                                           */
/*                                                                                      */
/*******************************************|********************************************/
static EPS_UINT8*   _pmScanTfield (

        EPS_UINT8   id,
        EPS_UINT8*  pSfield

){
    EPS_UINT8* pScan = pSfield;
    EPS_UINT8* pT    = NULL;

	if(*pScan == 'S') {
        pScan += 2;

        while(*pScan == 'T') {
            if(id == *(pScan+1)) {
                pT = pScan;
                break;
            }

            pScan += 4;
        }
    }

    return pT;

}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   _pmAppendTfield()                                                   */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* pT           EPS_UINT8*          I: Pointer to 'T' field                             */
/* pDes         EPS_UINT8*          O: Pointer to 'S' field                             */
/*                                                                                      */
/* Return value:                                                                        */
/*      4                               - Success (Return increased bytes)              */
/*      -1                              - Failure                                       */
/*                                                                                      */
/* Description:                                                                         */
/*      Append 'T' field to <pDes> if same field dose not exsit, but same one aleady    */
/*      exsits just combine mode properdy.                                              */
/*      <pDes> should have a complete 'S' field consist of 'S' and '/' and pT should    */
/*      have a 'T' field of 4 bytes starts with 'T'.                                    */
/*      This function returns the increased bytes so that caller change the last        */
/*      position or (-1) to indicate nothing changed.                                   */
/*                                                                                      */
/*******************************************|********************************************/
static EPS_INT16    _pmAppendTfield (

        EPS_UINT8*  pT,
        EPS_UINT8*  pDes

){
    EPS_UINT8 t_id    = *(pT+1);
    EPS_INT16 t_exist = 0;

	if(*pDes == 'S') {

        pDes += 2; /* move to first 'T' */

        while(*pDes == 'T') {

            /* same id exist */
            if(t_id == *(pDes+1)) {
                /* Just combine mode property */
                *(pDes+2) |= *(pT+2);

                t_exist = 1;
                break;
            }

            /* next 'T' */
            pDes += 4;
        }

        /* samd id field dose not exist */
        /* Append new 'T' fields */
        if(t_exist == 0) {
            memcpy(pDes, pT, 4);
            pDes += 4;
            *pDes = '/';

            return 4; /* size of 'T' field */
        }

        /* type id aleady exist then do not anything */
    }

    return (-1);

}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   _pmValidateRemoveDelimiter()                                        */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* pDes         EPS_UINT8*          O: Pointer to validated pm string                   */
/* pSrc         EPS_UINT8*          I: Pointer to original pm string                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      The number of valid fields                                                      */
/*                                                                                      */
/* Description:                                                                         */
/*      Copy valid fields to reply buffer only.                                         */
/*      Remove <CR><LF> on the way.                                                     */
/*                                                                                      */
/*******************************************|********************************************/
static void    _pmValidateRemoveDelimiter (

        EPS_UINT8*  pDes,
        EPS_UINT8*  pSrc, 
		EPS_INT32   bufSize

){

    EPS_UINT8* pEOF  = pSrc + bufSize;

	EPS_LOG_FUNCIN;

    while (pSrc < pEOF) {

        if(*pSrc == 'S') {
			memcpy(pDes, pSrc, 2);
            pSrc += 2;
			pDes += 2;

            while(*pSrc == 'T') {
				memcpy(pDes, pSrc, 3);
                pSrc += 3;
				pDes += 3;

                if(*pSrc == '/') {
                    *pDes++ = *pSrc++;
                }
            }

            if(*pSrc == '/') {
                *pDes++ = *pSrc++;
            }

		} else if(*pSrc == 'M' || *pSrc == 'R'){
			/* Jpeg size limit */ 
			if( *(pSrc + 5) == '/' ){
				memcpy(pDes, pSrc, 6 );
				pSrc += 6;
				pDes += 6;
			}
		} else if(*pSrc == 0xD || *(pSrc+1) == 0xA){
			/* Terminater skip */
			pSrc += 2;
		} else{
			/* unknown field */
	        pSrc++;
		}
    }

	/* set truth terminater */
    *pDes++ = 0x0d;   /* 0xD */
    *pDes   = 0x0a;   /* 0xA */

    EPS_RETURN_VOID;
}


/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   _pmValidateRemoveUnknownSfield()                                    */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* pDes         EPS_UINT8*          O: Pointer to validated pm string                   */
/* pSrc         EPS_UINT8*          I: Pointer to original pm string                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      The number of valid fields                                                      */
/*                                                                                      */
/* Description:                                                                         */
/*      Copy valid fields to reply buffer only.                                         */
/*      Remove unknown 'S' field.                                                       */
/*      Minimum conditons for valid PM REPLY are                                        */
/*       - it must have a complete 'S' field more than one ( 'S' ~ '/').                */
/*       - it must end with 0xD and 0xA.                                                */
/*                                                                                      */
/*******************************************|********************************************/
static EPS_INT16    _pmValidateRemoveUnknownSfield (

        EPS_UINT8*  pDes,
        EPS_UINT8*  pSrc

){

    EPS_UINT8* pPrev = NULL;    /* save previous pointer                                */
    EPS_UINT8* pS    = NULL;    /* valid field's starting position                      */
    EPS_UINT8* pE    = NULL;    /* valid field's ending postion                         */

    EPS_INT16  valid = 0;       /* flag for indicating 'S' field's validation           */
    EPS_INT16  t_cnt = 0;       /* count valid 'T' fields                               */
    EPS_UINT16 s_idx = 0;       /* index of epsMediaSize                                */

    EPS_INT16  num_valid_fields = 0;    /* value for returning                          */

	EPS_LOG_FUNCIN;

#ifdef _TEST_PM_STEP_1 /* Change first 'S' field's id to unknown id such as 0xFF */
    *(pSrc+1) = 0xFF;
#endif

    while (*pSrc != 0xD || *(pSrc+1) != 0xA) {
        pPrev = pSrc;

        pS    = NULL;
        pE    = NULL;
        valid = 0;
        t_cnt = 0;
        s_idx = 0;

        if(*pSrc == 'S') {
            pS    = pSrc;
            pSrc += 2;

            while(*pSrc == 'T') {
                pSrc += 3;

                if(*pSrc == '/') {
                    pSrc++;
                    t_cnt++;
                }
            }

            if(t_cnt && *pSrc == '/') {
                pE = pSrc;
            }

		} else if(*pSrc == 'M' || *pSrc == 'R'){
			/* Jpeg size limit */ 
			if( *(pSrc + 5) == '/' ){
				memcpy(pDes, pSrc, 6 );
				pDes += 6;
				pSrc += 6;
				continue;
			}
		}

        /* Copy valid and support 'S' fields only */
        /* Valid means size id should be find in its table */
        /* and 'T' field exist at least more than one */
        /* Unknown 'S' field should be removed */
        if(pS && pE) {
            for(s_idx = 0; s_idx < EPS_NUM_MEDIA_SIZES; s_idx++) {
                if(epsMediaSize[s_idx].id == *(pS+1)) {
                    memcpy(pDes, pS, (EPS_UINT32)((pE-pS)+1) );
                    pDes += (pE-pS)+1;
                    valid = 1;

                    /* now increase num of valid fields */
                    num_valid_fields++;

                    break;
                }
            }
        }

        /* Restore work buffer pos to the previous */
        /* cause fail to get a valid fields */
        if(valid == 0) {
            pSrc = pPrev;
        }

        pSrc++;
    }

    *pDes++ = *pSrc++;   /* 0xD */
    *pDes++ = *pSrc;     /* 0xA */

    EPS_RETURN( num_valid_fields );

}


/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   _pmCorrectUnknownTfield()                                           */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* pDes         EPS_UINT8*          O: Pointer to validated pm string                   */
/* pSrc         EPS_UINT8*          I: Pointer to original pm string                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      None                                                                            */
/*                                                                                      */
/* Description:                                                                         */
/*      Change an unknown 'T' field to PGPP's in case that PGPP dose not exist in       */
/*      'S' field. If aleady PGPP exist delete it.                                      */
/*                                                                                      */
/*******************************************|********************************************/
static void     _pmCorrectUnknownTfield (

        EPS_UINT8*  pDes,
        EPS_UINT8*  pSrc

){

    static const EPS_UINT8 PGPP_FIELD [ ] = { 0x54, 0x0B, 0x87, 0x2F };

    EPS_INT16 PGPP    = 0;      /* Premium Glossy Photo Paper (type id : 0x0b)          */
    EPS_UINT16 t_idx  = 0;      /* Index of table defined Support 'T' id table          */
    EPS_UINT8 * pScan = NULL;   /* word pointer for scanning id                         */

	EPS_LOG_FUNCIN;

#ifdef _TEST_PM_STEP_2 /* Change 'T' field's id to unknown id such as 0xFF */
    *(pSrc+3) = 0xFF;
#endif

    while (*pSrc != 0xD || *(pSrc+1) != 0xA) {
        /* reset PGPP flag each new 'S' field */
        PGPP = 0;

        if(*pSrc == 'S') {
            /* Scan PGPP in current 'S' field */
            pScan = pSrc;

            if(_pmScanTfield(EPS_MTID_PGPHOTO, pScan) != NULL) {
                PGPP = 1;
            }

            *pDes++ = *pSrc++;
            *pDes++ = *pSrc++;

            while(*pSrc == 'T') {
                /* Copy support 'T' field */
                for(t_idx = 0; t_idx < EPS_NUM_MEDIA_TYPES; t_idx++) {
                    if(epsMediaTypeIndex[t_idx] == *(pSrc+1)) {
                        memcpy(pDes, pSrc, 4);
                        pDes += 4;
                        break;
                    }
                }

                /* Unknown type id encountered */
                /* if PGPP did not exist in 'S' field */
                /* then append PGPP fields to pDes */
                if(t_idx == EPS_NUM_MEDIA_TYPES && PGPP == 0) {
                    memcpy(pDes, PGPP_FIELD, 4);
                    pDes += 4;
                    PGPP  = 1;
                }

                /* move to next 'T' */
                pSrc += 4;
            }

            /* copy '/' and move next 'S' */
            *pDes++ = *pSrc++;

		}else if(*pSrc == 'M' || *pSrc == 'R') {
			memcpy(pDes, pSrc, 6);
			pDes += 6;
			pSrc += 6;
		}
    }

    *pDes++ = *pSrc++;   /* 0xD */
    *pDes   = *pSrc;     /* 0xA */

    EPS_RETURN_VOID;
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   _pmCorrectDupulicatedFields()                                       */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* pDes         EPS_UINT8*          O: Pointer to validated pm string                   */
/* pSrc         EPS_UINT8*          I: Pointer to original pm string                    */
/*                                                                                      */
/* Return value:                                                                        */
/*      None                                                                            */
/*                                                                                      */
/* Description:                                                                         */
/*      Merge duplicated fields.                                                        */
/*                                                                                      */
/*******************************************|********************************************/
static void     _pmCorrectDupulicatedFields (

        EPS_UINT8*  pDes,
        EPS_UINT8*  pSrc

){
    EPS_UINT8 merged_buf[EPS_PM_MAXSIZE];

    EPS_UINT8* pFieldS = NULL;      /* current 'S' in merged buffer                     */
    EPS_UINT8* pFieldT = NULL;      /* work pontter to merge a 'T'                      */
    EPS_UINT8* pS      = NULL;      /* duplicated field's starting position             */
    EPS_UINT8* pE      = NULL;      /* duplicated field's ending postion                */
    EPS_UINT8* pM      = NULL;      /* pos of merged buffer                             */
    EPS_UINT8* pScan   = NULL;      /* work pointer to find a field                     */
    EPS_UINT8  s_id    = 0xFF;      /* current 'S' id                                   */
    EPS_INT16  bytes;

	EPS_LOG_FUNCIN;

#ifdef _TEST_PM_STEP_3
    *(pSrc+8) = 0x0F; /* make duplicate 'S' */
#endif
	memset(merged_buf, 0, EPS_PM_MAXSIZE);
    pM = &merged_buf[0];

    /* Aleady merged fields no need to copy again */
    while (*pSrc != 0xD || *(pSrc+1) != 0xA) {
        pFieldS = NULL;

        if(*pSrc == 'S') {
            VERBOSE_DUMP_PMREPLY((pSrc, DUMP_S_TAG_ONLY, "< STEP 3 : SOURCE 'S' ... >"));

            /* save current 'S' id */
            s_id = *(pSrc+1);

            if(s_id != MERGED_FIELD) {
                /* Current 'S' field's starting pos */
                /* it is used to merge fields later */
                pFieldS = pM;

                COPY_BYTES(pM, pSrc, 2);
            }

            pSrc += 2; /* move to first 'T' */

            /* Merge 'T' fields */
            while(*pSrc == 'T') {

                if(pFieldS && s_id != MERGED_FIELD) {
                    /* if 'T' aleady exist just combine its property by BIT OR operation */
                    if((pFieldT = _pmScanTfield(*(pSrc+1), pFieldS)) != NULL) {
                        *(pFieldT+2) |= *(pSrc+2);
                    }

                    /* Copy only new 'T' field */
                    if(pFieldT == NULL) {
                        COPY_BYTES(pM, pSrc, 4);
                    }
                }

                pSrc += 4; /* next 'T' */
            }
 		}else if(*pSrc == 'M' || *pSrc == 'R') {
			memcpy(pM, pSrc, 6);
			pM += 6;
			pSrc += 6;
			continue;
		}

        if(s_id != MERGED_FIELD) {
            COPY_BYTES(pM, pSrc, 1);
        }
        pSrc++;

        /* aleady merged field just go on next */
        if(s_id == MERGED_FIELD)  {
            continue;
        }

        /*----------------------------------------------------*/
        /* Find dupulicated 'S' being followed and merge them */

        pScan = pSrc; /* do not change pSrc in following loop */

        while(_pmFindSfield(s_id, pScan, &pS, &pE) > 0) {

            /* Change source's 'S' id to MERGED_FIELD */
            *(pS+1) = MERGED_FIELD;
            pS     += 2;

            /* merge dupulicated 'T' */
            while(*pS == 'T') {

                /* Append NEW 'T' field to the current 'S' field */
                /* aleady same 'T' exist only its mode property will be combined */
                /* after called function */
                if(pFieldS) {
                    if((bytes = _pmAppendTfield(pS, pFieldS)) > 0) {

                    /* update merged_buf's the last pos that pM point it */
                    pM += bytes; /* MUST 4 BYTES(size of 'T' field) ! */
                    }
                }

                pS += 4; /* next 'T' */
            }

            /* find next 'S' */
            pScan = (pE+1);

            VERBOSE_DUMP_PMREPLY((pFieldS, DUMP_S_TAG_ONLY, "< STEP 3 : MERGE PROCESSING ... >"));
        }
    }

    /* 0x0D & 0x0A */
    COPY_BYTES(pM, pSrc, 2);

    /*----------------------------------*/
    /* Copy the merged PM REPLY to pDes */

    pM = &merged_buf[0];

    while (*pM != 0xD || *(pM+1) != 0xA) {
        *pDes++ = *pM++;
    }

    *pDes++ = *pM++; /* 0xD */
    *pDes   = *pM;   /* 0xA */

    EPS_RETURN_VOID;
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   _pmAdjustQuality()                                                  */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* pData        EPS_UINT8*          I/O: Pointer to pm string                           */
/*                                                                                      */
/* Return value:                                                                        */
/*      None                                                                            */
/*                                                                                      */
/* Description:                                                                         */
/*      Adjust quality properties to the formal.                                        */
/*      example : quality has only draft mode -> turn on normal mode.                   */
/*                                                                                      */
/*******************************************|********************************************/
static void     _pmAdjustQuality (

        EPS_UINT8*  pData

){
    EPS_UINT8* p = pData;

	EPS_LOG_FUNCIN;

	/* skip pm heder */
    p += EPS_PM_HEADER_LEN;

    verbose_dbprint(("< STEP 4 :  Adjust quality >\r\n"));

    /* adjuct each quality properties */
    while(!(*p == 0x0D && *(p+1) == 0x0A)) {

        while(*p == 'S') {

            verbose_dbprint(("%c %02d\r\n", *p, *(p+1)));

            p += 2; /* move to the first T field */

            while(*p == 'T') {

                verbose_dbprint(("\t%c %02d 0x%02x %c -> ", *p, *(p+1), *(p+2), *(p+3)));

                p += 2; /* move to quality pos */

                /* Quality property */
                switch(*p & 0x07) {
                    /* Should be handled following case 1 bit of Draft turned on only */
                    case 0x01: /* 0 0 1 -> 0 1 1 */
                        *p |= (1<<1); /* turn normal on */
                        break;
                    default:
                        break;
                }

                verbose_dbprint(("%c %02d 0x%02x %c\r\n", *(p-2), *(p-1), *(p), *(p+1)));

                p += 2; /* move to the next T field */
            }

            p += 1; /* move to the next S field */
        }

		if(*p == 'M' || *p == 'R') {
			p += 6;
		}
    }

    EPS_RETURN_VOID;
}


/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   obsSetPrinter()                                                     */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* printer      EPS_PRINTER_INN8*   I: Pointer to printer info                          */
/*                                                                                      */
/* Return value:                                                                        */
/*      None                                                                            */
/*                                                                                      */
/* Description:                                                                         */
/*      set the observation target printer.                                             */
/*                                                                                      */
/*******************************************|********************************************/
void     obsSetPrinter (

        const EPS_PRINTER_INN* printer

){
	g_observer.printer = printer;
}


/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   obsSetColorPlane()                                                  */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* colorPlane   EPS_UINT8           I: color plane                                      */
/*                                                                                      */
/* Return value:                                                                        */
/*      None                                                                            */
/*                                                                                      */
/* Description:                                                                         */
/*      set color plane of current job.                                                 */
/*                                                                                      */
/*******************************************|********************************************/
void     obsSetColorPlane (

        EPS_UINT8 colorPlane

){
	g_observer.colorPlane = colorPlane;
}


/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   obsClear()                                                          */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* N/A                                                                                  */
/*                                                                                      */
/* Return value:                                                                        */
/*      None                                                                            */
/*                                                                                      */
/* Description:                                                                         */
/*      clear all property.                                                             */
/*                                                                                      */
/*******************************************|********************************************/
void     obsClear (

        void

){
	g_observer.printer = NULL;
	g_observer.colorPlane = EPS_CP_FULLCOLOR;
}


/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   obsGetPageMode()                                                    */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* N/A                                                                                  */
/*                                                                                      */
/* Return value:                                                                        */
/*      Means to page process.                                                          */
/*                                                                                      */
/* Description:                                                                         */
/*      The means to page process is decided.                                           */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32     obsGetPageMode (

        void

){
	const EPS_INT8 *modelName = NULL;

	if( NULL == g_observer.printer ){
		return EPS_PM_PAGE;
	}

	if(EPS_CP_JPEG == g_observer.colorPlane )
	{
		modelName = g_observer.printer->modelName;
		if( (strcmp(modelName, "EP-801A"             ) == 0) ||
		    (strcmp(modelName, "Artisan 700"         ) == 0) ||
		    (strcmp(modelName, "Stylus Photo TX700W" ) == 0) ||
		    (strcmp(modelName, "Stylus Photo PX700W" ) == 0) ||
		    (strcmp(modelName, "EP-901F"             ) == 0) ||
		    (strcmp(modelName, "EP-901A"             ) == 0) ||
		    (strcmp(modelName, "Artisan 800"         ) == 0) ||
		    (strcmp(modelName, "Stylus Photo PX800FW") == 0) ||
		    (strcmp(modelName, "Stylus Photo TX800FW") == 0) )
		{
			return EPS_PM_JOB;
		}
	}

	return EPS_PM_PAGE;
}


EPS_BOOL    obsIsA3Model (
							  
		EPS_INT32 ch
							  
){
	const EPS_INT8 *modelName = NULL;
	
	modelName = g_observer.printer->modelName;
	if(EPS_MDC_STATUS == ch){			/* status code */
		if( (strcmp(modelName, "PX-5V"						) == 0) ||
			   (strcmp(modelName, "Epson Stylus Photo R3000") == 0) ||
			   (strcmp(modelName, "PX-7V"					) == 0) ||
			   (strcmp(modelName, "Epson Stylus Photo R2000") == 0) ||
			   (strcmp(modelName, "EP-4004"					) == 0) ||
			   (strcmp(modelName, "Artisan 1430"            ) == 0) ||
			   (strcmp(modelName, "Epson Stylus Photo 1430") == 0)  ||
			   (strcmp(modelName, "Epson Stylus Photo 1500") == 0) )
		{
			return TRUE;
		}
	} else if(EPS_MDC_NOZZLE == ch){	/* nozzle patern */
		if( (strcmp(modelName, "PX-5V"						) == 0) ||
			   (strcmp(modelName, "Epson Stylus Photo R3000") == 0) ||
			   (strcmp(modelName, "PX-7V"					) == 0) ||
			   (strcmp(modelName, "Epson Stylus Photo R2000") == 0) )
		{
			return TRUE;
		}
	}

	return FALSE;
}

static EPS_INT8 modelFY11Bussiness[][16] = {
					"PX-1600F", "WF-7510 Series", "WF-7511 Series", "WF-7515 Series",
					"PX-1700F", "WF-7520 Series", "WF-7521 Series", "WF-7525 Series",
					"PX-1200", "WF-7010 Series", "WF-7011 Series", "WF-7012 Series", "WF-7015 Series",
					"PX-B750F", "WP-4511 Series", "WP-4515 Series", "WP-4521 Series", "WP-4525 Series", 
					"WP-4530 Series", "WP-4531 Series", "WP-4535 Series", 
					"WP-4540 Series", "WP-4545 Series", 
					"PX-B700", "WP-4015 Series", "WP-4011 Series", "WP-4020 Series", "WP-4025 Series"
};

EPS_BOOL    obsEnableDuplex (
							  
		EPS_INT32	sizeID
							  
){
	const EPS_INT8 *modelName = NULL;
	EPS_INT32 i = 0;
	
	if( !(sizeID == EPS_MSID_A4		|| 
		sizeID == EPS_MSID_LETTER	|| 
		sizeID == EPS_MSID_B5		))
	{
		modelName = g_observer.printer->modelName;
		for(i = 0; i < 28; i++){
			EPS_DBGPRINT(("%s\n", modelFY11Bussiness[i]));
			if( strcmp(modelName, modelFY11Bussiness[i]) == 0){
				return FALSE;
			}
		}
	}

	return TRUE;
}

EPS_BOOL    obsEnableAutoFeed (
							  
		void
							  
){
	const EPS_INT8 *modelName = NULL;
	EPS_INT32 i = 0;
	
	modelName = g_observer.printer->modelName;
	for(i = 0; i < 28; i++){
		/*EPS_DBGPRINT(("%s\n", modelFY11Bussiness[i]));*/
		if( strcmp(modelName, modelFY11Bussiness[i]) == 0){
			return TRUE;
		}
	}

	return FALSE;
}
/*____________________________   epson-escpr-services.c   ______________________________*/

/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*       1         2         3         4         5         6         7         8        */
/*******************************************|********************************************/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/***** End of File *** End of File *** End of File *** End of File *** End of File ******/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
