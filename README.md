Simple binary utilities for the Color Computer to work with the FujiNet device.

TIME.BIN - Prints out the current date/time.  (NOTE: If you get an improper value for the year, update your firmware in the FujiNet device to the latest release or nightly build.

DRIVES.BIN - Prints out the current drive slot configuration, much like what you see when you exit the config program.

CFG.BIN - Resets the drive configuration to point to the initial configuration boot disk and resets the CoCo to re-run the config program without having to power off or hit the reset button.

FNINFO.BIN - Prints out the FujiNet adapter network information and firmware version.

APPKEYS.BIN - Allows you to peruse the app keys created by various applications, stored on the FujiNet SD card.

These files are contained on a Color Computer disk image file, fujiutil.dsk.

Building this requires a Linux system with a copy of the C compiler cmoc, found at  http://sarrazip.com/dev/cmoc.html, and the lwtools assembler/linker utilities, found at http://lwtools.ca, and the fujinet-lib, found at https://github.com/FujiNetWIFI/fujinet-lib.

You will need to manually build fujinet-lib from source, because the config programs use some internal header files in the coco-specific source.  It is expected that the fujinet-lib directory be at the same level as the coco-fujinet-utils folder.

NOTE: At present, you must create a soft link to the fujinet.lib.coco file in fujinet-lib/build in order for the applications to link: "ln -s fujinet.lib.coco libfujinet.lib.coco.a"
