#ifndef EPSREPLYPARSER_H
#define EPSREPLYPARSER_H

#include "escpr-lib/inc/epson-escpr-api.h"
/*
#include "epson-escpr-media.h"
#include "epson-cbt.h"
#include "epson-protocol.h"
#include "epson-net-snmp.h"
#include "epson-net-lpr.h"
#include "epson-net-lpr.h"
#include "epson-usb.h"
#include "linux_cmn.h"
#include "linux_net.h"
*/

class EPSReplyParser
{
private:
    EPSReplyParser();
    virtual ~EPSReplyParser();

public:
    static EPSReplyParser* GetInstance();
    static void ReleaseInstance();

public:
    bool InitializeParser();

    //bool ParseStatus(EPRP_STATUS* status, const EPS_UINT8* buffer, EPS_INT32 bufferSize);
    //bool ParseInkInfo(EPRP_INK_INFO* inkInfo, const EPS_UINT8* buffer, EPS_INT32 bufferSize);

private:
    static EPSReplyParser* pSelf;
};

#endif // EPSREPLYPARSER_H
