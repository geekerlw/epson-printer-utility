SUBDIRS = Languages

bindir=$(EPS_BIN_DIR)
bin_PROGRAMS =$(EPS_PREFIX)-utility

AM_CFLAGS=-g -w -c
AM_CXXFLAGS=-g -w 

if ARCH_CON
    DEFS+=-DX86_64
endif

DEFS += \
	-DQT_NO_DEBUG_OUTPUT

__EPS_PREFIX__utility_CPPFLAGS = \
	-I/opt/lsb/include/QtCore \
	-I/opt/lsb/include/QtCore \
	-I/opt/lsb/include/QtGui \
	-I/opt/lsb/include/QtGui \
   -I/opt/lsb/include/QtNetwork \
	-I/../EPUCommon	\
	-I/..	\
	-I/opt/lsb/include \
	-I../EPSCommonLib/	\
	-I../EPSCommonLib/escpr-lib/inc \
	-I../EPSCommonLib/epsmp/linux \
	-I../EPSCommonLib/epsmp


__EPS_PREFIX__utility_SOURCES = \
	Main.cpp \
	EPUMainWindow.cpp \
	EPUMainController.cpp \
	EPUView.cpp \
	EPUMaintenanceGuideView.cpp \
	EPUMainView.cpp \
	EPUCallbackObject.cpp \
	EPUOperationView.cpp \
	EPUGlobal.cpp	\
	EPUInkLevelView.cpp \
	../EPUCommon/EThread.cpp \
	EPUPrinterListDialog.cpp \
	EPUPrinterController.cpp \
	EPUPrinter.cpp	\
	moc_EPUMaintenanceGuideView.cpp	\
	moc_EPUMainView.cpp	\
	moc_EPUMainWindow.cpp	\
	moc_EPUOperationView.cpp	\
	moc_EPUPrinterListDialog.cpp	\
	moc_EPUView.cpp	\
	moc_EThread.cpp	\
	qrc_Resources.cpp

__EPS_PREFIX__utility_LDADD = \
	/opt/lsb/$(LIBARCH)/libQtCore.so \
	/opt/lsb/$(LIBARCH)/libQtGui.so \
	/opt/lsb/$(LIBARCH)/libpthread.so \
	/opt/lsb/$(LIBARCH)/libcupsimage.so \
	/opt/lsb/$(LIBARCH)/libcups.so \
   /opt/lsb/$(LIBARCH)/libQtNetwork.so \
	../EPSCommonLib/libEPSCommonLib.a
