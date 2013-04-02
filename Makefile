CONTIKI = ../..
LD_LIBRARY_PATH+=./
PORT=/dev/ttyUSB0
DEPS=$(OBJECTDIR)/packages.o $(OBJECTDIR)/pebble_functions.o
all: attilio_send attilio_receive

include $(CONTIKI)/Makefile.include

attilio_receive.$(TARGET): $(DEPS)

attilio_send.$(TARGET): $(DEPS)

#Dirty Hack for cooja
attilio_receive.elf: $(DEPS)

attilio_send.elf: $(DEPS)

