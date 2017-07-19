ESCPR_LIB_DIR	=	../../escpr-lib

export RM		=	/bin/rm -f
export CC		=	g++
export CFLAGS	=	-Wall -fpermissive $(DEFINES) $(INCLUDES)

################################################################
# Protocol set
################################################################
DEFINES 		+=	-DGCOMSW_PRT_USE_LPR
#DEFINES 		+=	-DGCOMSW_PRT_USE_RAW

################################################################
# Extend function
################################################################
#DEFINES 		+=	-DGCOMSW_CMD_ESCPAGE
DEFINES 		+=	-DGCOMSW_EL_CDLABEL
DEFINES 		+=	-DGCOMSW_EF_MAINTE

################################################################
# Use Async job process
DEFINES 		+=	-DGCOMSW_ASYNCJOB_SUPPORT
# Use raw file print
DEFINES 	    +=	-DGCOMSW_RAWFILE_SUPPORT

#JPG_DEFINES = -DGCOMSW_JPEG_SUPPORT


ifdef JPG_DEFINES
DEFINES 	   +=	$(JPG_DEFINES)
LIB_JPEG 		=	../jpeg-8d/.libs/libjpeg.a
LIB_JPEG_H  	=	-I../jpeg-8d/
LIB_JPEG_OBJ 	=	../jpeg-8d/transupp.o
endif


INCLUDES		=	-I$(ESCPR_LIB_DIR)/inc -I../linux -I../ $(LIB_JPEG_H)

LIB_OBJS 		=	$(ESCPR_LIB_DIR)/src/epson-cbt.o			\
					$(ESCPR_LIB_DIR)/src/epson-escpr-api.o		\
					$(ESCPR_LIB_DIR)/src/epson-escpr-services.o	\
					$(ESCPR_LIB_DIR)/src/epson-escpr-mem.o	\
					$(ESCPR_LIB_DIR)/src/epson-net-lpr.o	\
					$(ESCPR_LIB_DIR)/src/epson-net-raw.o	\
					$(ESCPR_LIB_DIR)/src/epson-net-snmp.o	\
					$(ESCPR_LIB_DIR)/src/epson-protocol.o	\
					$(ESCPR_LIB_DIR)/src/epson-escpage-color.o	 \
					$(ESCPR_LIB_DIR)/src/epson-escpage-comp.o		\
					$(ESCPR_LIB_DIR)/src/epson-escpage-mono.o		\
					$(ESCPR_LIB_DIR)/src/epson-escpage.o			\
					$(ESCPR_LIB_DIR)/src/epson-layout.o	    \
					$(ESCPR_LIB_DIR)/src/epson-usb.o	    \


OBJS			=	../demo_print.o             \
                ../demo_printer.o           \
				../demo_print_uni.o         \
				../demo_status.o			\
				../demo_supported_media.o	\
				../demo_maintenance.o		\
				../demo_interactive.o		\
				../epsmp-bmp.o				\
				../epsmp-common-functions.o \
				../epsmp-file-io.o			\
				../epsmp-jpg.o				\
				../epsmp-rawimg.o			\
				../epsmp-make-img.o         \
				../epsmp-image.o			\
				../epsmp-print-io.o         \
				../epsmp-prn-parse.o        \
				../epsmp-scale.o            \
				epsmp-linux-portal-io.o     \
				linux_cmn.o	\
				linux_net.o	\
				XAsyncSocket.o

TARGET			=	escpr_smp

all: $(OBJS) LIB_ALL
	$(CC) $(CFLAGS) -pthread -lm ../epson-escpr-sample-main.c $(OBJS) $(LIB_OBJS) $(LIB_JPEG) $(LIB_JPEG_OBJ) -o $(TARGET)

LIB_ALL:
	$(MAKE) -C $(ESCPR_LIB_DIR)/src

.PHONY: clean

clean:
	$(RM) $(OBJS) $(TARGET)
	$(MAKE) -C $(ESCPR_LIB_DIR)/src clean
