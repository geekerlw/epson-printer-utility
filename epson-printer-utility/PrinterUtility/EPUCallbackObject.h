#ifndef EPUCALLBACKOBJECT_H
#define EPUCALLBACKOBJECT_H
#include <QWidget>

enum
{
    EPU_CALLBACK_NONE = 0,
    EPU_CALLBACK_FINISH,
    EPU_CALLBACK_GET_NEXT_VIEW,
    EPU_CALLBACK_SHOW_STATUS_MONITOR,
    EPU_CALLBACK_SHOW_NOZZLE_CHECK,
    EPU_CALLBACK_SHOW_HEADCLEANING,
    EPU_CALLBACK_SHOW_PRINT_HEAD_ALIGNMENT
};

typedef int EPUCallbackCode;

class EPUCallbackObject
{
public:
    EPUCallbackObject();
    virtual ~EPUCallbackObject();

public:
    virtual void EPUViewDidCloseWithCode(QWidget* epuView, EPUCallbackCode code) = 0;
    virtual void EPUViewDisAble() = 0;
    virtual void EPUViewDisAbleHeadClean() = 0;
    virtual void EPUViewAbleNozzleCheck() = 0;
    virtual void EPUViewEnAble() = 0;
};

//typedef QSharedPointer<EPUCallbackObject> EPUCallbackObjectPtr;

#endif // EPUCALLBACKOBJECT_H
