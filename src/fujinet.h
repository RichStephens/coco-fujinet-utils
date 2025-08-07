#ifndef FUJINET_H
#define FUJINET_H

typedef unsigned char byte;
#define FILE_MAXLEN    36

typedef struct {
  unsigned char hostSlot;
  unsigned char mode;
  unsigned char file[FILE_MAXLEN];
} DeviceSlot;

/**
 * @brief Drivewire Opcodes we added for FujiNet.
 */
#define OP_FUJI 0xE2
#define OP_TIME 0x23
#define OP_NET 0xE3
#define OP_RESET1 0xFF
#define CMD_READY 0x00
#define CMD_RESPONSE 0x01
#define FUJI_DEVICEID_CLOCK 0x45
#define FUJICMD_APETIME_GETTIME 0x93

/**
 * @brief Read string to s from DriveWire with expected length l
 * @param s pointer to string buffer
 * @param l expected length of string (0-65535 bytes)
 * @return 1 = read successful, 0 = not successful
 */

byte dwread(byte *s, int l);

/**
 * @brief Write string at s to DriveWire with length l
 * @param s pointer to string buffer
 * @param l length of string (0-65535 bytes)
 * @return error code
 */

byte dwwrite(byte *s, int l);

int fuji_get_response(byte *buf, int len);
void fuji_ready(void);
#endif /* FUJINET_H */
