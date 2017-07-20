QT += network core gui widgets

TARGET = epson-printer-utility
TEMPLATE = app

SOURCES += \
    EPUCallbackObject.cpp \
	EPUGlobal.cpp	\
	EPUInkLevelView.cpp	\
	EPUMainController.cpp	\
    EPUMaintenanceGuideView.cpp \
    EPUMainView.cpp \
    EPUMainWindow.cpp \
    EPUOperationView.cpp \
    EPUPrinter.cpp \
    EPUPrinterController.cpp \
    EPUPrinterListDialog.cpp \
    EPUView.cpp \
    Main.cpp \
    ../EPUCommon/EThread.cpp

HEADERS += \
	EPUCallbackObject.h	\
	EPUGlobal.h	\
    EPUInkLevelView.h \
    EPUMainController.h \
    EPUMaintenanceGuideView.h \
    EPUMainView.h \
    EPUMainWindow.h \
    EPUOperationView.h \
	EPUPrinter.h	\
    EPUPrinterController.h \
    EPUPrinterListDialog.h \
    EPUView.h \
    ../EPUCommon/EPUCommonDef.h \
    ../EPUCommon/EThread.h \
    ../EPSCommonLib/escpr-lib/inc/epson-cbt.h \
	../EPSCommonLib/escpr-lib/inc/epson-escpage.h	\
	../EPSCommonLib/escpr-lib/inc/epson-escpage-color.h \
	../EPSCommonLib/escpr-lib/inc/epson-escpage-comp.h	\
	../EPSCommonLib/escpr-lib/inc/epson-escpage-mono.h \
	../EPSCommonLib/escpr-lib/inc/epson-escpr-api.h \	
	../EPSCommonLib/escpr-lib/inc/epson-escpr-closed.h \
	../EPSCommonLib/escpr-lib/inc/epson-escpr-dbg.h \
	../EPSCommonLib/escpr-lib/inc/epson-escpr-def.h \
	../EPSCommonLib/escpr-lib/inc/epson-escpr-err.h \
	../EPSCommonLib/escpr-lib/inc/epson-escpr-media.h \
	../EPSCommonLib/escpr-lib/inc/epson-escpr-mem.h	\
	../EPSCommonLib/escpr-lib/inc/epson-escpr-pvt.h \
	../EPSCommonLib/escpr-lib/inc/epson-escpr-services.h \
	../EPSCommonLib/escpr-lib/inc/epson-layout.h \
	../EPSCommonLib/escpr-lib/inc/epson-net-lpr.h \
	../EPSCommonLib/escpr-lib/inc/epson-net-snmp.h \
	../EPSCommonLib/escpr-lib/inc/epson-protocol.h \
	../EPSCommonLib/escpr-lib/inc/epson-usb.h

RESOURCES += \
    Resources.qrc

INCLUDEPATH += ../EPSCommonLib ../EPSCommonLib/escpr-lib/inc ../EPSCommonLib/epsmp/linux  ../EPSCommonLib/epsmp

LIBS += -L../EPSCommonLib -lEPSCommonLib -lcups
