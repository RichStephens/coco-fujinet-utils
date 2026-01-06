# Makefile for Fuji utility disk image creation

CC=cmoc
AS=lwasm
CP=cp
ECHO=echo

FUJINET_LIB_DIR=../fujinet-lib-unified
LIBS=-L$(FUJINET_LIB_DIR)/r2r/coco -lfujinet.coco
CFLAGS=-Wno-const -I./src -I$(FUJINET_LIB_DIR)/include -I$(FUJINET_LIB_DIR)/bus/coco

all: clean time.bin drives.bin cfg.bin fninfo.bin appkeys.bin fujilogo.bin jsonread.bin fujiutil.dsk

fujiutil.dsk: time.bin drives.bin
	$(RM) fujiutil.dsk
	decb dskini fujiutil.dsk
	writecocofile fujiutil.dsk time.bin
	writecocofile fujiutil.dsk drives.bin	
	writecocofile fujiutil.dsk cfg.bin
	writecocofile fujiutil.dsk fninfo.bin
	writecocofile fujiutil.dsk appkeys.bin
	writecocofile fujiutil.dsk fujilogo.bin
	writecocofile fujiutil.dsk jsonread.bin
	decb copy -3 -a standard-tests.json fujiutil.dsk,TESTS.TXT
	cp fujiutil.dsk ~/tnfs

time.bin: time.o    
	$(CC) -o time.bin time.o $(LIBS)

drives.bin: drives.o    
	$(CC) -o drives.bin drives.o $(LIBS)

cfg.bin: cfg.o    
	$(CC) -o cfg.bin cfg.o $(LIBS)

fninfo.bin: fninfo.o    
	$(CC) -o fninfo.bin fninfo.o $(LIBS)

appkeys.bin: appkeys.o    
	$(CC) -o appkeys.bin appkeys.o $(LIBS)

fujilogo.bin: fujilogo.o	
	$(CC) -o fujilogo.bin fujilogo.o $(LIBS)

jsonread.bin: jsonread.o	
	$(CC) -o jsonread.bin jsonread.o $(LIBS)

time.o: src/time.c
	$(CC) $(CFLAGS) -c src/time.c

drives.o: src/drives.c
	$(CC) $(CFLAGS) -c src/drives.c		

cfg.o: src/cfg.c
	$(CC) $(CFLAGS) -c src/cfg.c

fninfo.o: src/fninfo.c
	$(CC) $(CFLAGS) -c src/fninfo.c

appkeys.o: src/appkeys.c
	$(CC) $(CFLAGS) -c src/appkeys.c

fujilogo.o: src/fujilogo.c
	$(CC) $(CFLAGS) -c src/fujilogo.c

jsonread.o: src/jsonread.c
	$(CC) $(CFLAGS) -c src/jsonread.c	

clean:
	$(RM) *.o
	$(RM) *.bin
	$(RM) fujiutil.dsk
