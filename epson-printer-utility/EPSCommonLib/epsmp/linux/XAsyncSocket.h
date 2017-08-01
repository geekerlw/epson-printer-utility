#ifndef	_XAsyncSocket_Header
#define	_XAsyncSocket_Header

#define noErr 0
#define abortErr -1
//#define ETIMEDOUT -2
//#define errno -3

//#define EINTR 100
//#define ECANCELED 200


typedef int OSStatus;
typedef bool Boolean;

// 2nd parameter "key" of Select().
enum {
	kXAsyncSocketSelect_Write	= 1,
	kXAsyncSocketSelect_Read,
	kXAsyncSocketSelect_Connect,
	kXAsyncSocketSelect_Accept
};


//-------------------------------------
class XAsyncSocket
{
public:
						XAsyncSocket();
	virtual				~XAsyncSocket();

	virtual	OSStatus	Open();
	virtual	OSStatus	Close();
	virtual	OSStatus	Cancel();
	virtual	OSStatus	Select(int sock, long key, long timeoutMilliSec);
											// timeoutMilliSec==0 means non timeout.
	virtual	void		ClearCancel()	{ mCancel = false; }

protected:
	virtual	Boolean		IsCancel()	{ return(mCancel); }

private:
	void		InitMember();

    int 				mSndCancelSock;
    int 				mRcvCancelSock;
	volatile Boolean	mCancel;
};
//-------------------------------------

#endif //	_XAsyncSocket_Header

