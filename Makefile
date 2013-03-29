CONTIKI = ../..
LD_LIBRARY_PATH+=./

all: attilio_send attilio_receive

include $(CONTIKI)/Makefile.include

attilio_receive.$(TARGET): $(OBJECTDIR)/packages.o

attilio_send.$(TARGET): $(OBJECTDIR)/packages.o
