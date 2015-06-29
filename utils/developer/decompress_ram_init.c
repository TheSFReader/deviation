#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* The Devo bootloaders store their RAM initialization in a compressed format
   The follwoing code will decopress the ROM storage to show the RAM contents
*/

#define dbgprintf if(0) printf

/* Example from devo7e */

#define LENGTH 0x304
unsigned char data[] = {
/*000 */ 0x01, 0x2A, 0x13, 0x12, 0x01, 0x32, 0x01, 0x16, 0x40, 0x83, 0x04, 0x11, 0xDF, 0x10, 0x09, 0x02,
/*010 */ 0x01, 0x02, 0x03, 0x01, 0x09, 0x02, 0x2D, 0x13, 0x01, 0x01, 0x35, 0x80, 0x20, 0x09, 0x04, 0x1D,
/*020 */ 0xFE, 0x01, 0x02, 0x04, 0x09, 0x2A, 0x01, 0x09, 0x1A, 0x05, 0x09, 0x2A, 0x02, 0x09, 0x26, 0x06,
/*030 */ 0x09, 0x21, 0x0B, 0xFF, 0x10, 0x0B, 0x04, 0x1A, 0x01, 0x04, 0x03, 0x09, 0x04, 0x26, 0x03, 0x53,
/*040 */ 0x12, 0x54, 0x12, 0x4D, 0x12, 0x69, 0x12, 0x63, 0x12, 0x72, 0x12, 0x6F, 0x12, 0x65, 0x1A, 0x6C,
/*050 */ 0x04, 0x12, 0x63, 0x3A, 0x74, 0x0E, 0x3A, 0x6E, 0x18, 0x12, 0x73, 0x5A, 0x14, 0x26, 0x12, 0x33,
/*060 */ 0x12, 0x32, 0x12, 0x20, 0x12, 0x44, 0x12, 0x46, 0x12, 0x55, 0x14, 0x20, 0x03, 0x44, 0x12, 0x45,
/*070 */ 0x12, 0x56, 0x12, 0x4F, 0x12, 0x2D, 0x1A, 0x37, 0x0A, 0xF2, 0x20, 0x14, 0x60, 0x03, 0x40, 0x1A,
/*080 */ 0x49, 0x42, 0x1A, 0x74, 0x50, 0x1A, 0x72, 0x08, 0x1A, 0x61, 0x5A, 0x1A, 0x20, 0x3A, 0x1A, 0x6C,
/*090 */ 0x0A, 0x12, 0x73, 0x1A, 0x68, 0x0C, 0x12, 0x20, 0x12, 0x2F, 0x12, 0x30, 0x1A, 0x78, 0x04, 0x1A,
/*0a0 */ 0x38, 0x04, 0x1A, 0x30, 0x5E, 0x29, 0x06, 0x3A, 0x30, 0x16, 0x12, 0x30, 0x3A, 0x2A, 0x06, 0x12,
/*0b0 */ 0x31, 0x1A, 0x4B, 0x30, 0x1A, 0x2C, 0x78, 0x12, 0x34, 0x9A, 0x34, 0x14, 0x12, 0x67, 0x1A, 0x62,
/*0c0 */ 0x60, 0x12, 0x53, 0x1A, 0x50, 0x64, 0xA9, 0x56, 0x1A, 0x3A, 0x0E, 0x3A, 0x43, 0xBA, 0x1A, 0x66,
/*0d0 */ 0xBC, 0x7A, 0x67, 0x62, 0x49, 0x02, 0x9A, 0x32, 0x62, 0x29, 0x6E, 0x29, 0x50, 0x3A, 0x34, 0x4E,
/*0e0 */ 0x01, 0x14, 0x0A, 0x17, 0x6A, 0x62, 0x1A, 0x4C, 0x5C, 0x1A, 0x62, 0xD6, 0x1A, 0x61, 0x04, 0xBA,
/*0f0 */ 0x79, 0x64, 0x69, 0x62, 0xC9, 0x64, 0x0A, 0x19, 0x36, 0x66, 0x91, 0x13, 0xFD, 0x0F, 0x0A, 0x32,
/*100 */ 0x08, 0x04, 0xD1, 0x12, 0x30, 0x23, 0x08, 0x2A, 0x33, 0x20, 0x12, 0x1A, 0x3C, 0x08, 0x32, 0x2D,
/*110 */ 0x13, 0xE1, 0x07, 0x14, 0x08, 0x6D, 0x06, 0x1B, 0x08, 0x2D, 0x08, 0x1A, 0x31, 0x04, 0x13, 0xA1,
/*120 */ 0x04, 0x14, 0x08, 0x59, 0x05, 0x1B, 0x08, 0x49, 0x04, 0x1A, 0x1D, 0x04, 0x1A, 0x11, 0x04, 0x1A,
/*130 */ 0x29, 0x04, 0x41, 0x32, 0x40, 0x1A, 0x69, 0x38, 0x32, 0x04, 0x1A, 0x6D, 0x08, 0x32, 0x26, 0x1A,
/*140 */ 0x93, 0x08, 0x32, 0x14, 0x1A, 0xA7, 0x08, 0x32, 0x20, 0x1A, 0xC7, 0x08, 0x32, 0x60, 0x13, 0x27,
/*150 */ 0x01, 0x33, 0x20, 0x62, 0x1A, 0x89, 0x08, 0x32, 0x6A, 0x4B, 0x01, 0x01, 0xC4, 0x1A, 0x09, 0x64,
/*160 */ 0x09, 0x16, 0xAC, 0x1A, 0x21, 0x1C, 0x41, 0x19, 0x2B, 0x01, 0x1D, 0x00 };

