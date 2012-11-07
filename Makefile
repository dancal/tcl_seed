#2008.01.21 created by dancal

CC = gcc
INC = -I./seedsrc
LIBS =
CFLAGS = -O3 -g $(INC) -fPIC

OBJS = tcl_seed.o seedsrc/SEED_KISA.o base64.o
SRCS = tcl_seed.c seedsrc/SEED_KISA.c base64.c

TARGET = libseed.so

all : $(TARGET)
$(TARGET) : $(OBJS)
	$(CC) -shared -Wl,-soname,$@ -o $@ $(OBJS) -lc -ltcl -lcrypt -L.

dep :
	gccmakedep $(INC) $(SRCS)

clean :
	rm -rf $(OBJS) $(TARGET) core
