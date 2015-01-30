CCOMPILER=gcc
CFLAGS=-I include -std=c99
CC= $(CCOMPILER)
SRC=$(wildcard ./src/*.c)
OBJ=$(SRC:.c=.o)

PBS_EXE="pbs.exe"
PFE_EXE="pfe.exe"

EXECUTABLES=$(PBS_EXE) $(PFE_EXE)

vpath %.c src
vpath %.h include

all: $(EXECUTABLES)


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(PBS_EXE): $(OBJ) pbs.o
	$(CC) $(CFLAGS) -o $@ $^

$(PFE_EXE): $(OBJ) pfe.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm $(OBJ)
	rm *.o
	rm ./*.exe
	rm *~
	rm include/*~
	rm src/*~

cleanobj:
	rm $(OBJ)
	rm *.o