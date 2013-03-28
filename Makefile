CONTIKI = ../..

all: attilio_send.c attilio_receive.c packages.c

attilio_send: packages.c

attilio_receive: packages.c

include $(CONTIKI)/Makefile.include
