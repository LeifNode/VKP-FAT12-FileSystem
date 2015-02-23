CCOMPILER=gcc
CFLAGS=-I include -std=c99 -g
POSTCFLAGS=-lrt
CC= $(CCOMPILER)
SRC=$(wildcard ./src/*.c)
OBJ=$(SRC:.c=.o)
BINDIR="./bin/"

PBS_EXE="pbs"
PFE_EXE="pfe"
MOUNT_EXE="mount"
CD_EXE="cd"
SHELL_EXE="shell"

EXECUTABLES=$(PBS_EXE) $(PFE_EXE) $(MOUNT_EXE) $(CD_EXE) $(SHELL_EXE)

vpath %.c src
vpath %.h include

all: $(EXECUTABLES)


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(POSTCFLAGS)

$(PBS_EXE): $(OBJ) pbs.o
	$(CC) $(CFLAGS) -o $(BINDIR)$@ $^ $(POSTCFLAGS)

$(PFE_EXE): $(OBJ) pfe.o
	$(CC) $(CFLAGS) -o $(BINDIR)$@ $^ $(POSTCFLAGS)
	
$(MOUNT_EXE): $(OBJ) mount.o
	$(CC) $(CFLAGS) -o $(BINDIR)$@ $^ $(POSTCFLAGS)
	
$(CD_EXE): $(OBJ) cd.o
	$(CC) $(CFLAGS) -o $(BINDIR)$@ $^ $(POSTCFLAGS)

$(SHELL_EXE): $(OBJ) vkpshell.o
	$(CC) $(CFLAGS) -o $@ $^ $(POSTCFLAGS)

clean:
	rm $(OBJ)
	rm *.o
	rm ./*.exe
	rm *~
	rm include/*~
	rm src/*~
	rm bin/*

cleanobj:
	rm $(OBJ)
	rm *.o