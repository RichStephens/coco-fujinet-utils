Simple binary utilities for the Color Computer to work with the FujiNet device.

APPKEYS.BIN - Allows you to peruse the app keys created by various applications, stored on the FujiNet SD card.

CFG.BIN - Resets the drive configuration to point to the initial configuration boot disk and resets the CoCo to re-run the config program without having to power off or hit the reset button.

CLOCK.BIN - A full-screen character-based clock based on the FujiNet clock.

DRIVES.BIN - Prints out the current drive slot configuration, much like what you see when you exit the config program.

FNINFO.BIN - Prints out the FujiNet adapter network information and firmware version.

FUJILOGO.BIN - Just for fun.  Draws a full-size FujiNet logo on the screen.

GUID.BIN - Prints out a randomly-generated GUID. (This feature is only available with the latest nightly build of the FujiNet Firmware)

HOSTS.BIN - Prints out the currently configured host slot configuration.

TIME.BIN - Prints out the current date/time.  (NOTE: If you get an improper value for the year, update your firmware in the FujiNet device to the latest release or nightly build.

These files are contained on a Color Computer disk image file, fujiutil.dsk.

Building this requires a Linux system with a copy of the C compiler cmoc, found at  http://sarrazip.com/dev/cmoc.html, and the lwtools assembler/linker utilities, found at http://lwtools.ca, the Color Computer toolshed, found at https://github.com/n6il/toolshed, and the fujinet-lib, found at https://github.com/FujiNetWIFI/fujinet-lib.

You will need to manually build fujinet-lib from source, because these programs use functions not yet available in the latest released version of fujinet-lib.

