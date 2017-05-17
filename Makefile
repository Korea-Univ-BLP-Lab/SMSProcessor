CFLAGS=-O2 -Wall
INSTALL_INC_PATH=test/include
INSTALL_LIB_PATH=test/lib
INSTALL_RES_PATH=test/rsc

all: library application

library:
	cd kulog; make CFLAGS="${CFLAGS}";
	cd reviser/src; make CFLAGS="${CFLAGS}";
	cd tagger/src; make CFLAGS="${CFLAGS}";


application:
	cd reviser/app; make CFLAGS="${CFLAGS}";
	cd tagger/app; make CFLAGS="${CFLAGS}";

clean:
	cd kulog; make clean;
	cd reviser; make clean;
	cd tagger; make clean;

#install:
#	cp include/* ${INSTALL_INC_PATH}
#	cp lib/* ${INSTALL_LIB_PATH}
#	cp -R rsc/* ${INSTALL_RES_PATH}

