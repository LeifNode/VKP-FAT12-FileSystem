CCOMPILER=gcc
CFLAGS=-I include
CC= $(CCOMPILER) $(CFLAGS)
LINK_TARGET=filesystem.exe
OBJs= \
 sectors.o \
 fat.o \
 pbs.o \
 pfe.o \
 main.o

REBUILDABLES = $(OBJs) $(LINK_TARGET)

vpath %.c src
vpath %.h include

all: $(LINK_TARGET)


$(LINK_TARGET): $(OBJs)
	$(CC) -o $@ $^

main.o: main.c
	$(CC) -c $<

pbs.o: pbs.c
	$(CC) -c $<

pfe.o: pfe.c
	$(CC) -c $<

sectors.o: sectors.c
	$(CC) -c $<

fat.o: fat.c
	$(CC) -c $<

clean:
	rm $(REBUILDABLES)
	rm *~
	rm include/*~
	rm src/*~
	echo Clean completed

cleanobj:
	rm $(OBJs)

main.o: common.h pbs.h
pbs.o: common.h
pfe.o: common.h pfe.h
sectors.o: common.h sectors.h