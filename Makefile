#Makefile for Fuji utility disk image creation

CC=cmoc
AS=lwasm
CP=cp
ECHO=echo

FUJINET_LIB_DIR=../fujinet-lib
LIBS=-L$(FUJINET_LIB_DIR)/build -l:fujinet.lib.coco
CFLAGS=-Wno-const -I./src -I$(FUJINET_LIB_DIR) -I$(FUJINET_LIB_DIR)/coco/src/include

all: clean time.bin drives.bin cfg.bin fninfo.bin appkeys.bin fujilogo.bin clock.bin hosts.bin guid.bin fujiutil.dsk

fujiutil.dsk: time.bin drives.bin
	$(RM) fujiutil.dsk
	decb dskini fujiutil.dsk
	writecocofile fujiutil.dsk time.bin
	writecocofile fujiutil.dsk drives.bin	
	writecocofile fujiutil.dsk cfg.bin
	writecocofile fujiutil.dsk fninfo.bin
	writecocofile fujiutil.dsk appkeys.bin
	writecocofile fujiutil.dsk fujilogo.bin
	writecocofile fujiutil.dsk clock.bin
	writecocofile fujiutil.dsk hosts.bin
	writecocofile fujiutil.dsk guid.bin
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

clock.bin: clock.o    
	$(CC) -o clock.bin clock.o $(LIBS)

hosts.bin: hosts.o    
	$(CC) -o hosts.bin hosts.o $(LIBS)

guid.bin: guid.o    
	$(CC) -o guid.bin guid.o $(LIBS)

fujilogo.bin: fujilogo.o	
	$(CC) -o fujilogo.bin fujilogo.o $(LIBS)

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

clock.o: src/clock.c
	$(CC) $(CFLAGS) -c src/clock.c	

hosts.o: src/hosts.c
	$(CC) $(CFLAGS) -c src/hosts.c	

guid.o: src/guid.c
	$(CC) $(CFLAGS) -c src/guid.c	

clean:
	$(RM) *.o
	$(RM) *.bin
	$(RM) fujiutil.dsk