void uncompress_string(unsigned char *from_addr, unsigned char *to_addr, int length);

void main() {
    int i;
    unsigned char result[LENGTH];
    memset(result, 0, sizeof(result));
    int len = sizeof(data);
    uncompress_string(data, result, sizeof(result));
    for (i = 0; i < sizeof(result); i++) {
        printf("%c", result[i]);
    }
}

void uncompress_string(unsigned char *_from_addr, unsigned char *_to_addr, int length)
{
  unsigned char *from_addr = _from_addr;
  unsigned char *to_addr   = _to_addr;

  unsigned long last_to_addr;
  unsigned int orig_byte;
  int bytes_to_copy;
  int bytes_to_zero;

  last_to_addr = (unsigned long)&to_addr[length];

  while ( (unsigned long)to_addr < last_to_addr )
  {
    orig_byte = *from_addr++;
    bytes_to_copy = orig_byte & 0x07;
    if ( ! bytes_to_copy) {
      // if more than 7 bytes to copy, get count from next byte
      bytes_to_copy = *from_addr++;
    }
    bytes_to_zero = orig_byte >> 4;
    if ( ! bytes_to_zero) {
      // if more than 16 bytes to zero, get count from next byte
      bytes_to_zero = *from_addr++;
    }
    if (bytes_to_copy > 1) {
        dbgprintf("\nCopying %d bytes from %04lx to %04lx:\n\t", bytes_to_copy-1, from_addr - _from_addr, to_addr - _to_addr);
        while (--bytes_to_copy) {
            dbgprintf("%c ", *from_addr);
            *to_addr++ = *from_addr++;
        }
    }
    if ( orig_byte & 0x08 ) {
      //instead of zeroing bytes, copy from previously-copied bytes
      unsigned char addr_offset = *from_addr++;
      unsigned char *ram_from_addr = &to_addr[-addr_offset];
      bytes_to_zero += 2;
      dbgprintf("\nDuplicating %d bytes from %04lx to %04lx:\n\t", bytes_to_zero, ram_from_addr - _to_addr, to_addr - _to_addr);
      while (bytes_to_zero-- )
      {
        dbgprintf("%c ", *ram_from_addr);
        *to_addr++ = *ram_from_addr++;
      }
    } else {
      dbgprintf("\nZeroing %d bytes at %04lx", bytes_to_zero, to_addr - _to_addr);
      while ( bytes_to_zero--)
      {
        *to_addr++ = 0;
      }
    }
  }
  dbgprintf("\n");
}

