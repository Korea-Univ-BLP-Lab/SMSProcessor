CC = g++
CFLAGS=-g 
OSFLAGS = -DHPUX_OS -D_REENTRANT -fPIC
RM= rm -f
CP= cp
AR= ar
LIB=../../lib/
INC=../../include/
PTLIB=./
FSTLIB=./

.cpp.o: 
	$(CC) -c $(CFLAGS) ${OSFLAGS} $<

all: libFST.a libhan.a libReviser.a _kspacer

libFST.a: $(FSTLIB)FST.o $(FSTLIB)MakeFST.o $(FSTLIB)GetFST.o $(FSTLIB)fileio.o
	$(AR) rcv $(LIB)$@ FST.o MakeFST.o GetFST.o fileio.o
	ranlib $(LIB)$@
	gcc $(CFLAGS) -shared -fPIC -o $(LIB)libFST.sl $?

libhan.a: $(PTLIB)han.o $(PTLIB)hcc2001.o $(PTLIB)ph.o
	$(AR) rcv $(LIB)$@ han.o hcc2001.o ph.o
	ranlib $(LIB)$@
	gcc $(CFLAGS) -shared -fPIC -o $(LIB)libhan.sl $?

libReviser.a : reviserAPI.o reviser.o HString.o AString.o 
	$(AR) rcv $(LIB)$@ $?
	ranlib $(LIB)$@
	gcc $(CFLAGS) -shared -fPIC -o $(LIB)libReviser.sl $?
	cp reviserAPI.h $(INC)

_kspacer :
	cd kspacer ; make;

clean: 	kclean rmobj rmlib rminc
kclean:
	cd kspacer ; make clean;
rmobj:
	${RM} *.o 
rmlib:
	${RM} $(LIB)libFST.*
	${RM} $(LIB)libhan.*
	${RM} $(LIB)libReviser.*
rminc:
	${RM} $(INC)reviserAPI.h
