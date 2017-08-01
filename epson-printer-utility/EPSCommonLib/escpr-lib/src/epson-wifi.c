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
/*                                Core Function Calls                                   */
/*                            --------------------------                                */
/*		API-0001   EPS_ERR_CODE epsSetPlatform       (platform                );        */
/*                                                                                      */
/*******************************************|********************************************/

/*------------------------------------  Includes   -------------------------------------*/
/*******************************************|********************************************/
#include "epson-escpr-def.h"
#include "epson-escpr-err.h"
#include "epson-protocol.h"
#include "epson-escpr-mem.h"
#include "epson-wifi.h"

/*-----------------------------------  Definitions  ------------------------------------*/
/*******************************************|********************************************/
#define SEC_CRERT_MD5			"\x00"				/* 0                         */
#define SEC_ENCRYPT_NONE		"\x00"				/* 0                         */
#define SEC_ENCRYPT_MODE0		"\x01"				/* 1                         */

#define EPSNET_STAT_RECV_TIMEOUT_SEC	(5000)		/* status receive timeout for  SecLogin,SecGetPassword,SecLogout */

extern EPS_NET_FUNC    epsNetFnc;
extern EPS_CMN_FUNC    epsCmnFnc;

/*--------------------------------  Local Functions   ----------------------------------*/
/*******************************************|********************************************/
static EPS_ERR_CODE FindPrinter     (EPS_PRINTER* printer                               );
static EPS_ERR_CODE SecLogin		(EPS_SOCKET sock, const EPS_INT8* address, 
                                     const EPS_INT8* engineID, 
                                     const EPS_UINT8 midKey[16], const EPS_UINT8 midKeyIndex[2],
                                     EPS_GetMD5 md5Callback                             );
static EPS_ERR_CODE SecLogout		(EPS_SOCKET sock, const EPS_INT8* address, 
                                     const EPS_INT8* engineID, 
                                     const EPS_UINT8 midKey[16], const EPS_UINT8 midKeyIndex[2],
                                     EPS_GetMD5 md5Callback                             );
static EPS_ERR_CODE SecGetPassword (EPS_SOCKET sock, const EPS_INT8* address, 
                                    const EPS_INT8* engineID, 
                                    const EPS_UINT8 midKey[16], const EPS_UINT8 midKeyIndex[2],
                                    EPS_GetMD5 md5Callback,
                                    EPS_INT8* password, EPS_INT32 passwordLen           );

static EPS_ERR_CODE SecCreateCommand(const EPS_INT8	engineID[10], const EPS_INT8 securityFlags[2], 
                                     const EPS_SNMP_VARBIND* varBind, EPS_INT32 varBindNum,
                                     const EPS_UINT8 midKey[16], EPS_GetMD5 md5Callback,
                                     EPS_UINT8 requestCmd, EPS_BOOL encryptPDU,
                                     EPS_INT8* buf, EPS_UINT32* bufSize                 );
static EPS_ERR_CODE SecParseResponse(EPS_INT8* buf, EPS_INT32 nBufLen,
                                     const EPS_UINT8 midKey[16], EPS_GetMD5 md5Callback,
                                     EPS_SNMP_PDU* pdu                                  );
static EPS_ERR_CODE	SecSetAuthParameter(EPS_INT8* buf, EPS_INT32 nBufLen, 
                                     const EPS_UINT8 midKey[16], EPS_INT8* pReplace, 
									 EPS_GetMD5	md5Callback                             );
static EPS_ERR_CODE	encodeMode0     (EPS_UINT8*	buf, EPS_INT32*	extPduSize, 
	                                 const EPS_UINT8 midKey[16]                         );
static EPS_ERR_CODE	decodeMode0     (EPS_UINT8*	buf, EPS_INT32	extPduSize, 
	                                 const EPS_UINT8 midKey[16]                         );
static void GetMidKey               (EPS_UINT8 key[16], EPS_UINT8 bits[2]               );
static void GetEncryptKey		    (const EPS_UINT8 midKey[2], EPS_UINT8 encKey[2]     );

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%                                             %%%%%%%%%%%%%%%%%%%%%*/
/*--------------------            Wi-Fi Direct setup API           ---------------------*/
/*%%%%%%%%%%%%%%%%%%%%                                             %%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epwGetSetupConnectStrings()                                         */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* ssid         EPS_UINT8*          O: Command Code                                     */
/* password     EPS_UINT8*          O: Result of Info command                           */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPS_ERR_NONE                    - Info command executed successfully            */
/*      EPS_ERR_COMM_ERROR              - Info command execution error                  */
/*                                                                                      */
/* Description:                                                                         */
/*      Sends Information commands to the printer.                                      */
/*                                                                                      */
/*******************************************|********************************************/
EPS_ERR_CODE    epwGetSetupConnectStrings (

        const EPS_INT8**     ssid,
        const EPS_INT8**     password

){
	EPS_ERR_CODE ret = EPS_ERR_NONE;
	static const EPS_INT8 c_ssid[] = "DIRECT-epson-setup-iPrint";
	static const EPS_INT8 c_pwd[] = "4eUP2CeD5nzQB84L6KBir3";

	*ssid = c_ssid;
	*password = c_pwd;

	return ret;
}


