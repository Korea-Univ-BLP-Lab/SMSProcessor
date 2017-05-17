CC=g++
CFLAGS=-g
OSFLAGS = -DHPUX_OS -D_REENTRANT -fPIC
LIB=../../lib/
BIN=../bin/
LOG=../bin/log/
.cpp.o:
	$(CC) -c $(CFLAGS) $(OSFLAGS) $<

all: reviser_test.o $(LIB)libReviser.a $(LIB)libhan.a $(LIB)libFST.a $(LIB)libkspacer.a $(LIB)libkulog.a
	$(CC) $(CFLAG) $(OSFLAGS) -o $(BIN)reviser_test $? 

clean:
	rm -f *.o
	rm -f $(BIN)reviser_test
	rm -f $(LOG)*
	
