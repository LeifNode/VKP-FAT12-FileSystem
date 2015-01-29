CCOMPILER=gcc
CFLAGS=-I include -std=c99
CC= $(CCOMPILER)
SRC=$(wildcard ./src/*.c)
OBJ=$(SRC:.c=.o)

PBS_EXE="pbs.exe"
PFS_EXE="pfs.exe"

EXECUTABLES=$(PBS_EXE) $(PFS_EXE)

vpath %.c src
vpath %.h include

all: $(PBS_EXE)


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(PBS_EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^


clean:
	rm $(OBJ)
	rm ./*.exe
	rm *~
	rm include/*~
	rm src/*~

cleanobj:
	rm $(OBJ)
