/*__________________________________   epson-wifi.c   __________________________________*/

/*       1         2         3         4         5         6         7         8        */
/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*******************************************|********************************************/
/*
 *   Copyright (c) 2013  Seiko Epson Corporation                 All rights reserved.
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
/*             Epson Wi-Fi Direct Setup Application Programming Interfaces              */
/*                                                                                      */
/*******************************************|********************************************/
#ifndef __EPSON_ESCPR_WIFI_H__
#define __EPSON_ESCPR_WIFI_H__
#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------  Includes   -------------------------------------*/
/*******************************************|********************************************/
#include "epson-net-snmp.h"

/*----------------------------------  Generic Macros   ---------------------------------*/
/*******************************************|********************************************/
/* #define EPS_USE_CLOSEDFUNC */ 

/*-----------------------------------  Definitions  ------------------------------------*/
/*******************************************|********************************************/


/*---------------------------  Data Structure Declarations   ---------------------------*/
/*******************************************|********************************************/
	
/*--------------------------------  Global Declarations   ------------------------------*/
/*******************************************|********************************************/
typedef EPS_INT32 (*EPS_GetMD5 )(const EPS_UINT8* data, EPS_UINT32 dataLen, EPS_UINT8 hash[16]);


/*----------------------------  API Function Declarations   ----------------------------*/
/*******************************************|********************************************/

extern EPS_ERR_CODE epwGetSetupConnectStrings(const EPS_INT8**, const EPS_INT8**        );
extern EPS_ERR_CODE epwGetConnectStrings    (EPS_INT8* address, const EPS_INT8* engineID, 
                                             EPS_INT8* ssid, EPS_INT32 ssidLen, 
											 EPS_INT8* password, EPS_INT32 passwordLen, 
											 EPS_PRINTER* printer,
											 EPS_GetMD5 md5Callback );
extern EPS_ERR_CODE epwStartWifiDirect      (const EPS_INT8* address                    );

extern EPS_ERR_CODE epwGetAutoGoTimeOutValues(const EPS_INT8* address, EPS_INT32* curVal, 
                                             EPS_INT32* minVal, EPS_INT32* maxVal       );
extern EPS_ERR_CODE epwSetAutoGoTimeOut     (const EPS_INT8* address, EPS_INT32 val     );


#ifdef __cplusplus
}
#endif

#endif  /* def __EPSON_ESCPR_WIFI_H__ */

/*__________________________________   epson-wifi.c   __________________________________*/
  
/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*       1         2         3         4         5         6         7         8        */
/*******************************************|********************************************/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/***** End of File *** End of File *** End of File *** End of File *** End of File ******/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
