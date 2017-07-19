ESCPR_LIB_DIR	=	../../escpr-lib

export RM		=	/bin/rm -f
export CC		=	g++
export CFLAGS	=	-Wall -fpermissive $(DEFINES) $(INCLUDES) 

################################################################
# Protocol set
################################################################
DEFINES 		+=	-DGCOMSW_PRT_USE_LPR
#DEFINES 		+=	-DGCOMSW_PRT_USE_RAW


INCLUDES		=	-I$(ESCPR_LIB_DIR)/inc -I../ -I../linux

LIB_OBJS 		=	$(ESCPR_LIB_DIR)/src/epson-cbt.o			\
					$(ESCPR_LIB_DIR)/src/epson-escpr-api.o		\
					$(ESCPR_LIB_DIR)/src/epson-escpr-services.o	\
					$(ESCPR_LIB_DIR)/src/epson-escpr-mem.o	    \
					$(ESCPR_LIB_DIR)/src/epson-net-lpr.o	    \
					$(ESCPR_LIB_DIR)/src/epson-net-raw.o	    \
					$(ESCPR_LIB_DIR)/src/epson-net-snmp.o	    \
					$(ESCPR_LIB_DIR)/src/epson-protocol.o	    \
					$(ESCPR_LIB_DIR)/src/epson-usb.o	        \
					$(ESCPR_LIB_DIR)/src/epson-escpage-color.o	\
					$(ESCPR_LIB_DIR)/src/epson-escpage-comp.o	\
					$(ESCPR_LIB_DIR)/src/epson-escpage-mono.o	\
					$(ESCPR_LIB_DIR)/src/epson-escpage.o		\
					$(ESCPR_LIB_DIR)/src/epson-layout.o	    	\


OBJS			=	../demo_printer.o           \
                    ../demo_simple_print.o     \
					../demo_simple_print_uni.o  \
					../epsmp-bmp.o				\
					../epsmp-common-functions.o \
					../epsmp-file-io.o			\
					../epsmp-image.o			\
					../epsmp-print-io.o         \
					epsmp-linux-portal-io.o     \
					linux_cmn.o	                \
					linux_net.o	                \
					XAsyncSocket.o

TARGET			=	escpr_simple_smp

all: $(OBJS) LIB_ALL
	$(CC) $(CFLAGS) -pthread -lm ../epson-escpr-simple-sample-main.c $(OBJS) $(LIB_OBJS) -o $(TARGET)

LIB_ALL:
	$(MAKE) -C $(ESCPR_LIB_DIR)/src

.PHONY: clean

clean:
	$(RM) $(OBJS) $(TARGET)
	$(MAKE) -C $(ESCPR_LIB_DIR)/src clean