/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epwGetConnectStrings()                                              */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* address      EPS_INT8*           I: target printer IP Address                        */
/* macAddress   EPS_INT8*           I: host mac adddress                                */
/* ssid         EPS_UINT8*          O: Command Code                                     */
/* password     EPS_UINT8*          O: Result of Info command                           */
/* md5Callback  EPS_GetMD5          I: pointer that get MD5 hash callback function      */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPS_ERR_NONE                    - Info command executed successfully            */
/*      EPS_ERR_COMM_ERROR              - Info command execution error                  */
/*                                                                                      */
/* Description:                                                                         */
/*      Sends Information commands to the printer.                                      */
/*                                                                                      */
/*******************************************|********************************************/
EPS_ERR_CODE    epwGetConnectStrings (

        EPS_INT8*		address, 
        const EPS_INT8* engineID, 
        EPS_INT8*		ssid,
        EPS_INT32		ssidLen,
        EPS_INT8*		password,
        EPS_INT32		passwordLen,
		EPS_PRINTER*	printer,
		EPS_GetMD5		md5Callback

){
	EPS_ERR_CODE ret = EPS_ERR_NONE;
	EPS_SOCKET  sock = EPS_INVALID_SOCKET;
	ASN_VARIANT responseVal;
	EPS_UINT8	midKey[16];
	EPS_UINT8	midKeyIndex[2];

	memset(ssid, 0, ssidLen);
	memset(password, 0, passwordLen);

	/* ----------------------------------------------------------------------- */
	/* Find Printer */
	if(NULL == address){
		ret = FindPrinter(printer);
		if(EPS_ERR_NONE != ret){
			return ret;
		}

		address = printer->location;
		EPS_DBGPRINT(("printer : %s\n", printer->modelName));
	}

	ret = snmpOpenSocket(&sock);
	if(EPS_ERR_NONE != ret){
		return ret;
	}

	/* ----------------------------------------------------------------------- */
	/* Get SSID */
	ret = snmpTransactS(sock, address, EPSNET_STAT_RECV_TIMEOUT, 
						"1.3.6.1.4.1.1248.1.1.3.1.29.3.1.35.0", ASN_PDU_GET,
						"public", NULL, &responseVal);
	if(EPS_ERR_NONE != ret){
		snmpCloseSocket(&sock);
		return ret;
	}
	memcpy(ssid, responseVal.val.v_str, Min((EPS_UINT32)ssidLen-1, responseVal.length));
	EPS_DBGPRINT(("SSID : %s\n", ssid));

	/* ----------------------------------------------------------------------- */
	/* Get Password */
	GetMidKey(midKey, midKeyIndex);				/* 中間キー */

	/* Login */
	ret = SecLogin(sock, address, engineID, midKey, midKeyIndex, md5Callback);
	if(EPS_ERR_NONE != ret){
		snmpCloseSocket(&sock);
		return ret;
	}

	/* Get Password */
	ret = SecGetPassword(sock, address, engineID, midKey, midKeyIndex, md5Callback, password, passwordLen);
	EPS_DBGPRINT(("password : %s\n", password));

	/* Logout */
	/*ret = ignore */SecLogout(sock, address, engineID, midKey, midKeyIndex, md5Callback);
	
	snmpCloseSocket(&sock);

	return ret;
}


/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epwStartWifiDirect()                                                */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* address      EPS_INT8*           I: target printer IP Address                        */
/* val          EPS_INT32           I: time out value (second)                          */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPS_ERR_NONE                    - Info command executed successfully            */
/*      EPS_ERR_COMM_ERROR              - Info command execution error                  */
/*                                                                                      */
/* Description:                                                                         */
/*      set Wi-Fi Direct timeout to the printer.                                        */
/*                                                                                      */
/*******************************************|********************************************/
EPS_ERR_CODE    epwStartWifiDirect (

        const EPS_INT8* address

){
	EPS_ERR_CODE    ret = EPS_ERR_NONE;       /* Return status of internal calls      */
	EPS_SOCKET      sock = EPS_INVALID_SOCKET;
	ASN_VARIANT     request;

	memset(&request, 0, sizeof(request));
	request.type = ASN_VT_INTEGER;
	request.length = sizeof(EPS_INT32);
	request.val.v_long = 1;		/* start */

	ret = snmpOpenSocket(&sock);
	if(EPS_ERR_NONE != ret){
		return ret;
	}

	/* epsWlanConfWifiDirectVeryEasySetupStart */
	ret = snmpTransactS(sock, address, EPSNET_STAT_RECV_TIMEOUT, 
						"1.3.6.1.4.1.1248.1.1.3.1.29.3.1.37.0", ASN_PDU_SET, "epson",
						&request, NULL);
	
	snmpCloseSocket(&sock);

	return ret;
}


