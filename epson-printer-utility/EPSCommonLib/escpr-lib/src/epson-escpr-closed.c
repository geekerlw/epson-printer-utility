/*_____________________________   epson-escpr-closed.c   _______________________________*/

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
/*                             Epson ESC/P-R Library APIs                               */
/*                                                                                      */
/*                                Core Function Calls                                   */
/*                            --------------------------                                */
/*		API-0001   EPS_ERR_CODE epsSetPlatform       (platform                );        */
/*                                                                                      */
/*******************************************|********************************************/

/*------------------------------------  Includes   -------------------------------------*/
/*******************************************|********************************************/
#include "../inc/epson-escpr-err.h"
#include "../inc/epson-protocol.h"
#include "../inc/epson-escpr-mem.h"

#ifdef GCOMSW_PRT_USE_USB
#include "epson-usb.h"
#endif
#ifdef GCOMSW_PRT_USE_NETWORK
#include "../inc/epson-net-snmp.h"
#endif
#include "../inc/epson-escpr-closed.h"

/*--------------------------------  Local Definition   ---------------------------------*/
/*******************************************|********************************************/
#ifdef EPS_LOG_MODULE_API
#define EPS_LOG_MODULE EPS_LOG_MODULE_API
#else
#define EPS_LOG_MODULE 0
#endif

/*----------------------------  ESC/P-R Lib Global Variables  --------------------------*/
/*******************************************|********************************************/
extern EPS_CMN_FUNC    epsCmnFnc;
extern EPS_PRINT_JOB   printJob;

static EPS_BOOL	g_blnNonCheckPrintMode = FALSE;				/* use [u:chku]          */

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%                                             %%%%%%%%%%%%%%%%%%%%%*/
/*--------------------             ESC/P-R Library API             ---------------------*/
/*%%%%%%%%%%%%%%%%%%%%                                             %%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:     epsSetPlatform()                                                  */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* platform     EPS_INT8            I: Platform code                                    */
/*                                                                                      */
/*******************************************|********************************************/
EPS_ERR_CODE    epsSetPlatform (

        EPS_UINT8               platform

){
/*** Declare Variable Local to Routine                                                  */
EPS_ERR_CODE    retStatus = EPS_ERR_NONE;       /* Return status of internal calls      */

	printJob.platform = platform;

	return retStatus;
}


/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   InfoCommand()                                                       */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* command      EPS_INT32           I: Command Code                                     */
/* buffer       EPS_UINT8*          O: Result of Info command                           */
/* bufSize      EPS_INT32*          I/O: size of buffer                                 */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPS_ERR_NONE                    - Info command executed successfully            */
/*      EPS_ERR_COMM_ERROR              - Info command execution error                  */
/*                                                                                      */
/* Description:                                                                         */
/*      Sends Information commands to the printer.                                      */
/*                                                                                      */
/*******************************************|********************************************/
EPS_ERR_CODE   epsInfoCommand (

        EPS_INT32   cmd,
        EPS_UINT8*  buffer,
        EPS_INT32*  bufSize
        
){
	EPS_ERR_CODE    ret = EPS_ERR_NONE;
#ifdef GCOMSW_PRT_USE_NETWORK
	EPS_SNMP_FUNCS	snmp;
#endif

	if( EPS_PROTOCOL_USB == EPS_PRT_PROTOCOL(printJob.printer->protocol) ){
		/* USB */
#ifdef GCOMSW_PRT_USE_USB
		ret = usbInfoCommand(printJob.printer, cmd, buffer, bufSize);
#else
		ret = EPS_ERR_OPR_FAIL;
#endif
	} else{
		/* Net */
#ifdef GCOMSW_PRT_USE_NETWORK
		snmpSetupSTFunctions(&snmp, printJob.printer);
		ret = snmp.InfoCommand(printJob.printer, cmd, buffer, bufSize);
#else
		ret = EPS_ERR_OPR_FAIL;
#endif
	}

	return ret;
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:     epsSetNonCheckPrintMode()                                         */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* val          EPS_BOOL            I: flag                                             */
/*                                                                                      */
/*******************************************|********************************************/
void    epsSetNonCheckPrintMode (

        EPS_BOOL	val

){
	g_blnNonCheckPrintMode = val;
}


/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   clsAddPrvCommands()                                                 */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* pBuff        EPS_UINT8**         IO: pointer to command buffer                       */
/* pPos         EPS_UINT8**         IO: pointer to append position                      */
/* bufSize      EPS_UINT32*         IO: pointer to size of command buffer(pBuff)        */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPS_ERR_NONE                    - Info command executed successfully            */
/*      EPS_ERR_MEMORY_ALLOCATION       - Failed to allocate memory                     */
/*                                                                                      */
/* Description:                                                                         */
/*      Add hidden command.                                                             */
/*                                                                                      */
/*******************************************|********************************************/
EPS_ERR_CODE   clsAddPrvCommands (

  		EPS_UINT8 **pBuff, 
		EPS_UINT8 **pPos, 
		EPS_UINT32 *bufSize
        
){
	static const EPS_UINT8  cmd[]   = {
	                        0x1B, 'u', 0x02, 0x00, 0x00, 0x00,
                            'c', 'h', 'k', 'u',
                            0x01, 0x01};

	EPS_UINT32	cmdPosDist = (EPS_UINT32)(*pPos - *pBuff);	/* command offset distance  */
	EPS_UINT32	cmdSize = sizeof(cmd);

	EPS_LOG_FUNCIN;

	EPS_DBGPRINT(("Use OffsetPritingMode : %d\n", g_blnNonCheckPrintMode))
	if( FALSE == g_blnNonCheckPrintMode){
		EPS_RETURN( EPS_ERR_NONE )
	}
	g_blnNonCheckPrintMode = FALSE;	/* reset */

	EPS_MEM_GROW(EPS_UINT8*, *pBuff, bufSize, cmdPosDist + cmdSize )

	if(*pBuff != NULL){
		*pPos = *pBuff + cmdPosDist;
		memcpy(*pPos, cmd, cmdSize);
		*pPos += cmdSize;
		EPS_RETURN( EPS_ERR_NONE )
	} else{
		EPS_RETURN( EPS_ERR_MEMORY_ALLOCATION )
	}
}
/*_____________________________   epson-escpr-closed.c   _______________________________*/

/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*       1         2         3         4         5         6         7         8        */
/*******************************************|********************************************/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/***** End of File *** End of File *** End of File *** End of File *** End of File ******/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
