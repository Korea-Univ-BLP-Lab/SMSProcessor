CFLAGS=-Wall -O2
all: library application

library:
	cd src; make CFLAGS="${CFLAGS}";

application:
	cp src/reviserAPI.h ../include
	cd app; make  CFLAGS="${CFLAGS}";

clean:
	cd src; make clean;
	cd app; make clean;