/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epwGetAutoGoTimeOutValues()                                         */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* address      EPS_INT8*           I: target printer IP Address                        */
/* curVal       EPS_INT32*          O: current Timeout value (second)                   */
/* minVal       EPS_INT32*          O: current Timeout value (second)                   */
/* maxVal       EPS_INT32*          O: current Timeout value (second)                   */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPS_ERR_NONE                    - Info command executed successfully            */
/*      EPS_ERR_COMM_ERROR              - Info command execution error                  */
/*                                                                                      */
/* Description:                                                                         */
/*      Sends Information commands to the printer.                                      */
/*                                                                                      */
/*******************************************|********************************************/
EPS_ERR_CODE epwGetAutoGoTimeOutValues (
	
        const EPS_INT8* address, 
		EPS_INT32* curVal,
		EPS_INT32* minVal, 
		EPS_INT32* maxVal 
		
){
	EPS_ERR_CODE    ret = EPS_ERR_NONE;       /* Return status of internal calls      */
	EPS_SOCKET      sock = EPS_INVALID_SOCKET;
	ASN_VARIANT     responseVal;

	ret = snmpOpenSocket(&sock);
	if(EPS_ERR_NONE != ret){
		return ret;
	}

	/* epsWlanConfWifiDirectAutoGoCloseTimeout */
	ret = snmpTransactS(sock, address, EPSNET_STAT_RECV_TIMEOUT, 
						"1.3.6.1.4.1.1248.1.1.3.1.29.3.1.31.0", ASN_PDU_GET, "public",
						NULL, &responseVal);
	if(EPS_ERR_NONE != ret){
		snmpCloseSocket(&sock);
		return ret;
	}
	*curVal = responseVal.val.v_long;

	/* epsWlanConfWifiDirectAutoGoCloseMinTimeout */
	ret = snmpTransactS(sock, address, EPSNET_STAT_RECV_TIMEOUT, 
						"1.3.6.1.4.1.1248.1.1.3.1.29.3.1.29.0", ASN_PDU_GET, "public", 
						NULL, &responseVal);
	if(EPS_ERR_NONE != ret){
		snmpCloseSocket(&sock);
		return ret;
	}
	*minVal = responseVal.val.v_long;

	/* epsWlanConfWifiDirectAutoGoCloseMaxTimeout */
	ret = snmpTransactS(sock, address, EPSNET_STAT_RECV_TIMEOUT, 
						"1.3.6.1.4.1.1248.1.1.3.1.29.3.1.30.0", ASN_PDU_GET, "public",
						NULL, &responseVal);
	if(EPS_ERR_NONE != ret){
		snmpCloseSocket(&sock);
		return ret;
	}
	*maxVal = responseVal.val.v_long;

	return ret;
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epwSetAutoGoTimeOut()                                               */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* address      EPS_INT8*           I: target printer IP Address                        */
/* val          EPS_INT32           I: time out value (second)                          */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPS_ERR_NONE                    - Info command executed successfully            */
/*      EPS_ERR_COMM_ERROR              - Info command execution error                  */
/*                                                                                      */
/* Description:                                                                         */
/*      set Wi-Fi Direct timeout to the printer.                                        */
/*                                                                                      */
/*******************************************|********************************************/
EPS_ERR_CODE    epwSetAutoGoTimeOut (

        const EPS_INT8* address, 
        EPS_INT32       value

){
	EPS_ERR_CODE    ret = EPS_ERR_NONE;       /* Return status of internal calls      */
	EPS_SOCKET      sock = EPS_INVALID_SOCKET;
	ASN_VARIANT     request;

	memset(&request, 0, sizeof(request));
	request.type = ASN_VT_INTEGER;
	request.length = sizeof(EPS_INT32);
	request.val.v_long = value;

	ret = snmpOpenSocket(&sock);
	if(EPS_ERR_NONE != ret){
		return ret;
	}

	/* epsWlanConfWifiDirectAutoGoCloseTimeout */
	ret = snmpTransactS(sock, address, EPSNET_STAT_RECV_TIMEOUT, 
						"1.3.6.1.4.1.1248.1.1.3.1.29.3.1.31.0", ASN_PDU_SET, "epson",
						&request, NULL);
	
	snmpCloseSocket(&sock);

	return ret;
}


static EPS_ERR_CODE    FindPrinter (

		EPS_PRINTER*	printer

){
	EPS_ERR_CODE ret = EPS_ERR_NONE;
	EPS_SOCKET  *socks = NULL;
	EPS_PRINTER_INN *prn = NULL;
	EPS_UINT32		tmStart, tmNow;

	EPS_INT32		n = 0;
	EPS_INT32		ifNum = 0;
	EPS_UINT8*		ifBuf = NULL;
#if LCOMSW_USE_MULTI_IF
	EPS_UINT32		ifBufSize = 0;
#endif

#if LCOMSW_USE_MULTI_IF
	ifNum = epsNetFnc.enumInterface(NULL, ifBufSize);
	if(ifNum > 0){
		ifBufSize = ifNum * EPS_IFNAME_LEN;
		ifBuf = (EPS_UINT8*)EPS_ALLOC(ifBufSize);
		if(NULL == ifBuf){
			return EPS_ERR_MEMORY_ALLOCATION;
		}
		memset(ifBuf, 0, ifBufSize);
		ifNum = epsNetFnc.enumInterface(ifBuf, ifBufSize);
	}
	if(ifNum <= 0){
		EPS_SAFE_RELEASE( ifBuf );
		return EPS_ERR_COMM_ERROR;
	}
#else
	ifNum = 1;
#endif

	socks = (EPS_SOCKET*)EPS_ALLOC(sizeof(EPS_SOCKET)*ifNum);

	for(n = 0; n < ifNum; n++){
		*(socks+n) = EPS_INVALID_SOCKET;
		ret = snmpFindStart(socks+n, EPSNET_UDP_BROADCAST_ADDR, TRUE, ifBuf+(n*EPS_IFNAME_LEN));
	}

	if(EPS_ERR_NONE == ret){
		tmStart = epsCmnFnc.getTime();
		ret = EPS_ERR_PRINTER_NOT_FOUND;
		while(EPS_ERR_PRINTER_NOT_FOUND == ret || EPS_ERR_PRINTER_NOT_USEFUL == ret){
			for(n = 0; n < ifNum; n++){
				ret = snmpFind(*(socks+n), 0, 0, &prn);	// 無効なprotocolじゃないとprtIsRegistered()に引っかかる

				//それぞれのNICについての戻り値をチェック
				if( EPS_ERR_NONE == ret){
					//正常終了
					break;
				}else if( EPS_COM_NOT_RECEIVE == ret ){
					//プリンタが見つかっていない
					ret = EPS_ERR_PRINTER_NOT_FOUND;
				}
			}

			if( EPS_ERR_NONE == ret){
				break;
			}

			tmNow = epsCmnFnc.getTime();
			if((EPS_UINT32)(tmNow - tmStart) > 15000){
				ret = EPS_ERR_PRINTER_NOT_FOUND;
				break;
			}
		}
	}

	if(EPS_ERR_NONE == ret){
		printer->protocol					= EPS_PRT_PROTOCOL(prn->protocol);
		printer->supportFunc				= prn->supportFunc;
		printer->language					= prn->language;
		strcpy(printer->location,			prn->location);
		strcpy(printer->macAddress,			prn->macAddress);
		strcpy(printer->manufacturerName,	prn->manufacturerName);
		strcpy(printer->modelName,			prn->modelName);
		strcpy(printer->friendlyName,		prn->friendlyName);
		strcpy(printer->serialNo,			prn->serialNo);
		strcpy(printer->printerID,			prn->printerID);
	}
	EPS_SAFE_RELEASE(prn);

	for(n = 0; n < ifNum; n++){
		snmpFindEnd(*(socks+n));
	}
	EPS_FREE(socks);

	return ret;
}


static EPS_ERR_CODE    SecLogin (

		EPS_SOCKET      sock,
        const EPS_INT8* address, 
        const EPS_INT8* engineID, 
		const EPS_UINT8 midKey[16],
		const EPS_UINT8 midKeyIndex[2],
		EPS_GetMD5		md5Callback

){
	EPS_ERR_CODE ret = EPS_ERR_NONE;
	EPS_SNMP_VARBIND	request[3];
	ASN_VARIANT responseVal;
	EPS_INT8	buf[512];
	EPS_UINT32	nBufLen = sizeof(buf);

	memset(buf, 0, sizeof(buf));

	/* ----------------------------------------------------------------------- */
	/* epsAdminMibAccessLoginOperation */ 
	request[0].identifire = "1.3.6.1.4.1.1248.1.1.3.1.30.2.2.0";
	request[0].value.type = ASN_VT_INTEGER;
	request[0].value.val.v_long = 1;				/* login */
	/* epsAdminMibAccessLoginTimeout */ 
	request[1].identifire = "1.3.6.1.4.1.1248.1.1.3.1.30.2.3.0";
	request[1].value.type = ASN_VT_INTEGER;
	request[1].value.val.v_long = 30;
	/* epsAdminMibAccessLoginAuthPrivKey */ 
	request[2].identifire = "1.3.6.1.4.1.1248.1.1.3.1.30.2.4.0";
	request[2].value.type = ASN_VT_OCTET_STRING;
	request[2].value.val.v_str = (EPS_INT8*)midKeyIndex;
	request[2].value.length = 2;

	/* Create extend PDU */
	ret = SecCreateCommand(engineID, SEC_CRERT_MD5 SEC_ENCRYPT_NONE,
							request, 3, midKey, md5Callback,
							ASN_PDU_SET, FALSE, buf, &nBufLen);

	/* ----------------------------------------------------------------------- */
	/* request & response */
	memset(&request[0], 0, sizeof(request));	// 流用
	request[0].value.type = ASN_VT_OCTET_STRING;
	request[0].value.length = nBufLen;
	request[0].value.val.v_str = buf;
	// epsAdminMibAccessID
	ret = snmpTransactS(sock, address, EPSNET_STAT_RECV_TIMEOUT_SEC, 
						"1.3.6.1.4.1.1248.1.1.3.1.30.2.1.0", ASN_PDU_GET, 
						"public", &(request[0].value), &responseVal);
	if(EPS_ERR_NONE != ret){
		return ret;
	}

	/* ----------------------------------------------------------------------- */
	/* parse extend response */
	ret = SecParseResponse(responseVal.val.v_str, responseVal.length, midKey, md5Callback, NULL);

	return ret;
}


static EPS_ERR_CODE    SecLogout (

		EPS_SOCKET      sock,
        const EPS_INT8* address, 
        const EPS_INT8* engineID, 
		const EPS_UINT8 midKey[16], 
		const EPS_UINT8 midKeyIndex[2],
		EPS_GetMD5		md5Callback

){
	EPS_ERR_CODE ret = EPS_ERR_NONE;
	EPS_SNMP_VARBIND	request;
	ASN_VARIANT responseVal;
	EPS_INT8	buf[1024];
	EPS_UINT32	nBufLen = 0;

	memset(buf, 0, sizeof(buf));

	/* ----------------------------------------------------------------------- */
	/* Encrypt PDU */
	/* epsAdminMibAccessLoginOperation */ 
	request.identifire = "1.3.6.1.4.1.1248.1.1.3.1.30.2.2.0";
	request.value.type = ASN_VT_INTEGER;
	request.value.val.v_long = 2;					/* logout */
	ret = SecCreateCommand(engineID, SEC_CRERT_MD5 SEC_ENCRYPT_MODE0,
						&request, 1, midKey, md5Callback,
						ASN_PDU_SET, TRUE, buf, &nBufLen);

	/* ----------------------------------------------------------------------- */
	/* request & response */
	memset(&request, 0, sizeof(request));	// 流用
	request.value.type = ASN_VT_OCTET_STRING;
	request.value.length = nBufLen;
	request.value.val.v_str = buf;
	// epsAdminMibAccessID
	ret = snmpTransactS(sock, address, EPSNET_STAT_RECV_TIMEOUT_SEC, 
						"1.3.6.1.4.1.1248.1.1.3.1.30.2.1.0", ASN_PDU_GET,
						"public", &(request.value), &responseVal);
	if(EPS_ERR_NONE != ret){
		return ret;
	}

	/* ----------------------------------------------------------------------- */
	/* parse extend response */
	ret = SecParseResponse (responseVal.val.v_str, responseVal.length, midKey, md5Callback, NULL);

	return ret;
}


static EPS_ERR_CODE    SecGetPassword (

		EPS_SOCKET      sock,
        const EPS_INT8* address, 
        const EPS_INT8* engineID, 
		const EPS_UINT8 midKey[16], 
		const EPS_UINT8	midKeyIndex[2],
		EPS_GetMD5		md5Callback,
        EPS_INT8*		password,
        EPS_INT32		passwordLen

){
	EPS_ERR_CODE ret = EPS_ERR_NONE;
	EPS_SNMP_VARBIND	request;
	ASN_VARIANT responseVal;
	EPS_INT8	buf[1024];
	EPS_UINT32	nBufLen = 0;
	EPS_SNMP_PDU responsePDU;

	memset(buf, 0, sizeof(buf));

	/* ----------------------------------------------------------------------- */
	/* Encrypt PDU */
	/* epsWlanConfWifiDirectVeryEasySetupPassword */ 
	request.identifire = "1.3.6.1.4.1.1248.1.1.3.1.29.3.1.36.0";
	request.value.type = ASN_VT_NULL;
	ret = SecCreateCommand(engineID, SEC_CRERT_MD5 SEC_ENCRYPT_MODE0,
						&request, 1, midKey, md5Callback,
						ASN_PDU_GET, TRUE, buf, &nBufLen);

	/* ----------------------------------------------------------------------- */
	/* request & response */
	memset(&request, 0, sizeof(request));	// 流用
	request.value.type = ASN_VT_OCTET_STRING;
	request.value.length = nBufLen;
	request.value.val.v_str = buf;
	// epsAdminMibAccessID
	ret = snmpTransactS(sock, address, EPSNET_STAT_RECV_TIMEOUT_SEC, 
						"1.3.6.1.4.1.1248.1.1.3.1.30.2.1.0", ASN_PDU_GET, 
						"public", &(request.value), &responseVal);
	if(EPS_ERR_NONE != ret){
		return ret;
	}

	/* ----------------------------------------------------------------------- */
	/* parse extend response */
	ret = SecParseResponse (responseVal.val.v_str, responseVal.length, midKey, md5Callback, &responsePDU);
	if(EPS_ERR_NONE == ret){
		memcpy(password, responsePDU.var[0].value.val.v_str,
				Min((EPS_UINT32)passwordLen-1, responsePDU.var[0].value.length));
	}
	EPS_SNMP_REREASE_PDU( responsePDU )

	return ret;
}


static EPS_ERR_CODE    SecCreateCommand (

		const EPS_INT8	engineID[10], 
		const EPS_INT8	securityFlags[2], 
		const EPS_SNMP_VARBIND*	varBind,
		EPS_INT32		varBindNum,
		const EPS_UINT8 midKey[16],
		EPS_GetMD5		md5Callback,
		EPS_UINT8		requestCmd,
		EPS_BOOL		encryptPDU,
		EPS_INT8*		buf,
		EPS_UINT32*		bufSize

){
	EPS_ERR_CODE ret = EPS_ERR_NONE;
	EPS_INT8*	p = buf;
	EPS_INT8*	pEpAuthParameter = NULL;
	EPS_UINT8	extPduBuf[1024];
	EPS_INT32	extPduSize = 0;
	EPS_INT32   nReqID = 0;

	memset(buf, 0, *bufSize);

	/* epEngineID */
	p = snmpMakeStrField(engineID, 10,  p);
	/* epSecurityFlags */
	p = snmpMakeStrField(securityFlags, 2, p);			
	/* epAuthParameter */
	pEpAuthParameter = p;	// 後で使う
	p = snmpMakeStrField("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 12,  p);	/* temporary */
	/* epPrivParameter */
	p = snmpMakeStrField("", 0,  p);				/* unuse by Mode0 */
	/* epErrorStatus */
	p = snmpMakeIntField(0, p);						/* 0 = noError */
	/* epEncryptedPDU */
	nReqID = snmpGetRequestId();
//	nReqID = 1142589098; //TEST
//	nReqID = 1142589106;
	ret = snmpCreatePDU((EPS_INT8*)extPduBuf, requestCmd, nReqID, varBind, varBindNum, &extPduSize);
	if(EPS_ERR_NONE != ret){
		return ret;
	}
	if(encryptPDU){
		/* 暗号化 */
		ret = encodeMode0(extPduBuf, &extPduSize, midKey);
		if(EPS_ERR_NONE != ret){
			return ret;
		}
		/* OCTET-STRING型で格納 */
		p = snmpMakeStrField((EPS_INT8*)extPduBuf, extPduSize, p);
	} else{
		/* そのまま連結 */
		memcpy(p, extPduBuf, extPduSize);
		p += extPduSize;
	}
	*bufSize = (p-buf);

	/* Set AuthParameter */
	SecSetAuthParameter(buf, *bufSize, midKey, pEpAuthParameter, md5Callback);

	return ret;
}
		
static EPS_ERR_CODE    SecParseResponse (

		EPS_INT8*		buf,
		EPS_INT32		nBufLen,
		const EPS_UINT8 midKey[16],
		EPS_GetMD5		md5Callback,
		EPS_SNMP_PDU*	pdu

){
	EPS_ERR_CODE ret = EPS_ERR_NONE;
	EPS_INT8*	p = buf;
	ASN_VARIANT vField;
	EPS_BOOL	encryptPDU = FALSE;
	EPS_UINT8	extPduBuf[1024];
	EPS_INT32	extPduSize = 0;
	EPS_SNMP_PDU	responsePdu;

	/* epEngineID */
	ret = snmpParseField( &p, &nBufLen, &vField );
	EPS_SNMP_REREASE_VARIANT( vField )
	if(EPS_ERR_NONE != ret ){
		return ret;
	}
	/* epSecurityFlags */
	ret = snmpParseField( &p, &nBufLen, &vField );
	if(EPS_ERR_NONE != ret ){
		EPS_SNMP_REREASE_VARIANT( vField )
		return ret;
	}
	if( SEC_ENCRYPT_MODE0[0] == vField.val.v_str[1]  ){
		encryptPDU = TRUE;
	}

	/* epAuthParameter */
	ret = snmpParseField( &p, &nBufLen, &vField );
	if(EPS_ERR_NONE != ret ){
		return ret;
	}
	/* epPrivParameter */
	ret = snmpParseField( &p, &nBufLen, &vField );
	if(EPS_ERR_NONE != ret ){
		return ret;
	}
	/* epErrorStatus */
	ret = snmpParseField( &p, &nBufLen, &vField );
	if(EPS_ERR_NONE != ret ){
		return ret;
	}
	if(0 != vField.val.v_long){
		return EPS_ERR_COMM_ERROR;
	}

	/* epEncryptedPDU */
	if(TRUE == encryptPDU){
		// OCTET-STRINGになってる
		ret = snmpParseField( &p, &nBufLen, &vField );
		if(EPS_ERR_NONE != ret ){
			return ret;
		}
		if(sizeof(extPduBuf) < vField.length){
			return EPS_ERR_OPR_FAIL;
		}
		extPduSize = vField.length;
		memcpy(extPduBuf, vField.val.v_str, extPduSize);

		ret = decodeMode0(extPduBuf, extPduSize, midKey);
		if(EPS_ERR_NONE != ret ){
			return ret;
		}
		ret = snmpParsePDU((EPS_INT8*)extPduBuf, extPduSize, &responsePdu);
	} else{
		// PDUそのまま
		ret = snmpParsePDU(p, nBufLen, &responsePdu);
	}

	if(0 != responsePdu.errorStatus){
		return EPS_ERR_COMM_ERROR;
	}
	if(NULL != pdu){
		*pdu = responsePdu;
	} else{
		EPS_SNMP_REREASE_PDU( responsePdu );
	}

	return ret;
}


static void GetMidKey(EPS_UINT8 key[16], EPS_UINT8 bits[2])
{
	static const EPS_UINT8 keyTable[16][16] = {
		{ 0xe5, 0xac, 0x6c, 0x1b, 0xbd, 0x1b, 0x1c, 0x61, 0xab, 0xbf, 0xcc, 0x0c, 0xa1, 0x5f, 0x68, 0x70},
		{ 0xef, 0x53, 0xcb, 0x33, 0x20, 0xcf, 0xcc, 0x69, 0xea, 0x3e, 0xc5, 0xdb, 0x08, 0x07, 0x20, 0xc9},
		{ 0x40, 0x95, 0x9b, 0xfe, 0x4f, 0x32, 0x89, 0xe1, 0x50, 0x0c, 0xaf, 0x77, 0x69, 0xaf, 0x34, 0x9b},
		{ 0x1d, 0x17, 0x0e, 0xed, 0xfe, 0xdc, 0x8c, 0xab, 0x2a, 0xd8, 0x80, 0x72, 0x0c, 0x94, 0x09, 0x59},
		{ 0xbf, 0xe4, 0xf3, 0x28, 0x9e, 0x15, 0x00, 0x12, 0xc1, 0xf5, 0x3a, 0xe4, 0x52, 0x80, 0x1a, 0xc5},
		{ 0x43, 0x3e, 0xee, 0x76, 0x86, 0x5b, 0xfc, 0x6f, 0x4e, 0x76, 0x1e, 0x06, 0x49, 0xa4, 0x14, 0xbb},
		{ 0x4c, 0x7f, 0x37, 0xdb, 0xcb, 0xe8, 0x98, 0xb0, 0xee, 0xfc, 0x9a, 0x7f, 0x14, 0x49, 0x15, 0x5a},
		{ 0xb5, 0x72, 0x4d, 0x59, 0x15, 0xa9, 0x81, 0xb7, 0x9f, 0x9a, 0x9b, 0x07, 0x80, 0x97, 0xf6, 0x27},
		{ 0x71, 0xf4, 0xb1, 0x22, 0xa2, 0xfe, 0x7b, 0xc2, 0xba, 0x1e, 0x46, 0x83, 0xfc, 0x14, 0xb7, 0x9f},
		{ 0x9a, 0x9b, 0x07, 0x80, 0x97, 0xf6, 0x27, 0x71, 0xf4, 0xb1, 0x22, 0x1f, 0x4a, 0x08, 0xad, 0xe0},
		{ 0x8e, 0x7e, 0x02, 0x08, 0xe7, 0x5e, 0x61, 0xf5, 0xe4, 0x3f, 0x3c, 0x74, 0xdb, 0xb9, 0x04, 0xca},
		{ 0x1a, 0x25, 0x94, 0x8a, 0x67, 0x09, 0xb1, 0xc8, 0x35, 0x6a, 0xf5, 0x47, 0xfa, 0x4c, 0x40, 0xa1},
		{ 0xba, 0x65, 0xf8, 0x81, 0xc7, 0xed, 0x2f, 0x8a, 0xc1, 0x7d, 0xe8, 0xac, 0xfb, 0x46, 0x96, 0x05},
		{ 0x2c, 0xa3, 0x34, 0x06, 0xce, 0xb9, 0x4f, 0xcb, 0x96, 0x87, 0xc0, 0x69, 0xb3, 0x44, 0xe6, 0x59},
		{ 0xf3, 0x61, 0x88, 0xaa, 0xee, 0x6c, 0x77, 0x29, 0xab, 0x29, 0xbc, 0x01, 0x05, 0x58, 0x9e, 0x8d},
		{ 0x75, 0x95, 0x56, 0x01, 0xa6, 0xc5, 0x4a, 0x7c, 0x5a, 0x1f, 0x8a, 0x0a, 0x44, 0x3a, 0x9a, 0xa1}
	};
	EPS_UINT16	useBits = 0xFF;	// MSB
	EPS_INT32	n, m;

	useBits = (EPS_UINT16)epsCmnFnc.getTime();	// ランダム
	EPS_DBGPRINT(("use keys : %d\n", useBits));
	if( 0 == useBits ){
		useBits = 41;	// 0以外の適当な値にしておく
	}

	memset(key, 0, 16);
	for(n = 0; n < 16; n++){
		if( (useBits >> (15-n)) & 1 ){
			for(m = 0; m < 16; m++){
				key[m] ^= keyTable[n][m];
			}
		}
	}

	bits[0] = (useBits >> 8) & 0x00FF;
	bits[1] =  useBits       & 0x00FF;
}


static void GetEncryptKey(const EPS_UINT8 midKey[2], EPS_UINT8 encKey[2])
{
	EPS_INT32	n = 0;

	encKey[0] = encKey[1] = 0;
	for(n = 0; n < 8; n++){
		encKey[0] |= (midKey[n] & 0x80) >> n;
	}
	for(; n < 16; n++){
		encKey[1] |= (midKey[n] & 0x80) >> (n-8);
	}
}


#define ENPC_MODE0_COEFFICIENT(d) (5 + ((d&3)<<1) + ((d&7)>>2) )

static EPS_ERR_CODE	encodeMode0(
	
		EPS_UINT8*	buf,
		EPS_INT32*	extPduSize, 
		const EPS_UINT8	midKey[16]
		
){
	EPS_ERR_CODE ret = EPS_ERR_NONE;
	EPS_UINT8	encKey[2];
	EPS_UINT8	tmp;
	EPS_INT32	n = 0;
	EPS_INT32	rNum, ro;

	EPS_DBGPRINT(("=============== encode ================\n"));

	*extPduSize = (*extPduSize & 0x01)?(*extPduSize+1):*extPduSize;	/* 16の倍数bit */

	GetEncryptKey(midKey, encKey);

	/* ローテイト数 */
	rNum = ENPC_MODE0_COEFFICIENT(encKey[1]);
	if(rNum < 8) {
		ro = rNum;
	}
	else {
		ro = rNum - 8;
	}

	for(n = 0; n < *extPduSize; n+=2){
		/* 16bit毎にXOR */
		buf[n]   ^= encKey[0];
		buf[n+1] ^= encKey[1];

		/* 16bit毎に左ローテイト */
		if(rNum < 8) {
			tmp  	 = (buf[n] << ro)   | (buf[n+1] >> (8 - ro));
			buf[n+1] = (buf[n+1] << ro) | (buf[n] >> (8 - ro));
			buf[n]   = tmp;
		} else {
			tmp      = (buf[n+1] << ro) | (buf[n] >> (8 - ro));
			buf[n+1] = (buf[n] << ro)   | (buf[n+1] >> (8 - ro));
			buf[n]   = tmp;
		}
	}

	return ret;
}


static EPS_ERR_CODE	decodeMode0(
	
		EPS_UINT8*	buf,
		EPS_INT32	extPduSize, 
		const EPS_UINT8	midKey[16]
		
){
	EPS_ERR_CODE ret = EPS_ERR_NONE;
	EPS_UINT8	encKey[2];
	EPS_UINT8	tmp;
	EPS_INT32	n = 0;
	EPS_INT32	rNum, ro;

	EPS_DBGPRINT(("=============== decode ================\n"));

	GetEncryptKey(midKey, encKey);

	/* ローテイト数 */
	rNum = ENPC_MODE0_COEFFICIENT(encKey[1]);
	if(rNum < 8) {
		ro = rNum;
	}
	else {
		ro = rNum - 8;
	}

	for(n = 0; n < extPduSize; n+=2){
		/* 16bit毎に右ローテイト */
		if(rNum < 8) {
			tmp  	 = (buf[n] >> ro)   | (buf[n+1] << (8 - ro));
			buf[n+1] = (buf[n+1] >> ro) | (buf[n] << (8 - ro));
			buf[n]   = tmp;
		} else {
			tmp  	 = (buf[n+1] >> ro) | (buf[n] << (8 - ro));
			buf[n+1] = (buf[n] >> ro)   | (buf[n+1] << (8 - ro));	
			buf[n]   = tmp;
		}

		/* 16bit毎にXOR */
		buf[n]   ^= encKey[0];
		buf[n+1] ^= encKey[1];
	}

	return ret;
}


static EPS_ERR_CODE	SecSetAuthParameter(
	
		EPS_INT8*	buf, 
		EPS_INT32	nBufLen, 
		const EPS_UINT8 midKey[16],
		EPS_INT8*	pReplace, 
		EPS_GetMD5	md5Callback
		
){
	EPS_UINT8	md5Buf[32];
	EPS_UINT8	midKeyBuf[32];

	if(0 != md5Callback((EPS_UINT8*)buf, nBufLen, md5Buf) ){
		return EPS_ERR_OPR_FAIL;
	}

	memcpy(midKeyBuf,    midKey, 16);
	memcpy(midKeyBuf+16, md5Buf, 16);
	if(0 != md5Callback(midKeyBuf, 32, md5Buf) ){
		return EPS_ERR_OPR_FAIL;
	}

	snmpMakeStrField((EPS_INT8*)md5Buf, 12, pReplace);	/* 上位12OCTETでepAuthParameter置換 */

	return EPS_ERR_NONE;
}
/*__________________________________   epson-wifi.c   __________________________________*/

/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*       1         2         3         4         5         6         7         8        */
/*******************************************|********************************************/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/***** End of File *** End of File *** End of File *** End of File *** End of File ******/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

