CC       = gcc

################################################################
# Protocol set
################################################################
DEFINES 		+=	-DGCOMSW_PRT_USE_LPR
DEFINES 		+=	-DGCOMSW_PRT_USE_RAW
#DEFINES 		+=	-DGCOMSW_CMD_ESCPAGE
DEFINES 		+=	-DGCOMSW_EL_CDLABEL
DEFINES 		+=	-DGCOMSW_EF_MAINTE
 
INCLUDES = -I../inc
CFLAGS   += $(DEFINES) $(INCLUDES)

OBJS	 = epson-escpr-mem.o        \
           epson-escpr-services.o   \
           epson-net-lpr.o          \
           epson-net-raw.o          \
           epson-net-snmp.o         \
           epson-cbt.o              \
           epson-usb.o              \
           epson-protocol.o         \
           epson-escpage-color.o	\
		   epson-escpage-comp.o		\
		   epson-escpage-mono.o		\
		   epson-escpage.o			\
				epson-escpr-closed.o	\
           epson-layout.o           \
           epson-escpr-api.o        \

all: $(OBJS)

.PHONY: clean

clean:
	$(RM) $(OBJS)
