BIN  = ${HOME}/bin
SRC  = *.c

EXE != echo ${SRC} | sed 's/\.c//g'

CFLAGS += -Wall -std=c99 -pedantic

default: ${EXE}

clean:
	rm -f ${EXE}

install: ${EXE}
	mv ${EXE} ${BIN}
