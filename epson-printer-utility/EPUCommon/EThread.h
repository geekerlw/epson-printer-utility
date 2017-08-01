#ifndef ETHREAD_H
#define ETHREAD_H

#include <QThread>
#include <QMutex>
#include <QSemaphore>
#include <QStringList>

typedef unsigned int EThreadReturnValue;

class EThreadParam
{
public:
    EThreadParam()
        : m_boolParam(false)
        , m_intParam(0)
    {
        m_lstParam.clear();
    }

    EThreadParam(const int& number)
        : m_boolParam(false)
        , m_intParam(number)
    {
        m_lstParam.clear();
    }
    EThreadParam(const bool& flag)
        : m_boolParam(flag)
        , m_intParam(0)
    {
        m_lstParam.clear();
    }

    EThreadParam(const QStringList& lst)
        : m_boolParam(false)
        , m_intParam(0)
    {
        m_lstParam = lst;
    }

    EThreadParam(const QString& str)
        : m_boolParam(false)
        , m_intParam(false)
    {
        m_lstParam.push_back(str);
    }

public:
    bool getBoolValue()
    {
        return m_boolParam;
    }

    int getIntValue()
    {
        return m_intParam;
    }

    QString getStringValue()
    {
        return m_lstParam.at(0);
    }

    QStringList getLstParam()
    {
        return m_lstParam;
    }

    EThreadParam& operator = (const EThreadParam& param)
    {
        m_boolParam = param.m_boolParam;
        m_lstParam = param.m_lstParam;
        m_intParam = param.m_intParam;
        return *this;
    }

private:
    bool        m_boolParam;
    int         m_intParam;
    QStringList m_lstParam;
};

typedef EThreadReturnValue (*EThreadProc)(void* object, EThreadParam param);

class EThread : public QThread
{
    Q_OBJECT
public:
    friend EThread *EThreadCreateAndRun(EThreadProc proc, void *object, EThreadParam param, bool isRunImmediately);
    friend void EThreadRun(EThread *thread);
    friend int EThreadFree(EThread *thread);
public:
    explicit EThread(QObject *parent = 0);
    static void msleep(unsigned long);

    static bool stopAllThread();
    static bool isAppExitting();

public:
    bool isReadyForHandling();
    void setReadyForHandling(bool flag = true);
    EThreadReturnValue getThreadReturnValue();

public slots:

protected:
    void run();

private:
    EThreadProc             m_threadProc;
    EThreadParam            m_threadParam;
    void*                   m_threadObject;
    bool                    m_bIsReadyForHandling;
    QThread::Priority       m_threadPriority;
    static QList<EThread*>  m_threadList;
    static bool             m_bStopAllThread;
    static QMutex           m_mutex;
    EThreadReturnValue      m_nRetValue;
};

extern EThread *EThreadCreateAndRun(EThreadProc proc, void *object, EThreadParam param, bool isRunImmediately = true);
extern void EThreadRun(EThread *thread);
extern int EThreadFree(EThread *thread);

#endif // ETHREAD_H
