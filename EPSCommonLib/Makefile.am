noinst_LIBRARIES = libEPSCommonLib.a


DEFS=-DINSTALL_DIR=\"$(EPS_INSTALL_DIR)/\"

################################################################
# Protocol set
################################################################


DEFS += \
  		-DQT_NO_DEBUG_OUTPUT	\
  		-DGCOMSW_PRT_USE_LPR	\
  		-DGCOMSW_CMD_ESCPAGE	\
  		-DGCOMSW_PRT_USE_RAW	\
  		-DGCOMSW_EL_CDLABEL	\
  		-DGCOMSW_EF_MAINTE

AM_CFLAGS=-w -c \
	-I./escpr-lib/inc \
	-I./epsmp/linux \
	-I./epsmp	\
	-I./..
AM_CXXFLAGS=-w -c

AM_CPPFLAGS = \
	-I./escpr-lib/inc \
	-I./epsmp/linux \
	-I./epsmp	\
	-I./..	\
	-I/opt/lsb/include/QtCore	\
	-I/opt/lsb/include

libEPSCommonLib_a_LIBADD = \
	/opt/lsb/$(LIBARCH)/libQtCore.so \
	/opt/lsb/$(LIBARCH)/libQtGui.so \
	/opt/lsb/$(LIBARCH)/libpthread.so \
	/opt/lsb/$(LIBARCH)/libcupsimage.so \
	/opt/lsb/$(LIBARCH)/libcups.so \
   /opt/lsb/$(LIBARCH)/libQtNetwork.so
	
libEPSCommonLib_a_SOURCES = \
	escpr-lib/src/epson-cbt.c	\
	escpr-lib/src/epson-escpage.c	\
	escpr-lib/src/epson-escpage-color.c	\
	escpr-lib/src/epson-escpage-comp.c	\
	escpr-lib/src/epson-escpage-mono.c	\
	escpr-lib/src/epson-escpr-api.c	\
	escpr-lib/src/epson-escpr-closed.c	\
	escpr-lib/src/epson-escpr-dbg.c	\
	escpr-lib/src/epson-escpr-mem.c	\
	escpr-lib/src/epson-escpr-services.c	\
	escpr-lib/src/epson-layout.c	\
	escpr-lib/src/epson-net-lpr.c	\
	escpr-lib/src/epson-net-raw.c	\
	escpr-lib/src/epson-net-snmp.c	\
	escpr-lib/src/epson-protocol.c	\
	escpr-lib/src/epson-usb.c	\
	escpr-lib/src/epson-wifi.c	\
	epsmp/linux/epsmp-linux-portal-io.c \
	epsmp/linux/XAsyncSocket.c \
	epsmp/linux/linux_cmn.c	\
	epsmp/linux/linux_net.c	\
	epsmp/epsmp-scale.c	\
	epsmp/epsmp-bmp.c	\
	epsmp/epsmp-file-io.c	\
	epsmp/epsmp-image.c	\
	epsmp/epsmp-jpg.c	\
	epsmp/epsmp-print-io.c	\
	epsmp/epsmp-prn-parse.c	\
	epsmp/epsmp-rawimg.c	\
	EPSCommunicator.cpp \
	../BackendLib/backend_api.c\
	../BackendLib/ekpcom.c\
	EPSReplyParser.cpp \
	escpr-lib/src/err.c	\
	EPSPrinterController.cpp
