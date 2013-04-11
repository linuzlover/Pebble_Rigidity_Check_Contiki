CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

CONTIKI = ../..
LD_LIBRARY_PATH+=./
PORT=/dev/ttyUSB0
DEPS=$(OBJECTDIR)/pebble_assign_set.o $(OBJECTDIR)/packages_comm.o $(OBJECTDIR)/pebble_functions.o

include $(CONTIKI)/Makefile.include


all: sender receiver

receiver.$(TARGET): $(DEPS)

sender.$(TARGET): $(DEPS)

#Dirty Hack for cooja
receiver.elf: $(DEPS)

sender.elf: $(DEPS)

