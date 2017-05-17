.SUFFIX:.c .cpp .o
CC = g++
OSFLAGS = -D_REENTRANT -fPIC
CFLAGS = -O2
#CFLAGS = -g
AR= ar
LIB=../lib/
INCLUDE=../include/
BIN=./

.c.o:
	gcc -c $(CFLAGS) $(OSFLAGS) $<

.cpp.o: 
	$(CC) -c $(CFLAGS) $(OSFLAGS) $<


all: libkulog.a

libkulog.a: kulog.o
	$(AR) rcv $(LIB)$@ $?
	ranlib $(LIB)$@
	gcc $(CFLAGS) $(OSFLAGS) -shared -o $(LIB)libkulog.sl $?
	cp kulog.h $(INCLUDE)

test: kulog_test.o $(LIB)libkulog.a
	$(CC) $(CFLAGS) $(OSFLAGS) -o $(BIN)kulog_test $? 

clean: 
	rm -f *.o 
	rm -f $(LIB)libkulog.*
	rm -f $(INCLUDE)kulog.h
	rm -f $(BIN)kulog_test

