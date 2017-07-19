#include "EPSReplyParser.h"
#include <QDebug>

EPSReplyParser* EPSReplyParser::pSelf = NULL;

EPSReplyParser::EPSReplyParser()
{
}

EPSReplyParser::~EPSReplyParser()
{

}

EPSReplyParser* EPSReplyParser::GetInstance()
{
    if (!pSelf)
    {
        pSelf = new EPSReplyParser();
    }
    return pSelf;
}

void EPSReplyParser::ReleaseInstance()
{
    if (pSelf)
    {
        delete pSelf;
        pSelf = NULL;
        //eprpRelease();
    }
}

bool EPSReplyParser::InitializeParser()
{
    /*int ret = -1;
    EPRP_FUNC      stRpFunc;

    stRpFunc.size               = sizeof( stRpFunc );
    stRpFunc.memAlloc           = &epsmpMemAlloc;
    stRpFunc.memFree            = &epsmpMemFree;
    stRpFunc.version            = EPRP_FUNC_VERCUR;

    ret = eprpInitialize(&stRpFunc);
    if (ret != EPS_ERR_NONE)
    {
        qDebug()<<"Init reply parser failed: "<<ret;
        return false;
    }*/

    return true;
}

/*bool EPSReplyParser::ParseStatus(EPRP_STATUS* status, const EPS_UINT8* buffer, EPS_INT32 bufferSize)
{
    if (!status)
    {
        return false;
    }
    if (!buffer)
    {
        return false;
    }
    EPS_ERR_CODE ret = eprpParseStatus(buffer, bufferSize, status);
    return (EPS_ERR_NONE == ret);
    return true;
}

bool EPSReplyParser::ParseInkInfo(EPRP_INK_INFO* inkInfo, const EPS_UINT8* buffer, EPS_INT32 bufferSize)
{
    if (!inkInfo)
    {
        return false;
    }
    if (!buffer)
    {
        return false;
    }
    EPS_ERR_CODE ret = eprpParseInkInfo(buffer, bufferSize, inkInfo);
    qDebug()<<"Parse ink info error code: "<<ret;
    return (EPS_ERR_NONE == ret);
    return true;
}*/



