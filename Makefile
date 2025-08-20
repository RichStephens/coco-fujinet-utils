# Makefile for Fuji utility disk image creation

CC=cmoc
AS=lwasm
CP=cp
ECHO=echo

FUJINET_LIB_DIR=../fujinet-lib
LIBS=-L$(FUJINET_LIB_DIR)/build -lfujinet.lib.coco
CFLAGS=-I./src -I$(FUJINET_LIB_DIR) -I$(FUJINET_LIB_DIR)/coco/src/include

all: clean time.bin drives.bin cfg.bin fninfo.bin fujiutil.dsk

fujiutil.dsk: time.bin drives.bin
	$(RM) fujiutil.dsk
	decb dskini fujiutil.dsk
	writecocofile fujiutil.dsk time.bin
	writecocofile fujiutil.dsk drives.bin	
	writecocofile fujiutil.dsk cfg.bin
	writecocofile fujiutil.dsk fninfo.bin

time.bin: time.o    
	$(CC) -o time.bin time.o $(LIBS)

drives.bin: drives.o    
	$(CC) -o drives.bin drives.o $(LIBS)

cfg.bin: cfg.o    
	$(CC) -o cfg.bin cfg.o $(LIBS)

fninfo.bin: fninfo.o    
	$(CC) -o fninfo.bin fninfo.o $(LIBS)

time.o: src/time.c
	$(CC) $(CFLAGS) -c src/time.c

drives.o: src/drives.c
	$(CC) $(CFLAGS) -c src/drives.c		

cfg.o: src/cfg.c
	$(CC) $(CFLAGS) -c src/cfg.c

fninfo.o: src/fninfo.c
	$(CC) $(CFLAGS) -c src/fninfo.c

clean:
	$(RM) *.o
	$(RM) *.bin
	$(RM) fujiutil.dsk
