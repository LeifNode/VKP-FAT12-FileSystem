CCOMPILER=gcc -g
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
CAT_EXE="cat"
RM_EXE="rm"
LS_EXE="ls"
DF_EXE="df"
PWD_EXE="pwd"
MKDIR_EXE="mkdir"
RMDIR_EXE="rmdir"
TOUCH_EXE="touch"
ECHO_EXE="echo"
SHELL_EXE="shell"

EXECUTABLES=$(PBS_EXE) $(PFE_EXE) $(MOUNT_EXE) $(CD_EXE) $(CAT_EXE) $(RM_EXE) $(LS_EXE) $(DF_EXE) $(PWD_EXE) $(MKDIR_EXE) $(RMDIR_EXE) $(TOUCH_EXE) $(ECHO_EXE) $(SHELL_EXE)

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

$(CAT_EXE): $(OBJ) cat.o
	$(CC) $(CFLAGS) -o $(BINDIR)$@ $^ $(POSTCFLAGS)

$(RM_EXE): $(OBJ) rm.o
	$(CC) $(CFLAGS) -o $(BINDIR)$@ $^ $(POSTCFLAGS)

$(LS_EXE): $(OBJ) ls.o
	$(CC) $(CFLAGS) -o $(BINDIR)$@ $^ $(POSTCFLAGS)

$(DF_EXE): $(OBJ) df.o
	$(CC) $(CFLAGS) -o $(BINDIR)$@ $^ $(POSTCFLAGS)

$(PWD_EXE): $(OBJ) pwd.o
	$(CC) $(CFLAGS) -o $(BINDIR)$@ $^ $(POSTCFLAGS)

$(RMDIR_EXE): $(OBJ) rmdir.o
	$(CC) $(CFLAGS) -o $(BINDIR)$@ $^ $(POSTCFLAGS)

$(MKDIR_EXE): $(OBJ) mkdir.o
	$(CC) $(CFLAGS) -o $(BINDIR)$@ $^ $(POSTCFLAGS)

$(TOUCH_EXE): $(OBJ) touch.o
	$(CC) $(CFLAGS) -o $(BINDIR)$@ $^ $(POSTCFLAGS)
	
$(ECHO_EXE): $(OBJ) echo.o
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
