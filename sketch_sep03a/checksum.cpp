#include "checksum.h"

/*
data checksums
The checksums are a mess. There are four "types" of checksums:
Type 0: this is a CRC16 checksum of the first 0x1E unsigned chars of sector 0. The checksum itself is stored in block 0x01, offset 0x0E.
Type 1: this is a CRC16 checksum of the data area header. As there are two data areas, there are two of these checksums. 
One is at block 0x08, offset 0x0E, and the other is at block 0x24, offset 0x0E.
Type 2: this is a CRC16 checksum of the data area. As there are two data areas, there are two of these checksums. 
One is at block 0x08, offset 0x0C, and the other is at block 0x24, offset 0x0C.
Type 3: this is another CRC16 checksum of the data area, except padded with zeroes. As there are two data areas, 
there are two of these checksums. One is at block 0x08, offset 0x0A, and the other is at block 0x24, offset 0x0A.
As type 0 is a checksum of a *supposedly* read-only sector, it's not all that important. It's also very straightforward to understand.

The type 1 checksum is a checksum of just one block, the data area header (0x08 and 0x24). As it's also stored WITHIN the 
data area header, a default value must be supplied for the checksum before actually calculating it. That value is 0x0005.

The type 2 checksum is actually only a checksum of the first 4 blocks (EXCLUDING the data area header, and the access control blocks).

The type 3 checksum is a checksum of the next 4 blocks (EXCLUDING the data area header, and the access control blocks), 
and then 0x0E blocks of zeroes.

Just to re-iterate, the encryption is applied AFTER all this checksum mess is done.
*/

// CCITT CRC Code
// Update the CRC for transmitted and received data using
// the CCITT 16bit algorithm (X^16 + X^12 + X^5 + 1).
 unsigned short Checksum::UpdateCcittCrc16(unsigned short crc16, unsigned char data)
{
	unsigned short num2 = (unsigned short) (data << 8);
	for (unsigned int i = 0; i < 8; i++)
	{
		int num3;
		if ((crc16 ^ num2) > 0x7fff)
		{
			num3 = 1;
		}
		else
		{
			num3 = 0;
		}
		crc16 = (unsigned short) ((crc16 << 1) ^ (num3 * 0x1021));
		num2 = (unsigned short) (num2 << 1);
	}
	return crc16;
}

unsigned short Checksum::ComputeCcittCrc16(void const* data, unsigned int bytes)
{
	unsigned short crc = 0xffff;
	unsigned char const* numPtr = (unsigned char const*)data;
	for (unsigned int i = 0; i < bytes; i++)
	{
		crc = UpdateCcittCrc16(crc, *((unsigned char const*) (numPtr + i)));
	}
	return crc;
}

 
