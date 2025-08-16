# Makefile for Fuji utility disk image creation

CC=cmoc
AS=lwasm
CP=cp
ECHO=echo

CFLAGS=-I./src

all: clean time.bin drives.bin cfg.bin fujiutil.dsk

fujiutil.dsk: time.bin drives.bin
	$(RM) fujiutil.dsk
	decb dskini fujiutil.dsk
	writecocofile fujiutil.dsk time.bin
	writecocofile fujiutil.dsk drives.bin	
	writecocofile fujiutil.dsk cfg.bin

time.bin: time.o dwwrite.o dwread.o fuji_get_response.o fuji_ready.o
	$(CC) -o time.bin time.o dwwrite.o dwread.o fuji_get_response.o fuji_ready.o

drives.bin: drives.o dwwrite.o dwread.o fuji_get_response.o fuji_ready.o
	$(CC) -o drives.bin drives.o dwwrite.o dwread.o fuji_get_response.o fuji_ready.o

cfg.bin: cfg.o dwwrite.o dwread.o fuji_get_response.o fuji_ready.o
	$(CC) -o cfg.bin cfg.o dwwrite.o dwread.o fuji_get_response.o fuji_ready.o	

time.o: src/time.c
	$(CC) $(CFLAGS) -c src/time.c

drives.o: src/drives.c
	$(CC) $(CFLAGS) -c src/drives.c		

cfg.o: src/cfg.c
	$(CC) $(CFLAGS) -c src/cfg.c

dwwrite.o: src/dwwrite.c
	$(CC) $(CFLAGS) -c src/dwwrite.c

dwread.o: src/dwread.c
	$(CC) $(CFLAGS) -c src/dwread.c

fuji_get_response.o: src/fuji_get_response.c
	$(CC) $(CFLAGS) -c src/fuji_get_response.c

fuji_ready.o: src/fuji_ready.c
	$(CC) $(CFLAGS) -c src/fuji_ready.c	

clean:
	$(RM) *.o
	$(RM) *.bin
	$(RM) fujiutil.dsk
