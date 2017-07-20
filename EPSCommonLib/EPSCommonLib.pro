#-------------------------------------------------
#
# Project created by QtCreator 2013-10-10T10:18:35
#
#-------------------------------------------------

QT       -= gui
QMAKE_CC = g++
QMAKE_C++ = g++

TARGET = EPSCommonLib
TEMPLATE = lib
CONFIG += staticlib

INCLUDEPATH += escpr-lib/inc epsmp/linux epsmp

SOURCES += \
	../BackendLib/backend_api.c	\
	../BackendLib/ekpcom.c \
	EPSCommonLib.cpp	\
    EPSCommunicator.cpp \
    EPSPrinterController.cpp \
    EPSReplyParser.cpp \
    ../EPUCommon/EThread.cpp \
	epsmp/epsmp-bmp.c	\
	epsmp/epsmp-file-io.c	\
	epsmp/epsmp-image.c	\
	epsmp/epsmp-jpg.c	\
	epsmp/epsmp-print-io.c \
	epsmp/epsmp-prn-parse.c \
	epsmp/epsmp-rawimg.c \
	epsmp/epsmp-scale.c \
    epsmp/linux/epsmp-linux-portal-io.c \
    epsmp/linux/linux_cmn.c \
    epsmp/linux/linux_net.c \
    epsmp/linux/XAsyncSocket.cpp \
    escpr-lib/src/epson-cbt.c \
    escpr-lib/src/epson-escpage.c \
    escpr-lib/src/epson-escpage-color.c \
    escpr-lib/src/epson-escpage-comp.c \
    escpr-lib/src/epson-escpage-mono.c \
    escpr-lib/src/epson-escpr-api.c \
    escpr-lib/src/epson-escpr-closed.c \
    escpr-lib/src/epson-escpr-dbg.c \
    escpr-lib/src/epson-escpr-mem.c \
    escpr-lib/src/epson-escpr-services.c \
    escpr-lib/src/epson-layout.c \
    escpr-lib/src/epson-net-lpr.c \
    escpr-lib/src/epson-net-raw.c \
    escpr-lib/src/epson-net-snmp.c \
    escpr-lib/src/epson-protocol.c \
    escpr-lib/src/epson-usb.c \
    escpr-lib/src/epson-wifi.c \
	escpr-lib/src/err.c

HEADERS += \
	../BackendLib/backend_api.h \
	../BackendLib/ekpcom.h \
    epsmp/linux/epsmp-linux-portal-io.h \
    epsmp/linux/linux_cmn.h \
    epsmp/linux/linux_net.h \
	epsmp/linux/XAsyncSocket.h \
    epsmp/epsmp-bmp.h \
    epsmp/epsmp-file-io.h \
    epsmp/epsmp-image.h \
    epsmp/epsmp-jpg.h \
    epsmp/epsmp-print-io.h \
    epsmp/epsmp-prn-parse.h \
    epsmp/epsmp-rawimg.h \
    epsmp/epsmp-scale.h \
    epsmp/included_headers.h \
    escpr-lib/inc/epson-cbt.h \
    escpr-lib/inc/epson-escpage.h \
    escpr-lib/inc/epson-escpage-color.h \
    escpr-lib/inc/epson-escpage-comp.h \
    escpr-lib/inc/epson-escpage-mono.h \
    escpr-lib/inc/epson-escpr-api.h \
    escpr-lib/inc/epson-escpr-closed.h \
    escpr-lib/inc/epson-escpr-dbg.h \
    escpr-lib/inc/epson-escpr-def.h \
    escpr-lib/inc/epson-escpr-err.h \
    escpr-lib/inc/epson-escpr-media.h \
    escpr-lib/inc/epson-escpr-mem.h \
    escpr-lib/inc/epson-escpr-pvt.h \
    escpr-lib/inc/epson-escpr-services.h \
    escpr-lib/inc/epson-layout.h \
    escpr-lib/inc/epson-net-lpr.h \
    escpr-lib/inc/epson-net-raw.h \
    escpr-lib/inc/epson-net-snmp.h \
    escpr-lib/inc/epson-protocol.h \
    escpr-lib/inc/epson-typedefs.h \
    escpr-lib/inc/epson-usb.h \
    escpr-lib/inc/epson-wifi.h  \
	escpr-lib/inc/err.h	\
	EPSCommonLib.h	\
    EPSCommonLibDefine.h \
    EPSCommunicator.h \
    EPSPrinterController.h \
    EPSReplyParser.h \
	err.h
    ../EPUCommon/EThread.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

OTHER_FILES +=
