CC = g++
CFLAGS = -O2
OSFLAGS = -DHPUX_OS -D_REENTRANT
RM= /bin/rm -f
BIN= ../bin/

.cpp.o: 
	$(CC) -c $(CFLAGS) $(OSFLAGS) $<


all:	MakeExistDic

MakeExistDic:	MakeRule.o entry2FST.o fileio.o MakeFST.o GetFST.o FST.o
	$(CC) $(CFLAGS) -o $(BIN)$@ $?

clean: 	rmobj rmbin

rmobj:
	${RM} *.o 
rmbin:
	${RM} $(BIN)MakeExistDic
