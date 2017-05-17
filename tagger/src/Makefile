# For gcc
CC = g++
CFLAGS=-g
BIN = ../bin/
LIB = ../../lib/
INCLUDE = ../../include/
RM= /bin/rm -f
OSFLAGS= -D_REENTRANT -fPIC

.cpp.o: 
	$(CC) -c $(CFLAGS) $(OSFLAGS) $<

all:	libKoma.a libHanTag.a 

libKoma.a:	Koma.o KomaRsc.o core.o hcc98.o ph.o predict.o FST.o KomaAPI.o
	cp $(INCLUDE)kulog.h .
	ar -r $(LIB)$@ Koma.o KomaRsc.o core.o hcc98.o ph.o predict.o FST.o KomaAPI.o
	gcc $(CFLAGS) -shared -fPIC -o $(LIB)libKoma.sl $?

libHanTag.a:	Tagger.o TaggerRsc.o Taganal.o FST.o HanTagAPI.o
	ar -r $(LIB)$@ Tagger.o TaggerRsc.o Taganal.o FST.o HanTagAPI.o
	gcc $(CFLAGS) -shared -fPIC -o $(LIB)libHanTag.sl $?
	cp *API.h $(INCLUDE)
	cp nlp.def $(INCLUDE)

clean:
	rm -f *.o 
	rm -f $(LIB)libKoma.*
	rm -f $(LIB)libHanTag.*
	rm -f $(INCLUDE)HanTagAPI.h
	rm -f $(INCLUDE)KomaAPI.h
	rm -f $(INCLUDE)nlp.def

