/*__________________________________   linux-net.h   ___________________________________*/

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
/*                                  Linux Socket  I/O                                   */
/*                                                                                      */
/*                                    Function Calls                                    */
/*                              --------------------------                              */
/*		EPS_SOCKET	epsmpSocket		(domain, type, protocol							);  */
/*      EPS_INT32	epsmpClose		(sockfd											);  */
/*      EPS_INT32	epsmpConnect	(sockfd, remoteHost, remotePort					);  */
/*      EPS_INT32	epsmpShutdown	(sockfd, type									);  */
/*      EPS_INT32	epsmpSend		(sockfd, msg, len, timeout						);  */
/*      EPS_INT32	epsmpSendTo		(sockfd, msg, len, remoteHost, remotePort, timeout);*/
/*      EPS_INT32	epsmpReceive	(sockfd, buf, len, timeout						);  */
/*      EPS_INT32	epsmpReceiveFrom(sockfd, buf, len, remoteHost, remotePort, timeout);*/
/*      EPS_INT32	epsmpSetMulticastTTL(sockfd, TTL								);  */
/*      EPS_INT32	epsmpSetBroadCast(sockfd										);  */
/*                                                                                      */
/*******************************************|********************************************/

#ifndef __LINUX_NET__
#define __LINUX_NET__

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------  Includes   -------------------------------------*/
/*******************************************|********************************************/
#include "../../escpr-lib/inc/epson-escpr-def.h"


#define EPS_MACADDER_LEN	(6)
/*---------------------------  Public Function Declarations  ---------------------------*/
/*******************************************|********************************************/
EPS_SOCKET	epsmpSocket			(EPS_INT32, EPS_INT32, EPS_INT32						);
EPS_INT32	epsmpClose			(EPS_SOCKET												);
EPS_INT32	epsmpConnect		(EPS_SOCKET, const EPS_INT8*, EPS_UINT16				);
EPS_INT32	epsmpShutdown		(EPS_SOCKET, EPS_INT32									);
EPS_INT32	epsmpSend			(EPS_SOCKET, const void*, EPS_INT32, EPS_UINT32			);
EPS_INT32	epsmpSendTo			(EPS_SOCKET, const void*, EPS_INT32, 
								 const EPS_INT8*, EPS_UINT16, EPS_UINT32				);
EPS_INT32	epsmpReceive		(EPS_SOCKET, void*, EPS_INT32, EPS_UINT32				);
EPS_INT32	epsmpReceiveFrom	(EPS_SOCKET, void*, EPS_INT32, 
								EPS_INT8*, EPS_UINT16*, EPS_UINT32						);
EPS_INT32	epsmpSetMulticastTTL(EPS_SOCKET, EPS_INT32									);
EPS_INT32	epsmpSetBroadCast	(EPS_SOCKET												);

#if LCOMSW_USE_MULTI_IF
EPS_INT32	epsmpEnumInterface	(EPS_UINT8* , EPS_UINT32 bufSize                        );
EPS_INT32	epsmpBindInterface	(EPS_SOCKET sockfd, const EPS_UINT8*                    );
#elif LCOMSW_BINDIF_IF_NEED
EPS_INT32	epsmpBindInterface	(EPS_SOCKET sockfd                                      );
#endif

#if 0	/* not use now */
EPS_INT32	epsmpBind(EPS_SOCKET sockfd, EPS_UINT16 localPort, EPS_UINT16* bindedPort);
EPS_INT32	epsmpListen(EPS_SOCKET sockfd, EPS_INT32 backlog);
EPS_INT32	epsmpAccept(EPS_SOCKET sockfd, EPS_UINT32 timeout, 
					EPS_SOCKET* socClient, EPS_INT8* address, EPS_UINT16* port);
EPS_INT32	epsmpGetSockName(EPS_SOCKET sockfd, EPS_INT8* Address, EPS_UINT16* port);
EPS_INT32	epsmpSetMulticastMembership(EPS_SOCKET sockfd, const EPS_INT8* remoteHost);
EPS_INT32	epsmpSetReuseAddr(EPS_SOCKET sockfd);
#endif	/* not use now */

#ifdef __cplusplus
}
#endif


#endif	/* def __LINUX_NET__ */

/*__________________________________   linux-net.h   ___________________________________*/
  
/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*       1         2         3         4         5         6         7         8        */
/*******************************************|********************************************/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/***** End of File *** End of File *** End of File *** End of File *** End of File ******/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
