//#define		IF_EPIOSeparateDebugMode

#include	"XAsyncSocket.h"
#include	<stdio.h>
//#include	<unistd.h>
#include	<winsock.h>
#include	<Windows.h>
//#include	<sys/socket.h>
#include	<fcntl.h>
//#include	<netinet/in.h>
//#include	<netdb.h>
//#include	<arpa/inet.h>
//#include	<sys/time.h>
#include	<time.h>

#pragma comment(lib, "Ws2_32.lib")

//------------------------------------------------------
XAsyncSocket::XAsyncSocket()
{
    InitMember();
}
//------------------------------------------------------
XAsyncSocket::~XAsyncSocket()
{
}
//------------------------------------------------------
void XAsyncSocket::InitMember()
{
	mCancel				= false;
    mSndCancelSock		= -1;
    mRcvCancelSock		= -1;
}
//------------------------------------------------------
OSStatus XAsyncSocket::Open()
{
    int	err;
    int			tempSockPair[2];
    
//    printf(("XAsyncSocket::Open+++\n"));

    InitMember();

    //err = socketpair(AF_UNIX, SOCK_STREAM, 0, tempSockPair);
	/* todo : windows has no socketpair */
	err = socket(AF_UNIX, SOCK_STREAM, 0);
    if (err == noErr  &&  tempSockPair[0] >= 0  &&  tempSockPair[1] >= 0) {
        mSndCancelSock = tempSockPair[0];
        mRcvCancelSock = tempSockPair[1];
    }
	else {
		err = -1;
	}

    if (err != noErr) {
		XAsyncSocket::Close();
		err = -1;
	}
//    printf("XAsyncSocket::Open--- err = %d\n", err);
	return err;
}
//------------------------------------------------------
OSStatus XAsyncSocket::Close()
{
//    printf(("XAsyncSocket::Close+++\n"));

    if (mSndCancelSock != -1) {
		closesocket(mSndCancelSock);
		mSndCancelSock = -1;
	}
    if (mRcvCancelSock != -1) {
		closesocket(mRcvCancelSock);
		mRcvCancelSock = -1;
	}

//     printf(("XAsyncSocket::Close---\n"));
	return(noErr);
}
//------------------------------------------------------
OSStatus XAsyncSocket::Cancel()
{
    int		err = noErr;
    char		cancelData = 0;

    printf(("XAsyncSocket::Cancel+++\n"));

	mCancel = true;

	mCancel = true;
	if (mSndCancelSock >= 0) {
		err = (send(mSndCancelSock, &cancelData, sizeof(cancelData), MSG_DONTROUTE) > 0) ? noErr : -1;
	}

     printf("XAsyncSocket::Cancel--- err = %d\n", err);
	return err;
}
//------------------------------------------------------
//	return OSStatus of XAsyncSocket::Select is 4 type.
//		noErr
//		-1
//		abortErr
//		ETIMEDOUT
//------------------------------------------------------
OSStatus XAsyncSocket::Select(int sock, long key, long timeoutMilliSec)	// timeoutMilliSec==0 means non timeout.
{
    OSStatus			err				= -1;
    int					result, maxDescriptor;
    fd_set				readFDset, writeFDset, *checkFDset = NULL, *pWriteFDset = NULL;
    struct timeval		timeoutVal;
    struct timeval *	timeoutValPtr	= NULL;
    
//    printf(("XAsyncSocket::Select+++  ckeck = %02lX\n", ckeck));

	if (mRcvCancelSock < 0  ||  mCancel == true  ||  sock < 0) {
		goto done;	// err = -1
	}

	maxDescriptor = sock;
	if (mSndCancelSock > maxDescriptor) maxDescriptor = mSndCancelSock;
	if (mRcvCancelSock > maxDescriptor) maxDescriptor = mRcvCancelSock;

    FD_ZERO(&readFDset);
    FD_ZERO(&writeFDset);
    FD_SET(mRcvCancelSock, &readFDset);

    switch (key) {
	case kXAsyncSocketSelect_Write:
		FD_SET(sock, &writeFDset);
		checkFDset	= &writeFDset;
		pWriteFDset	= &writeFDset;
		break;

	case kXAsyncSocketSelect_Read:
	case kXAsyncSocketSelect_Accept:
		FD_SET(sock, &readFDset);
		checkFDset	= &readFDset;
		pWriteFDset	= NULL;
		break;

	case kXAsyncSocketSelect_Connect:
		FD_SET(sock, &readFDset);
		FD_SET(sock, &writeFDset);
		checkFDset	= NULL;
		pWriteFDset	= &writeFDset;
		break;

	default:
		goto done;	// err = -1
    }

	if (timeoutMilliSec > 0) {
		timeoutVal.tv_sec	= timeoutMilliSec / 1000L;
		timeoutVal.tv_usec	= (timeoutMilliSec % 1000L) * 1000L;
        timeoutValPtr		= &timeoutVal;
	}

	err = noErr;
    do {
        result = select(maxDescriptor + 1, &readFDset, pWriteFDset, NULL, timeoutValPtr);

//		printf ("result=%d\n", result);

		if (mCancel == true) {
			err = abortErr;
		}
		else if (result > 0) {
			if (FD_ISSET(mRcvCancelSock, &readFDset) != 0) {	// cancel
				err = abortErr;
			}
			else if (checkFDset == NULL  ||  FD_ISSET(sock, checkFDset) != 0) {
				break;	// exfire,  err = noErr.
			}
		}
        else if (result == 0) {// If the time limit expires, select() returns 0.
            err = ETIMEDOUT;
		}
		else {

//	"/usr/include/errno.h"
//		#define	ECANCELED	89

			err = errno;

//			printf ("errno=%d\n", err);

			if		(err == EINTR)		err = noErr;	// retry.
			else if (err == ECANCELED)	err = abortErr;
			else						err = -1;
		}
	} while (err == noErr);

done:
	if ((err == noErr  ||  err == ETIMEDOUT)  &&  mCancel == true)
		err = abortErr;
		
//    IF_DebugPrintf((err, "XAsyncSocket::Select--- err = %d\n", err));
    return err;
}
//------------------------------------------------------
