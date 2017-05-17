CFLAGS = -O2 -Wall
all: library application

library:
	cd src; make CFLAGS="${CFLAGS}";

application:
	cd app; make CFLAGS="${CFLAGS}";

clean:
	cd src; make clean;
	cd app; make clean;

