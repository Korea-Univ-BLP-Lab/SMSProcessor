CC = g++
AR = ar
#For Optimization
OSFLAGS=-DHPUX_OS -D_REENTRANT -fPIC

#For debugging
#CFLAGS = -g

RM= rm -f
CP= cp

LIB= ../../../lib/

.c.o:
	gcc -c $(CFLAGS) $(OSFLAGS) $<
.cpp.o:
	$(CC) -c $(CFLAGS) $(OSFLAGS) $<

all: libkspacer.a 

libkspacer.a: probability_io_FST.o spacer_FST.o statistical_model_FST.o viterbi.o get_probability_FST.o common.o hsplit.o str_prob_FST.o
	$(AR) rcv $(LIB)$@ $?
	ranlib $(LIB)$@
	gcc $(CFLAGS) -shared -fPIC -o $(LIB)libkspacer.sl $?

clean:  rmobj rmlib 
rmobj:
	${RM} *.o
rmlib:
	${RM} $(LIB)libkspacer.*

