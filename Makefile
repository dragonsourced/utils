BIN  = ${HOME}/bin
SRC  = *.c
LDFLAGS = -s
EXE != echo ${SRC} | sed 's/\.c//g'

default: ${EXE}

clean:
	rm -f ${EXE}

install: ${EXE}
	mv ${EXE} ${BIN}
