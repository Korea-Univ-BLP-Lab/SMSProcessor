# For gcc
CC = g++
BIN = ../bin/
LIB = ../../lib/
LOG = ../bin/log/
CFLAGS=-g
OSFLAGS = -DHPUX_OS -D_REENTRANT -fPIC
RM= /bin/rm -f

.cpp.o: 
	$(CC) -c $(CFLAGS) $(OSFLAGS) $<

all:	 KUTagger

KUTagger:	KUTagger.o
	$(CC) ${CFLAGS} ${OSFLAGS} -o $(BIN)$@ KUTagger.o $(LIB)libHanTag.a $(LIB)libKoma.a $(LIB)libkulog.a -lm
simple:	simple_KUTagger.o
	$(CC) ${CFLAGS} ${OSFLAGS} -o $(BIN)$@ simple_KUTagger.o $(LIB)libHanTag.a $(LIB)libKoma.a $(LIB)libkulog.a -lm

clean:
	rm -f *.o
	rm -f $(BIN)KUTagger
	rm -f $(LOG)*

