# For gcc
CC = g++
CFLAGS = -O2
OSFLAGS = -DHPUX_OS -D_REENTRANT

RM= /bin/rm -f

.cpp.o: 
	$(CC) -c $(CFLAGS) $(OSFLAGS) $<

all:	MakeDic

MakeDic:	FST.o MakeFSTMain.o MakeDic.o
	$(CC) $(CFLAGS)  -o ../bin/$@ FST.o MakeFSTMain.o MakeDic.o -lm

clean:
	rm -f *.o *.a
