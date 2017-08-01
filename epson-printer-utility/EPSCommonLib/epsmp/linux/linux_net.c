/*__________________________________   linux-net.c   ___________________________________*/

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

#if defined (unix) || (__unix)
/*------------------------------------  Includes   -------------------------------------*/
/*******************************************|********************************************/
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <errno.h>
#include <net/if.h>
#include "unistd.h"
#include "epson-escpr-def.h"
#include "epson-escpr-err.h"
#include "linux_net.h"

/*--------------------------------  Function Declarations   ----------------------------*/
/*******************************************|********************************************/

/*******************************************|********************************************/
/* Function name:   epsmpSocket()                                                       */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* domain       EPS_INT32           I: domain.                                          */
/* type         EPS_INT32           I: socket type.                                     */
/* protocol     EPS_INT32           I: protocol.                                        */
/*                                                                                      */
/* Return value:                                                                        */
/*      (valid socket discripter)   - Success                                           */
/*      EPS_INVALID_SOCKET          - Failure                                           */
/*                                                                                      */
/* Description:                                                                         */
/*      Create a socket discripter.                                                     */
/*                                                                                      */
/*******************************************|********************************************/
EPS_SOCKET epsmpSocket(
					   
			EPS_INT32 domain, 
			EPS_INT32 type, 
			EPS_INT32 protocol
			
){
    EPS_SOCKET sock;

    switch (type) {
    case EPS_SOCK_STREAM:
		switch (protocol){
		case EPS_PROTOCOL_TCP:
			sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			break;
		case EPS_PROTOCOL_UDP:
            sock = socket(AF_INET, SOCK_STREAM, IPPROTO_UDP);
			break;
		default:
			return EPS_INVALID_SOCKET;
		}
		break;

	case EPS_SOCK_DGRAM:
		switch (protocol){
		case EPS_PROTOCOL_TCP:
			sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_TCP);
			break;
		case EPS_PROTOCOL_UDP:
			sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			break;
		default:
			return EPS_INVALID_SOCKET;
		}
		break;

	default:
		return EPS_INVALID_SOCKET;
    }

	if( EPS_INVALID_SOCKET != sock ){
#if 0
		if(EPS_SOCK_DGRAM == type && EPS_PROTOCOL_UDP == protocol){
			epsmpBindInterface(sock);
		}
#endif
		return (EPS_SOCKET)sock;
	} else{
		return EPS_INVALID_SOCKET;
	}
}


/*******************************************|********************************************/
/* Function name:   epsmpClose()                                                        */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* sockfd       EPS_SOCKET          I: socket discripter.                               */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPS_SOCKET_SUCCESS          - Success                                           */
/*      EPS_SOCKET_ERROR            - Failure                                           */
/*                                                                                      */
/* Description:                                                                         */
/*      Destroy a socket discripter.                                                    */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32 epsmpClose(
					 
			EPS_SOCKET sockfd
			
){
	if( 0 == close((EPS_SOCKET)sockfd) ){
		return EPS_SOCKET_SUCCESS;
	} else{
		return EPS_SOCKET_ERROR;
	}
}


