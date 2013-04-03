DEFINES=PROJECT_CONF_H=\"project-conf.h\"

CONTIKI = ../..
LD_LIBRARY_PATH+=./
PORT=/dev/ttyUSB0
DEPS=$(OBJECTDIR)/packages_comm.o $(OBJECTDIR)/pebble_functions.o

include $(CONTIKI)/Makefile.include


all: attilio_send attilio_receive

attilio_receive.$(TARGET): $(DEPS)

attilio_send.$(TARGET): $(DEPS)

#Dirty Hack for cooja
attilio_receive.elf: $(DEPS)

attilio_send.elf: $(DEPS)

