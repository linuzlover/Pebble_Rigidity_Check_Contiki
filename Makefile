CONTIKI = ../..
LD_LIBRARY_PATH+=./
PORT=/dev/ttyUSB0

all: attilio_send attilio_receive

include $(CONTIKI)/Makefile.include

attilio_receive.$(TARGET): $(OBJECTDIR)/packages.o
#Dirty Hack for cooja
attilio_receive.elf: $(OBJECTDIR)/packages.o

attilio_send.$(TARGET): $(OBJECTDIR)/packages.o