/*******************************************|********************************************/
/* Function name:   epsmpConnect()                                                      */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* sockfd       EPS_SOCKET          I: socket discripter.                               */
/* remoteHost   const EPS_INT8*     I: IP Address of remote host.                       */
/* remotePort   EPS_UINT16          I: Number of remote port.                           */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPS_SOCKET_SUCCESS          - Success                                           */
/*      EPS_SOCKET_ERROR            - Failure                                           */
/*                                                                                      */
/* Description:                                                                         */
/*      Establish the connection to remote host.                                        */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32 epsmpConnect(
					   
			EPS_SOCKET		sockfd, 
			const EPS_INT8* remoteHost, 
			EPS_UINT16		remotePort
			
){
	struct sockaddr_in addr;
	int val = 1;
	addr.sin_addr.s_addr = inet_addr(remoteHost);
	addr.sin_family = AF_INET;
	addr.sin_port = htons( remotePort );
	
	if(connect((EPS_SOCKET)sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == 0){
		ioctl(sockfd, FIONBIO, &val);
		return EPS_SOCKET_SUCCESS;
	} else{
		return EPS_SOCKET_ERROR;
	}
}


/*******************************************|********************************************/
/* Function name:   epsmpShutdown()                                                     */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* sockfd       EPS_SOCKET          I: socket discripter.                               */
/* type			EPS_INT32           I: Method.                                          */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPS_SOCKET_SUCCESS          - Success                                           */
/*      EPS_SOCKET_ERROR            - Failure                                           */
/*                                                                                      */
/* Description:                                                                         */
/*      Disconnect to remote host.                                                      */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32 epsmpShutdown(
						
			EPS_SOCKET	sockfd, 
			EPS_INT32	type
			
){
	int ret = 0;
    switch (type){
    case EPS_SHUTDOWN_SEND:
        ret = shutdown((EPS_SOCKET)sockfd, SHUT_WR);
        break;
    case EPS_SHUTDOWN_RECV:
        ret = shutdown((EPS_SOCKET)sockfd, SHUT_RD);
        break;
    case EPS_SHUTDOWN_BOTH:
        ret = shutdown((EPS_SOCKET)sockfd, SHUT_RDWR);
        break;
	default:
		ret = -1;
    }

	if( 0 == ret ){
		return EPS_SOCKET_SUCCESS;
	} else{
		return EPS_SOCKET_ERROR;
	}
}


/*******************************************|********************************************/
/* Function name:   epsmpSend()                                                         */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* sockfd       EPS_SOCKET          I: socket discripter.                               */
/* msg			const void*         I: Pointer to data buffer.                          */
/* len			EPS_INT32           I: lenght of the buffer.                            */
/* timeout		EPS_UINT32          I: time out.                                        */
/*                                                                                      */
/* Return value:                                                                        */
/*      (sent size)                 - Success                                           */
/*      EPS_SOCKET_ERROR            - Failure                                           */
/*      EPS_SOCKET_TIMEOUT          - Processing did the time-out                       */
/*                                                                                      */
/* Description:                                                                         */
/*      Send message to remote host.                                                    */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32 epsmpSend(
					
			EPS_SOCKET		sockfd, 
			const void*		msg, 
			EPS_INT32		len, 
			EPS_UINT32		timeout
			
){
	EPS_INT32 ret = EPS_SOCKET_SUCCESS;
	int numSocket = 0;
	struct pollfd pfd;

	pfd.fd = (EPS_SOCKET)sockfd;
	pfd.events = POLLOUT;
	pfd.revents = POLLERR;

	numSocket = poll(&pfd, 1, timeout);
	if(numSocket > 0){
		ret = send((EPS_SOCKET)sockfd, msg, len, 0);
		if(-1 == ret){
			ret = EPS_SOCKET_ERROR;
		}
	} else if(numSocket == 0){
		ret = EPS_SOCKET_TIMEOUT;
	} else{
	    ret = EPS_SOCKET_ERROR;
	}

	return ret;
}


/*******************************************|********************************************/
/* Function name:   epsmpShutdown()                                                     */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* sockfd       EPS_SOCKET          I: socket discripter.                               */
/* msg			const void*         I: Pointer to data buffer.                          */
/* len			EPS_INT32           I: lenght of the buffer.                            */
/* remoteHost   const EPS_INT8*     I: IP Address of remote host.                       */
/* remotePort   EPS_UINT16          I: Number of remote port.                           */
/* timeout		EPS_UINT32          I: time out.                                        */
/*                                                                                      */
/* Return value:                                                                        */
/*      (sent size)                 - Success                                           */
/*      EPS_SOCKET_ERROR            - Failure                                           */
/*      EPS_SOCKET_TIMEOUT          - Processing did the time-out                       */
/*                                                                                      */
/* Description:                                                                         */
/*      Send message to remote host.                                                    */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32 epsmpSendTo(
					  
			EPS_SOCKET		sockfd, 
			const void*		msg, 
			EPS_INT32		len, 
			const EPS_INT8* remoteHost, 
			EPS_UINT16		remotePort, 
			EPS_UINT32		timeout
			
){
	EPS_INT32 ret = EPS_SOCKET_SUCCESS;
	int numSocket = 0;
	struct pollfd pfd;

	pfd.fd = (EPS_SOCKET)sockfd;
	pfd.events = POLLOUT;
	pfd.revents = POLLERR;

	numSocket = poll(&pfd, 1, timeout);

	if(numSocket > 0){
		struct sockaddr_in addr;
		addr.sin_addr.s_addr = inet_addr(remoteHost);
		addr.sin_family = AF_INET;
		addr.sin_port = htons( remotePort );

	    ret = sendto((EPS_SOCKET)sockfd, msg, len, 0, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
		if(-1 == ret){
			ret = EPS_SOCKET_ERROR;
		}
	} else if(numSocket == 0){
		ret = EPS_SOCKET_TIMEOUT;
	} else{
	    ret = EPS_SOCKET_ERROR;
	}

	return ret;
}

#if LCOMSW_USE_MULTI_IF | LCOMSW_BINDIF_IF_NEED
#define MACADDR_LEN	(6)
#define ESCPR_CNF_BUF_LEN (sizeof(struct ifreq) * 32 + sizeof(int)) 
#define ESCPR_RIF_FLGS (IFF_UP | IFF_RUNNING | IFF_BROADCAST)

//#define USE_MAC_ADDRESS
/*******************************************|********************************************/
/* Function name:   epsmpEnumInterface()                                                */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* buf          EPS_UINT8*          I: interface names buffer.                          */
/* bufSize      EPS_UINT32          I: size of "buf".                                   */
/*                                                                                      */
/* Return value:                                                                        */
/*      (sent size)                 - count of interfaces                               */
/*      EPS_SOCKET_ERROR            - Failure                                           */
/*                                                                                      */
/* Description:                                                                         */
/*      enumerate interfaces.                                                           */
/*                                                                                      */
/*******************************************|********************************************/
#if LCOMSW_USE_MULTI_IF
EPS_INT32 epsmpEnumInterface(EPS_UINT8* buf, EPS_UINT32 bufSize)
{
	int ret = 0;

	int sockfd = 0;
	char lobuf[ESCPR_CNF_BUF_LEN];
	char *p = NULL;
	struct ifconf conf;
	struct ifreq *req, reqFlg;
	struct sockaddr *addr;
	int len;
	int ifCount = 0;
	EPS_UINT32 pos = 0;
	
	memset(lobuf, 0, ESCPR_CNF_BUF_LEN);
	conf.ifc_len = ESCPR_CNF_BUF_LEN;
	conf.ifc_ifcu.ifcu_buf = lobuf;
	
	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	ret = ioctl(sockfd, SIOCGIFCONF, &conf);
	if (0 != ret) {
		return ret;
	}

	for (p = lobuf; p < lobuf + conf.ifc_len; ) {
		req = (struct ifreq *)p;
		addr = &(req->ifr_ifru.ifru_addr);
		len = (sizeof(struct sockaddr) > addr->sa_family)?sizeof(struct sockaddr):addr->sa_family;
		p += sizeof(req->ifr_name) + len; 

//		printf("%s : %s : %08X\n", req->ifr_name, inet_ntoa(((struct sockaddr_in*)addr)->sin_addr), ntohl(((struct sockaddr_in*)addr)->sin_addr.s_addr));
		if(AF_INET !=((struct sockaddr_in*)addr)->sin_family){
			continue;
		}

		reqFlg = *req;
#ifdef USE_MAC_ADDRESS
		ret = ioctl(sockfd, SIOCGIFHWADDR, &reqFlg);
		addr = &(reqFlg.ifr_ifru.ifru_broadaddr);
		//printf("\t%s\n", inet_ntoa(((struct sockaddr_in*)addr)->sin_addr));
		printf("\t%.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",
		    (unsigned char)addr->sa_data[0],
		    (unsigned char)addr->sa_data[1],
		    (unsigned char)addr->sa_data[2],
		    (unsigned char)addr->sa_data[3],
		    (unsigned char)addr->sa_data[4],
		    (unsigned char)addr->sa_data[5]);

		if( 0 != memcmp(addr->sa_data, "\0\0\0\0\0\0", MACADDR_LEN) ){
			ifCount++;
			if(pos + EPS_IFNAME_LEN <= bufSize){
				memcpy(buf+pos, addr->sa_data, MACADDR_LEN);
				pos += EPS_IFNAME_LEN;
			}
		}
#else
		ret = ioctl(sockfd, SIOCGIFFLAGS, &reqFlg);
		if ((reqFlg.ifr_ifru.ifru_flags & ESCPR_RIF_FLGS) == ESCPR_RIF_FLGS && 
			(reqFlg.ifr_ifru.ifru_flags & IFF_LOOPBACK) == 0) {
			ifCount++;
			if(pos + EPS_IFNAME_LEN <= bufSize){
				strncpy((char*)buf+pos, req->ifr_name, EPS_IFNAME_LEN);
				buf[pos+EPS_IFNAME_LEN-1] = '\0';
				pos += EPS_IFNAME_LEN;
			}
		}
#endif
	}
//	printf("ifCount %d\n", ifCount);

	close(sockfd);

	return ifCount;
}
#endif


/*******************************************|********************************************/
/* Function name:   epsmpBindInterface()                                                */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* sockfd       EPS_SOCKET          I: socket discripter.                               */
/* ifName       const EPS_UINT8*    I: interface name.                                  */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPS_SOCKET_SUCCESS          - Success                                           */
/*      EPS_SOCKET_ERROR            - Failure                                           */
/*                                                                                      */
/* Description:                                                                         */
/*      Bind interface.                                                                 */
/*                                                                                      */
/*******************************************|********************************************/
#if LCOMSW_USE_MULTI_IF
EPS_INT32 epsmpBindInterface(EPS_SOCKET sockfd, const EPS_UINT8* ifName)
#elif LCOMSW_BINDIF_IF_NEED
EPS_INT32 epsmpBindInterface(EPS_SOCKET sockfd)
#endif
{
	int ret = EPS_SOCKET_SUCCESS;
	char buf[ESCPR_CNF_BUF_LEN];
	char *p = NULL;
	struct ifconf conf;
	struct ifreq *req, reqFlg;
	struct sockaddr *addr;
	int len;
	
	memset(buf, 0, ESCPR_CNF_BUF_LEN);
	conf.ifc_len = ESCPR_CNF_BUF_LEN;
	conf.ifc_ifcu.ifcu_buf = buf;

	ret = ioctl(sockfd, SIOCGIFCONF, &conf);
	if (0 != ret) {
		return ret;
	}

	for (p = buf; p < buf + conf.ifc_len; ) {
		req = (struct ifreq *)p;
		addr = &(req->ifr_ifru.ifru_addr);
		len = (sizeof(struct sockaddr) > addr->sa_family)?sizeof(struct sockaddr):addr->sa_family;
		p += sizeof(req->ifr_name) + len; 

//		printf("%s : %s : %08X\n", req->ifr_name, inet_ntoa(((struct sockaddr_in*)addr)->sin_addr), ntohl(((struct sockaddr_in*)addr)->sin_addr.s_addr));
		if(AF_INET !=((struct sockaddr_in*)addr)->sin_family){
			continue;
		}
			
		reqFlg = *req;
#if LCOMSW_USE_MULTI_IF

#ifdef USE_MAC_ADDRESS
		ret = ioctl(sockfd, SIOCGIFHWADDR, &reqFlg);
		if (0 == ret) {
			if( 0 == memcmp(reqFlg.ifr_ifru.ifru_broadaddr.sa_data, ifName, MACADDR_LEN) ){
//				printf("Bind %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",
//				     ifName[0], ifName[1], ifName[2], ifName[3], ifName[4], ifName[5]);
#else
		if (0 == ret) {
			if( 0 == strcmp((char*)req->ifr_name, (char*)ifName) ){
//				printf("Bind %s\n", ifName);
#endif

#elif LCOMSW_BINDIF_IF_NEED
		ret = ioctl(sockfd, SIOCGIFFLAGS, &reqFlg);
		if (0 == ret) {
			/*	printf("\t UP:%d, RUNNING:%d, BROADCAST:%d, MULTICAST:%d, LOOPBACK:%d, NOARP:%d\n", 
				reqFlg.ifr_ifru.ifru_flags & IFF_UP, 
				reqFlg.ifr_ifru.ifru_flags & IFF_RUNNING, 
				reqFlg.ifr_ifru.ifru_flags & IFF_BROADCAST, 
				reqFlg.ifr_ifru.ifru_flags & IFF_MULTICAST, 
				reqFlg.ifr_ifru.ifru_flags & IFF_LOOPBACK, 
				reqFlg.ifr_ifru.ifru_flags & IFF_NOARP);
				*/
			if ((reqFlg.ifr_ifru.ifru_flags & ESCPR_RIF_FLGS) == ESCPR_RIF_FLGS && 
				(reqFlg.ifr_ifru.ifru_flags & IFF_LOOPBACK) == 0) {
#endif
				ret = bind(sockfd, addr, sizeof(struct sockaddr_in));
				if (0 == ret) {
					break;
				} else{
					printf("bind err %d\n", errno);
				}
			}
		}
	}

	return ret;
}
#endif /* LCOMSW_USE_MULTI_IF | LCOMSW_BINDIF_IF_NEED */

/*******************************************|********************************************/
/* Function name:   epsmpReceive()                                                      */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* sockfd       EPS_SOCKET          I: socket discripter.                               */
/* Data         EPS_UINT8*          O: Pointer to a location to read the data to.       */
/* Length       EPS_INT32           I: Length of the data (in bytes).                   */
/* timeout		EPS_UINT32          I: time out.                                        */
/*                                                                                      */
/* Return value:                                                                        */
/*      (received size)             - Success                                           */
/*      EPS_SOCKET_ERROR            - Failure                                           */
/*      EPS_SOCKET_TIMEOUT          - Processing did the time-out                       */
/*                                                                                      */
/* Description:                                                                         */
/*      Receive data from remote host.                                                  */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32 epsmpReceive(
					   
			EPS_SOCKET	sockfd, 
			void*		buf, 
			EPS_INT32	len, 
			EPS_UINT32	timeout
			
){
	EPS_INT32 ret = EPS_SOCKET_SUCCESS;
	int numSocket = 0;
	struct pollfd pfd;

	pfd.fd = (EPS_SOCKET)sockfd;
	pfd.events = POLLIN;
	pfd.revents = POLLERR;

	numSocket = poll(&pfd, 1, timeout);
	if(numSocket > 0){
		ret = recv((EPS_SOCKET)sockfd, buf, len, 0);
		if(-1 == ret){
			ret = EPS_SOCKET_ERROR;
		}
	} else if(numSocket == 0){
		ret = EPS_SOCKET_TIMEOUT;
	} else{
	    ret = EPS_SOCKET_ERROR;
	}

	return ret;
}


/*******************************************|********************************************/
/* Function name:   epsmpReceiveFrom()                                                  */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* sockfd       EPS_SOCKET          I: socket discripter.                               */
/* Data         EPS_UINT8*          O: Pointer to a location to read the data to.       */
/* Length       EPS_INT32           I: Length of the data (in bytes).                   */
/* timeout		EPS_UINT32          I: time out.                                        */
/* remotePort   EPS_UINT16          O: Number of remote port.                           */
/* timeout		EPS_UINT32          O: time out.                                        */
/*                                                                                      */
/* Return value:                                                                        */
/*      (received size)             - Success                                           */
/*      EPS_SOCKET_ERROR            - Failure                                           */
/*      EPS_SOCKET_TIMEOUT          - Processing did the time-out                       */
/*                                                                                      */
/* Description:                                                                         */
/*      Receive data from remote host.                                                  */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32 epsmpReceiveFrom(
						   
			EPS_SOCKET	sockfd, 
			void*		buf, 
			EPS_INT32	len, 
			EPS_INT8*	remoteHost, 
			EPS_UINT16* remotePort,
			EPS_UINT32	timeout 
			
){
	EPS_INT32 ret = EPS_SOCKET_SUCCESS;
	int numSocket = 0;
	struct pollfd pfd;
	struct sockaddr_in addr;
	socklen_t nAddrSize = sizeof(addr);

	pfd.fd = (EPS_SOCKET)sockfd;
	pfd.events = POLLIN;
	pfd.revents = POLLERR;

	numSocket = poll(&pfd, 1, timeout);
	if(numSocket > 0){
		ret = recvfrom((EPS_SOCKET)sockfd, buf, len, 0, (struct sockaddr *)&addr, &nAddrSize);
		if(-1 == ret){
			ret = EPS_SOCKET_ERROR;
		} else{
			strcpy(remoteHost, inet_ntoa(addr.sin_addr));
			*remotePort = ntohs( addr.sin_port );
		}
	} else if(numSocket == 0){
		ret = EPS_SOCKET_TIMEOUT;
	} else{
	    ret = EPS_SOCKET_ERROR;
	}

	return ret;
}


/*******************************************|********************************************/
/* Function name:   epsmpSetMulticastTTL()                                              */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* sockfd       EPS_SOCKET          I: socket discripter.                               */
/* TTL          EPS_INT32           I: Time To Live value.                              */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPS_SOCKET_SUCCESS          - Success                                           */
/*      EPS_SOCKET_ERROR            - Failure                                           */
/*                                                                                      */
/* Description:                                                                         */
/*      Set a number of multicast packet TimeToLive.                                    */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32 epsmpSetMulticastTTL(
							   
			EPS_SOCKET sockfd, 
			EPS_INT32 TTL
			
){
	EPS_INT32 ret = EPS_SOCKET_SUCCESS;

	if( 0 == setsockopt((EPS_SOCKET)sockfd, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&TTL, sizeof(TTL)) ){
		ret = EPS_SOCKET_SUCCESS;
	} else{
		ret = EPS_SOCKET_ERROR;
	}

	return ret;
}


/*******************************************|********************************************/
/* Function name:   epsmpSetBroadCast()                                                 */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:        Type:               Description:                                        */
/* sockfd       EPS_SOCKET          I: socket discripter.                               */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPS_SOCKET_SUCCESS          - Success                                           */
/*      EPS_SOCKET_ERROR            - Failure                                           */
/*                                                                                      */
/* Description:                                                                         */
/*      The socket is made a possible broadcast.                                        */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32 epsmpSetBroadCast(
							
			EPS_SOCKET sockfd
			
){
	EPS_INT32 ret = EPS_SOCKET_SUCCESS;
	EPS_BOOL b = TRUE;

	if( 0 == setsockopt((EPS_SOCKET)sockfd, SOL_SOCKET, SO_BROADCAST, (char*)&b, sizeof(b)) ){
		ret = EPS_SOCKET_SUCCESS;
	} else{
		ret = EPS_SOCKET_ERROR;
	}

	return ret;
}



#if 0	/* not use now */
EPS_INT32 epsmpBind(EPS_SOCKET sockfd, EPS_UINT16 localPort, EPS_UINT16* bindedPort)
{          
	EPS_INT32 ret = EPS_ERR_NONE;
	struct sockaddr_in addr;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_family = AF_INET;
	addr.sin_port = htons( localPort );

	ret = bind((EPS_SOCKET)sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
	if(0 == ret){
		socklen_t nAddrSize = sizeof(addr);
	    ret = getsockname((EPS_SOCKET)sockfd, (struct sockaddr *)&addr, &nAddrSize);
		if(0 == ret){
			*bindedPort = ntohs( addr.sin_port );
		} else{
		    ret = EPS_SOCKET_ERROR;
		}
	} else{
		ret = EPS_SOCKET_ERROR;
	}

	return ret;
}


EPS_INT32 epsmpListen(EPS_SOCKET sockfd, EPS_INT32 backlog)
{
	if( 0 == listen((EPS_SOCKET)sockfd, backlog) ){
		return EPS_SOCKET_SUCCESS;
	} else{
		return EPS_SOCKET_ERROR;
	}
}


EPS_INT32 epsmpAccept(EPS_SOCKET sockfd, EPS_UINT32 timeout, 
					EPS_SOCKET* socClient, EPS_INT8* address, EPS_UINT16* port)
{
	EPS_INT32 ret = EPS_SOCKET_SUCCESS;
	int numSocket = 0;
	struct sockaddr_in addr;
	struct pollfd pfd;
	socklen_t addrlen = sizeof(addr);
	*socClient = EPS_INVALID_SOCKET;

	pfd.fd = (EPS_SOCKET)sockfd;
	pfd.events = POLLIN;
	pfd.revents = POLLERR;

	numSocket = poll(&pfd, 1, timeout);

	if(numSocket > 0){
		*socClient = (int)accept((EPS_SOCKET)sockfd, (struct sockaddr *)&addr, &addrlen);
		if(*socClient != EPS_INVALID_SOCKET) {
			strcpy(address, inet_ntoa(addr.sin_addr));
			*port = ntohs(addr.sin_port);
		} else{
		   ret = EPS_SOCKET_ERROR;
		}
	} else if(numSocket == 0){
		ret = EPS_SOCKET_TIMEOUT;
	} else{
	    ret = EPS_SOCKET_ERROR;
	}

	return ret;
}

EPS_INT32 epsmpGetSockName(EPS_SOCKET sockfd, EPS_INT8* Address, EPS_UINT16* port)
{
	EPS_INT32 ret = EPS_SOCKET_SUCCESS;
	struct sockaddr_in addr;
	socklen_t nAddrSize = sizeof(addr);

	if( 0 == getsockname((EPS_SOCKET)sockfd, (struct sockaddr *)&addr, &nAddrSize) ){
		strcpy(Address, inet_ntoa(addr.sin_addr));
		*port = ntohs( addr.sin_port );
		ret = EPS_SOCKET_SUCCESS;
	} else{
		ret = EPS_SOCKET_ERROR;
	}

	return ret;
}


EPS_INT32 epsmpSetMulticastMembership(EPS_SOCKET sockfd, const EPS_INT8* remoteHost)
{
	EPS_INT32 ret = EPS_SOCKET_SUCCESS;
	struct ip_mreq stMreq;

	memset(&stMreq, 0, sizeof(stMreq));
	stMreq.imr_multiaddr.s_addr = inet_addr(remoteHost);
	stMreq.imr_interface.s_addr = INADDR_ANY;

	if( 0 == setsockopt((EPS_SOCKET)sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&stMreq, sizeof(stMreq)) ){
		ret = EPS_SOCKET_SUCCESS;
	} else{
		ret = EPS_SOCKET_ERROR;
	}

	return ret;
}


EPS_INT32 epsmpSetReuseAddr(EPS_SOCKET sockfd)
{
	EPS_INT32 ret = EPS_SOCKET_SUCCESS;
	EPS_BOOL b = TRUE;

	if( 0 == setsockopt((EPS_SOCKET)sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&b, sizeof(b)) ){
		ret = EPS_SOCKET_SUCCESS;
	} else{
		ret = EPS_SOCKET_ERROR;
	}

	return ret;

}
#endif	/* not use now */

#endif  /* defined (unix) || (__unix) */

/*__________________________________   linux-net.c   ___________________________________*/
  
/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*       1         2         3         4         5         6         7         8        */
/*******************************************|********************************************/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/***** End of File *** End of File *** End of File *** End of File *** End of File ******/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
