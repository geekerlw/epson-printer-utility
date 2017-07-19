QT += network
SOURCES += \
    Main.cpp \
    EPUMainWindow.cpp \
    EPUMainController.cpp \
    EPUCommonUtility.cpp \
    EPUView.cpp \
    EPUMaintenanceGuideView.cpp \
    EPUMainView.cpp \
    EPUMonitorStatusView.cpp \
    EPUNozzleCheckResultView.cpp \
    EPUCallbackObject.cpp \
    EPUHeadCleaningView.cpp \
    EPUOperationView.cpp \
    EPUInkLevelView.cpp \
    ../EPUCommon/EThread.cpp \
    EPUPrinterListDialog.cpp \
    EPUPrinterController.cpp \
    EPUPrinter.cpp

HEADERS += \
    EPUMainWindow.h \
    EPUMainController.h \
    EPUCommonUtility.h \
    EPUView.h \
    EPUMaintenanceGuideView.h \
    EPUMainView.h \
    EPUMonitorStatusView.h \
    EPUNozzleCheckResultView.h \
    EPUCallbackObject.h \
    EPUHeadCleaningView.h \
    EPUOperationView.h \
    EPUInkLevelView.h \
    ../EPUCommon/EPUCommonDef.h \
    ../EPUCommon/EThread.h \
    EPUPrinterListDialog.h \
    EPUPrinterController.h \
    EPUPrinter.h \
    ../EPSCommonLib/reply-parser/inc/epson-ijpl-rp-def.h \
    ../EPSCommonLib/common/inc/epson-escpr-media.h \
    ../EPSCommonLib/common/inc/epson-ijpl-dbg.h \
    ../EPSCommonLib/common/inc/epson-ijpl-err.h \
    ../EPSCommonLib/common/inc/epson-ijpl-status.h \
    ../EPSCommonLib/common/inc/epson-ijpl-typedefs.h \
    ../EPSCommonLib/communicator/inc/epson-ijpl-comm-def.h \
    ../EPSCommonLib/communicator/inc/epson-cbt.h \
    ../EPSCommonLib/communicator/inc/epson-ijpl-comm-api.h \
    ../EPSCommonLib/communicator/inc/epson-ijpl-comm-pvt.h \
    ../EPSCommonLib/communicator/inc/epson-ijpl-comm-utl.h \
    ../EPSCommonLib/communicator/inc/epson-net-lpr.h \
    ../EPSCommonLib/communicator/inc/epson-net-snmp.h \
    ../EPSCommonLib/communicator/inc/epson-net-upnp.h \
    ../EPSCommonLib/communicator/inc/epson-net-upnp-media.h \
    ../EPSCommonLib/communicator/inc/epson-protocol.h \
    ../EPSCommonLib/communicator/inc/epson-usb.h

RESOURCES += \
    Resources.qrc

INCLUDEPATH += ../EPSCommonLib ../EPSCommonLib/common/inc ../EPSCommonLib/reply-parser/inc  ../EPSCommonLib/communicator/inc

LIBS += -L../EPSCommonLib -lEPSCommonLib -lcups
