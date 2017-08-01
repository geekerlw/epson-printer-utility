#ifndef EPSCOMMONLIBDEFINE_H
#define EPSCOMMONLIBDEFINE_H

#include <QVector>

#define MAX_PRINTER_MODEL_NAME_LEN 64

typedef struct _EPSPrinter
{
    qint8 modelName[MAX_PRINTER_MODEL_NAME_LEN];
    qint8 printerId;
    _EPSPrinter()
    {
        memset(modelName, 0x00, sizeof(modelName));
        printerId = -1;
    }
} EPSPrinter;

#endif // EPSCOMMONLIBDEFINE_H
