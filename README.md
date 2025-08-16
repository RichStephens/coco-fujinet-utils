Simple binary utilities for the Color Computer to work with the FujiNet device.

TIME.BIN - Prints out the current date/time.  (NOTE: There is a bug in the current release of the FujiNet firmware that gives an improper value for the year.  That will be fixed in the next release of the firmware.)

DRIVES.BIN - Prints out the current drive slot configuration, much like what you see when you exit the config program.

CONFIG.BIN - Resets the drive configuration to point to the initial configuration boot disk so you can re-run the config program without having to power off or hit the reset button.

These files are contained on a Color Computer disk image file, fujiutil.dsk.

Building this requires a Linux system with a copy of the C compiler cmoc, found at  http://sarrazip.com/dev/cmoc.html, and the lwtools assembler/linker utilities, found at http://lwtools.ca
