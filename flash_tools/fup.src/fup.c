/*****************************************************************************
 *                                                                           *
 * Name :   fup                                                              *
 *          Management program for Fortis .ird flash files                   *
 *                                                                           *
 * Author:  Schischu, enhanced and expanded by Audioniek                     *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 2 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program; if not, write to the Free Software               *
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA *
 *                                                                           *
 *****************************************************************************
 *
 * Changes in Version 1.9.9b:
 * + -i now also displays the file sizes in the ird file.
 *
 * Changes in Version 1.9.9a:
 * + Improve information output on Atemio (generation 5).
 *
 * Changes in Version 1.9.9:
 * + add change loader reseller ID (command -rl(v)).
 * + when a 4 digit reseller ID is specified with -r(v) or -rl(v)
 *   the remaining two bytes of the new reseller ID are copied from
 *   the reseller ID in the IRD file.
 *
 * Changes in Version 1.9.8f:
 * + Fix bug with 4 digit reseller IDs.
 *
 * Changes in Version 1.9.8e:
 * + support for Atemio AM 530 HD added.
 *
 * Changes in Version 1.9.8d:
 * + -c and -ce did not close the output .ird file upon successful completion.
 * + getHeader incorrectly assumed file pointer was at beginning of ird file.
 * + info for -i on Atemio AM 520 HD updated.
 *
 * Changes in Version 1.9.8c:
 * + Add support for (unused) partition types A - F on -i and -c.
 *
 * Changes in Version 1.9.8b:
 * + Add support for Atemio AM 520 HD (Crenova built).
 *
 * Changes in Version 1.9.8a:
 * + Add some comments regarding block types 0x0a - 0x10 derived
 *   from Fortis loader source code; program code unchanged.
 * 
 * Changes in Version 1.9.8:
 * + Fix wrong squashfs signatures
 * 
 * Changes in Version 1.9.7d:
 * + Add Octagon SF928 GX name info
 *
 * Changes in Version 1.9.7c:
 * + Fix formatting error in help text
 * + Small corrections in model names
 *
 * Changes in Version 1.9.7b:
 * + Fix error in -i with generation 2
 *
 * Changes in Version 1.9.7a:
 * + Fix error in -i with ird file containing loader
 * + Fix error in -x (wrong output file names)
 * + Fix two compiler warnings in Visual Studio
 *
 * Changes in Version 1.9.7:
 * + Fix typo in error message
 * + Fix error in header read back in -ce command
 * + Fix error in reporting illegal suboption in -c command
 * + Improvements in verbose output with -c and -ce
 *
 * Changes in Version 1.9.6d:
 * + Return value of fread was handled wrong with reading back the header.
 * + Fixed error in retrieving SW version in generation 1 loader
 *   in getLoaderdata, simplified code.
 *
 * Changes in Version 1.9.6c:
 * + Added automake style build files.
 * + Fixed struct tPartition errors with automake style compilation.
 * + Fixed compiler warnings with fread and uninitialized variables.
 *
 * Changes in Version 1.9.6b:
 * + -i: generation 2 with loader 6.XX shows actual flash addresses.
 * + -i: if ird file contains a loader partition, the new loaders reseller ID
 *       and SW version are shown, and checks are made on loader
 *       incompatibility and changed reseller (but compatible hardware).
 * + Names of models with resellerId 0x23/0x25 with loader 5.xx now
 *   display correct model name in -i and -r(v).
 *
 * Changes in Version 1.9.6a:
 * + Fix 0x13 error: ird file CRC16 in header was wrong
 *   after -r(v) and -n(v).
 * + Default resellerId moved to fup.h.
 * + Corrected DP7050 info on -i.
 * + Total block count changed to 32 bit.
 * + -c & -ce: SW version number default is 1.00.00.
 *
 * Changes in Version 1.9.6:
 * + -i displays detailed ird file info.
 * + Fixed several errors introduced in 1.9.3 - 1.9.5
 * + define USE_ZLIB uncommented
 *
 * Changes in Version 1.9.5:
 * + -c and -ce accept subarguments -i (set resellerID) and
 *   -s (set software version).
 *
 * Changes in Version 1.9.4:
 * + -rv displays model names.
 *
 * Changes in Version 1.9.3:
 * + -xv displays model name based on header resellerID.
 *
 * Changes in Version 1.9.2:
 * + Use correct mtd numbers and partition names on extract
 *   depending on resellerID.
 *
 * Changes in Version 1.9.1:
 * + -d and -dv options added: create dummy squashfs file.
 *
 * Changes in Version 1.9.0:
 * + Dummy squash header file is only created when needed.
 * + -rv shows old resellerID also.
 *
 * Changes in Version 1.8.3b:
 * + More rigid argument checking with -c and -ce; missing filenames for
 *   suboptions requiring them are reported.
 * + Several bugs involving suboptions -v and -1G in -c and -ce fixed.
 *
 * Changes in Version 1.8.3a:
 * + -ce can now add block types 2, 3, 4 and 5 (config0 - configA);
 * + Option -1G added to -ce to use squashfs dummy file with squashfs 3.0 in
 *   stead of 3.1 as required by first generation Fortis receivers.
 *
 * Changes in Version 1.8.2:
 * + -tv added;
 * + Cosmetic changes to output of -t and -tv.
 *
 * Changes in Version 1.8.1:
 * + Fixed two compiler warnings.
 * + Squashfs dummy file now padded with 0xFF in stead of 0x00; 0xFF
 *   is the erased state of flash memory.
 *
 * Changes in Version 1.8:
 * + If the file dummy.squash.signed.padded does not exist in the
 *   current directory at program start, it is created;
 * + -r option can now change all four reseller ID bytes;
 * + -ce suboptions now have numbered aliases;
 * + On opening a file, the result is tested: no more crashes
 *   on non existent files, but a neat error message;
 * + -n can change version number in IRD;
 * + Silent operation now possible on -x, -s, -n, -c, -r and -ce;
 * + Errors in mtd numbering corrected.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <zlib.h>

#include "fup.h"
#include "crc16.h"
#include "dummy30.h"
#include "dummy31.h"

#define VERSION "1.9.9b"
#define DATE "02.01.2023"

// Global variables
uint8_t verbose = 1;
char printstring;
uint8_t has[MAX_PART_NUMBER];
FILE *fd[MAX_PART_NUMBER];
uint32_t ucLen[MAX_PART_NUMBER];
uint16_t loaderFound;
uint32_t column;
uint8_t t_has[MAX_PART_NUMBER];
char nameOut[MAX_PART_NUMBER][64];
uint32_t partcount;
uint32_t loaderId;
uint32_t loaderSW;
uint32_t systemIDin;
uint32_t SWVersionIn;

/* Functions */

void printVerbose(const char *printstring)
{
	if (verbose)
	{
		printf("%s", printstring);
	}
}

void printProgress(const char *printstring)
{
	if (verbose)
	{
		printf("%s", printstring);
		column++;
		if (column > 80)
		{
			column = 0;
			printf("\n");
		}
	}
}

#if 0  // not needed/used anywhere
void fromint16_t(uint8_t **int16_tBuf, uint16_t val)
{
	*int16_tBuf[0] = val >> 8;
	*int16_tBuf[1] = val & 0xFF;
}
#endif

uint16_t toShort(uint8_t int16_tBuf[2])
{
	return (uint16_t)(int16_tBuf[1] + (int16_tBuf[0] << 8));
}

uint16_t extract_2Bytes(uint8_t dataBuf[], uint16_t pos)
{
	uint8_t int16_tBuf[2];

	memcpy(int16_tBuf, dataBuf + pos, 2);
	return toShort(int16_tBuf);
}

uint16_t readShort(FILE *file)
{
	uint8_t int16_tBuf[2];

	if (fread(int16_tBuf, 1, 2, file) == 2)
	{
		return toShort(int16_tBuf);
	}
	return 0;
}

int32_t getGeneration(int32_t resellerId)
{
	int32_t generation;

	if ((resellerId >> 24) == 0x25 && ((resellerId & 0xff) == 0xA5 || (resellerId & 0xff) == 0xAA))
	{
		generation = 5;  // Crenova model (AM 520 HD or AM 530 HD)
	}
	else
	{
		switch (resellerId >> 24)  // 1st resellerID byte
		{
			case 0x20:  // FS9000, FS9200, HS9510
			{
				generation = 1;
				break;
			}
			case 0x23:  // HS8200
			case 0x25:  // HS7110, HS7420, HS7810A
			{
				generation = 2;  // loader 5.XX or 6.XX
				break;
			}
			case 0x27:  // HS7119, HS7429, HS7819
			{
				generation = 3;  // loader 7.XX
				break;
			}
			case 0x29:  // DP2010, FX6010, DP7000, DP7001, DP7050, GPV8000
			case 0x2a:  // EP8000, EPP8000
			{
				generation = 4;  // loader 8.XX or X.0.X
				break;
			}
			default:
			{
				generation = -1;
				break;
			}
		}
	}
	return generation;
}

void getLoaderdata(uint8_t *dataBuf, uint32_t resellerId)
{
	uint32_t offset;

	if (loaderFound == 1)
	{
		switch (getGeneration(resellerId))
		{
			case 1:
			{
				offset = RESELLER_OFFSET_GEN1;
				break;
			}
			case 2:
			case 3:
			case 5:  // Crenova
			default:
			{
				offset = RESELLER_OFFSET_GEN2;
				break;
			}
			case 4:
			{
				offset = RESELLER_OFFSET_GEN4;
				break;
			}
		}
		loaderId = (dataBuf[offset]     << 24)  // stored little endian
	 	         + (dataBuf[offset + 1] << 16)
	 	         + (dataBuf[offset + 2] <<  8)
	 	         +  dataBuf[offset + 3];
		loaderSW =  dataBuf[offset + 4]  // stored big endian
	 	         + (dataBuf[offset + 5] <<  8)
	 	         + (dataBuf[offset + 6] << 16)
	 	         + (dataBuf[offset + 7] << 24);
		loaderFound = 2;  // flag loader data set
	}
}

int32_t extractAndWrite(FILE *file, uint8_t *buffer, uint16_t len, uint16_t decLen, uint8_t writeflag, uint32_t systemId)
{
	if (len != decLen)
	{
		// zlib
		z_stream strm;
		uint8_t out[DATA_BLOCK_SIZE + 6];

		strm.zalloc = Z_NULL;
		strm.zfree = Z_NULL;
		strm.opaque = Z_NULL;
		strm.avail_in = 0;
		strm.next_in = Z_NULL;
		inflateInit(&strm);

		strm.avail_in = len;
		strm.next_in = buffer;

		strm.avail_out = decLen;
		strm.next_out = out;
		inflate(&strm, Z_NO_FLUSH);

		inflateEnd(&strm);

		if (writeflag)
		{
			fwrite(out, 1, decLen, file);
			printProgress("z");
		}
		getLoaderdata(out, systemId);
		return decLen;
	}
	else
	{
		if (writeflag)
		{
			fwrite(buffer, 1, len, file);
			printProgress(".");
		}
		getLoaderdata(buffer, systemId);
		return len;
	}
}

uint16_t readAndCompress(FILE *file, uint8_t **dataBuf, uint16_t pos, uint16_t *ucDataLen)
{
	uint8_t in[DATA_BLOCK_SIZE + 6];
	uint8_t out[DATA_BLOCK_SIZE + 6];
	z_stream strm;
	uint16_t have;

	*ucDataLen = fread((*dataBuf) + pos, 1, *ucDataLen, file);
	// So now we have to check if zlib can compress this or not

	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	deflateInit(&strm, Z_DEFAULT_COMPRESSION);

	strm.avail_in = *ucDataLen;
	memcpy(in, (*dataBuf) + pos, *ucDataLen);
	strm.next_in = in;

	have = 0;

	strm.avail_out = *ucDataLen;
	strm.next_out = out + have;

	deflate(&strm, Z_FINISH);
	have = *ucDataLen - strm.avail_out;

	deflateEnd(&strm);

	if (have < *ucDataLen)  // if data was compressed
	{
		memcpy((*dataBuf) + pos, out, have);  // copy the compressed out buffer to dataBuf
		printProgress("z");
	}
	else
	{
		have = *ucDataLen;  // return uncompressed length & untouched dataBuf
		printProgress(".");
	}
	return have;
}

uint16_t insertint16_t(uint8_t ** dataBuf, uint16_t pos, uint16_t value)
{
	(*dataBuf)[pos] = value >> 8;
	(*dataBuf)[pos + 1] = value & 0xFF;
	return 2;
}

uint32_t insertint32_t(uint8_t ** dataBuf, uint16_t pos, uint32_t value)
{
	(*dataBuf)[pos]     = value >> 24;
	(*dataBuf)[pos + 1] = value >> 16;
	(*dataBuf)[pos + 2] = value >>  8;
	(*dataBuf)[pos + 3] = value & 0xFF;
	return 2;
}

void setHeader(FILE *irdFile, uint32_t headerDataBlockLen, uint8_t *dataBuf)
{
	uint16_t dataCrc;

	// Rewrite Header Data Block
	fseek(irdFile, 0x04, SEEK_SET);
	fwrite(dataBuf, 1, headerDataBlockLen, irdFile);

	// Update CRC
	dataCrc = crc16(0, dataBuf, headerDataBlockLen);
	insertint16_t(&dataBuf, 0, dataCrc);

	// Rewrite CRC
	fseek(irdFile, 0x02, SEEK_SET);
	fwrite(dataBuf, 1, 2, irdFile);

	free(dataBuf);
}

uint16_t getHeaderLen(FILE *irdFile)
{
	fseek(irdFile, 0x00, SEEK_SET);
	return readShort(irdFile);
}

uint8_t *getHeader(FILE *irdFile)
{
	uint16_t headerDataBlockLen;
	uint8_t *dataBuf;
	size_t count = 0;

	fseek(irdFile, 0, SEEK_SET);
//	headerDataBlockLen = 0;  // header length
	headerDataBlockLen = readShort(irdFile);
	headerDataBlockLen -= 2;

	// Read Header Data Block
	dataBuf = (uint8_t *)malloc(headerDataBlockLen);
	fseek(irdFile, 0x04, SEEK_SET);  // 0x04 -> skip length & CRC
	count = fread(dataBuf, 1, headerDataBlockLen, irdFile);

	if (count != headerDataBlockLen)
	{
		printf("ERROR: Reading header failed.\n");
		return (uint8_t *) -1;
	}
	return dataBuf;
}

uint32_t getresellerID(FILE *irdFile)
{
	uint8_t *dataBuf;

	dataBuf = getHeader(irdFile);
	return (((dataBuf[2] << 8) + dataBuf[3]) << 16) + (dataBuf[4] << 8) + dataBuf[5];  // resellerID from file
}

uint32_t getSWversion(FILE *irdFile)
{
	uint8_t *dataBuf;

	dataBuf = getHeader(irdFile);
	return (((dataBuf[12] << 8) + dataBuf[13]) << 16) + (dataBuf[14] << 8) + dataBuf[15];  // SWversion from file
}

int32_t writeBlock(FILE *irdFile, FILE *inFile, uint8_t firstBlock, uint16_t type)
{
	uint32_t resellerId;
	uint16_t cDataLen = 0;
	uint16_t ucDataLen = DATA_BLOCK_SIZE;
	uint16_t dataCrc = 0;
	uint8_t *blockHeader = (uint8_t *)malloc(4);
	uint8_t *dataBuf = (uint8_t *)malloc(DATA_BLOCK_SIZE + 4);

//#if defined USE_ZLIB
	if (firstBlock && type == 0x10)
//#else
//	if (firstBlock && type == 0x01)
//#endif
	{  // header
	/**********************************************************************************************
	 *
	 * Loader definition of header block:
	 * Offset   Size      CRC  Name/purpose
	 * -------------------------------------------
	 *   0x00   uint16_t   N   length of header or normal block length
	 *   0x02   uint16_t   N   CRC16 over rest of block
	 *   0x04   uint16_t   Y   _xfdVer -> transfer format, 0x10 = compressed (type in data block)
	 *   0x06   uint32_t   Y   _systemId -> resellerId
	 *   0x0a   uint32_t   Y   _nDataBlk -> number of blocks in file
	 *   0x0e   uint32_t   Y   SWversion
	 *
	 **********************************************************************************************/
		resellerId = RESELLER_ID;  // set default reseller ID

		insertint16_t(&dataBuf,  0, type);  // _xfdVer
		insertint32_t(&dataBuf,  2, resellerId);
		insertint16_t(&dataBuf,  6, 0);  // uncompressed length
		insertint32_t(&dataBuf,  8, 0xFFFFFFFF);  // block count
		insertint32_t(&dataBuf, 12, 0x00010000);  // SWversion, default 1.00.00
		cDataLen = 12;
		ucDataLen = cDataLen;  // quit after 1 (this) block
	}
	else
	{  // normal partition data block
	/**********************************************************************************************
	 *
	 * Loader definition of data block format:
	 * Offset   Size      CRC  Name/purpose
	 * -------------------------------------------
	 *   0x00   uint16_t   N   block length
	 *   0x02   uint16_t   N   CRC16 over rest of block
	 *   0x04   uint16_t   Y   block type (0x00 - 0x0f, or 0x10 = write to EEPROM)
	 *   0x06   uint16_t   Y   data length (uncompressed length)
	 *   0x08-   uint8_t   Y   block data (compressed length bytes in total)
	 *
	 **********************************************************************************************/
		cDataLen = readAndCompress(inFile, &dataBuf, 4, &ucDataLen);
		insertint16_t(&dataBuf, 0, type);
		insertint16_t(&dataBuf, 2, ucDataLen);
	}
	dataCrc = crc16(dataCrc, dataBuf, cDataLen + 4);

	insertint16_t(&blockHeader, 0, cDataLen + 6);
	insertint16_t(&blockHeader, 2, dataCrc);
	fwrite(blockHeader, 1, 4, irdFile);
	fwrite(dataBuf, 1, cDataLen + 4, irdFile);

	free(blockHeader);
	free(dataBuf);
	return ucDataLen;
}

char *getModelName(int32_t resellerId)
{
	int32_t i;
	int32_t generation;

	for (i = 0; i < sizeof(fortisNames); i++)
	{
		if (fortisNames[i].resellerID == resellerId)
		{
			break;
		}
	}
	if (i == sizeof(fortisNames))  // if not found, retry for loader 6.XX
	{
		generation = getGeneration(resellerId);
		if (generation == 2 || generation == 3)
		{
			resellerId |= 0xA0;
			for (i = 0; i < sizeof(fortisNames); i++)
			{
				if (fortisNames[i].resellerID == resellerId)
				{
					break;
				}
			}
		}
	}
	if (i == sizeof(fortisNames))
	{
		return (char *)"Unknown model";
	}
	return (char *)fortisNames[i].reseller_name;
}

struct tPartition *getTableAddr(uint32_t resellerId, uint32_t SWVersion)
{
	uint32_t generation;
	struct tPartition *partTable = partData2d;  // default is HS8200 L6.00

	generation = getGeneration(resellerId);

	switch (generation)
	{
		case 1:
		{
			partTable = partData1;
			break;
		}
		case 2:
		{
			switch (resellerId & 0xf0)  // get 4th reseller byte, less lower nibble
			{
				case 0:  // loader 5.XX
				{
					if ((resellerId >> 24) == 0x25)
					{
						partTable = partData2a;  // HS7110, HS7420, HS7810A
					}
					else
					{
						partTable = partData2b;  // HS8200
					}
					break;
				}
				case 0xA0:  // loader 6.XX
				{
					if ((resellerId >> 24) == 0x25)
					{
						partTable = partData2c;  // HS7110, HS7420, HS7810A
					}
					else
					{
						partTable = partData2d;  // HS8200
					}
					break;
				}
			}
			break;
		}
		case 3:
		{
			partTable = partData3;
			break;
		}
		case 4:
		{
			if (((resellerId >> 8) & 0xff) != 0x10)
			{
				partTable = partData4;
			}
			else
			{
				partTable = partData4a;  // DP7050
			}
			break;
		}
		case 5:
		{
			if (((SWVersion >> 16) & 0xff) < 3)  // Version 3 and up flash to NOR
			{
				partTable = partData5_nand;  // Crenova model, NAND flash
			}
			else
			{
				partTable = partData5_nor;  // Crenova model, NOR flash
			}
			break;
		}
		default:
		{
			printVerbose("\nCAUTION: Unknown receiver model detected (probably an SD model).\n");
			partTable = partData1;
			break;
		}
	}
	return partTable;
}

/***********************************************************
 *
 * readBlock
 *
 * Read one block of data from input file *file.
 * If writeflag set, writes data into output file *name
 *
 * Returns: number of bytes written.
 *
 */
int32_t readBlock(FILE *file, char *name, uint8_t firstBlock, uint8_t writeflag, int verboseFlag)
{
	uint16_t blockCounter = 0;
	uint16_t len = 0;
	uint16_t decLen;
	uint16_t crc_16;
	uint16_t dataCrc = 0;
	uint16_t type = 0;
	uint8_t dataBuf[DATA_BLOCK_SIZE + 6];
	uint16_t fpVersion;
	uint32_t blockCount;
	uint32_t systemId = systemIDin;
	uint16_t SWVersion1;
	uint16_t SWVersion2;
	char *modelName;
	struct tPartition *tableAddr = partData2d;  // default is HS8200 L6.00
	uint16_t part_number = 0;

	len = readShort(file);  // get length of block
	if (len < 1)
	{
		return 0;
	}
	crc_16 = readShort(file);  // get block CRC in file

	if (fread(dataBuf, 1, len - 2, file) != len - 2)  // get block data
	{
		return 0;
	}
	dataCrc = 0;
	dataCrc = crc16(dataCrc, dataBuf, len - 2);  // get actual block CRC

	if (crc_16 != dataCrc)
	{
		printf("\nERROR: CRC16 not correct in block #%d (type %d, %s).\n", blockCounter, type, (type == 0 ? "Header" : tableAddr[type].Description));
//		getchar();
		return -1;
	}
	type = extract_2Bytes(dataBuf, 0);
	blockCounter++;

#if 0 //not defined USE_ZLIB
	if (firstBlock && type == 0x01)
#else
	if (firstBlock && type == 0x10)
#endif
	{
		part_number++;
		fpVersion = extract_2Bytes(dataBuf, 0);
		systemId = ((extract_2Bytes(dataBuf, 2)) << 16) + extract_2Bytes(dataBuf, 4);
		blockCount = ((extract_2Bytes(dataBuf, 6)) << 16) + extract_2Bytes(dataBuf, 8);
		SWVersion1 = extract_2Bytes(dataBuf, 12);
		SWVersion2 = extract_2Bytes(dataBuf, 14);

		if (verboseFlag == 1)
		{
			modelName = getModelName(systemId);
			printf("\n Header data:\n");
			printf("  tfpVersion      : 0x%02X\n", fpVersion);
			printf("  Reseller ID     : %08X (%s)\n", systemId, modelName);
			printf("  # of blocks     : %d (0x%0X) blocks\n", blockCount, blockCount);
			printf("  SoftwareVersion : V%X.%02X.%02X\n", SWVersion1, SWVersion2 >> 8, SWVersion2 & 0xFF);
		}
		systemIDin = systemId;
		SWVersionIn = ((SWVersion1 << 16) + SWVersion2) & 0x00FFFFFF;
	}
	else
	{  // not header block but normal partition block
		if (type < MAX_PART_NUMBER)
		{
			if (!has[type])
			{
				part_number++;
				has[type] = (uint8_t)part_number;
				ucLen[type] = 0;
				column = 0;
				tableAddr = getTableAddr(systemIDin, SWVersionIn);

				if (verboseFlag)
				{
					printf("\nPartition found, type %02X -> %s (%s, %s", type, tableAddr[type].Description, tableAddr[type].Extension2, tableAddr[type].FStype);
					if (tableAddr[type].Flags & PART_SIGN)
					{
						printf(", signed");
					}
					printf(")\n");
				}
				if (type == 0x00 && loaderFound == 0)
				{
					loaderFound = 1;
				}

				if (writeflag)
				{
					/* Build output file name */
					strncpy(nameOut[type], name, strlen(name));  // get basic output file name
					strcat(nameOut[type], tableAddr[type].Extension);
					strcat(nameOut[type], ".");
					strcat(nameOut[type], tableAddr[type].Extension2);

					fd[type] = fopen(nameOut[type], "wb");

					if (fd[type] == NULL)
					{
						printf("\nERROR: Cannot open output file %s.\n", nameOut[type]);
						return -1;
					}
					if (verboseFlag)
					{
						printf("Writing partition data to file %s\n", nameOut[type]);
					}
				}
				else
				{
					t_has[partcount] = (type == 0 ? 0x10 : type);
					partcount++;
				}
			}
			decLen = extract_2Bytes(dataBuf, 2);
			ucLen[type] += decLen;
			extractAndWrite(fd[type], dataBuf + 4, len - 6, decLen, writeflag, systemId);
		}
		else
		{
			printf("\nERROR: Illegal partition type %02X found.\n", type);
			return -1;
		}
	}
	return len;
}

void scanBlocks(FILE *irdFile)
{  // finds the types of partitions in irdFile and sets has[] and ucLen[] accordingly.
	int32_t len;
	int32_t pos = 0;
	int32_t i;
	uint8_t	firstBlock = 1;

	for (i = 0; i < MAX_PART_NUMBER; i++)
	{
		has[i] = 0;
		t_has[i] = 0;
	}
	while (!feof(irdFile))
	{
		pos = ftell(irdFile);
		len = readBlock(irdFile, (char *)"dummy", firstBlock, 0, 0);
		firstBlock = 0;
		if (len < 0)  // error ocurred
		{
			goto out;
		}
		if (len > 0)
		{
			pos += len + 2;
			fseek(irdFile, pos, SEEK_SET);
		}
		else
		{
			break;
		}
	}

out:
	fclose(irdFile);
}

void createDummy(void)
{
	FILE *file;

	// Check if the file dummy.squash.signed.padded exists. If not, create it
	file = fopen("dummy.squash.signed.padded", "rb");
	if (file == NULL)
	{
		printVerbose("\nSigned dummy squashfs headerfile does not exist.\n");
		printVerbose("Creating it...");
		column = 0;
		file = fopen("dummy.squash.signed.padded", "wb");
		printProgress(".");

		fwrite(dummy, 1, dummy_size, file);
		printProgress(".");

		fclose(file);
		printProgress(".");
		file = fopen("dummy.squash.signed.padded", "rb");
		printProgress(".");

		if (file != NULL)
		{
			printVerbose("\n\nCreating signed dummy squashfs header file successfully completed.\n");
		}
		else
		{
			printf("\nERROR: Could not write signed dummy squashfs header file.\n");
			remove("dummy.squash.signed.padded");
		}
	}
//	else
//	{
//		printVerbose("Signed dummy squashfs headerfile exists already, doing nothing\n");
//	}
}

void deleteDummy(void)
{
	FILE *file;

	file = fopen("dummy.squash.signed.padded", "rb");
	if (file != NULL)
	{
		fclose(file);
		remove("dummy.squash.signed.padded");
	}
}

void changeResellerID(FILE *irdFile, uint32_t resellerId)
{
	uint32_t systemId;
	uint16_t headerDataBlockLen;
	uint8_t *dataBuf;

	headerDataBlockLen = getHeaderLen(irdFile);
	dataBuf = getHeader(irdFile);

	systemId = (dataBuf[2] << 24) + (dataBuf[3] << 16) + (dataBuf[4] << 8) + dataBuf[5];  // resellerID from file

	// Update Reseller ID
	insertint32_t(&dataBuf, 2, resellerId);

	setHeader(irdFile, headerDataBlockLen - 2, dataBuf);

	if (verbose)
	{
		printf("Changed reseller ID\n");
		printf("from %08X (%s)\n", systemId, getModelName(systemId));
		printf("  to %08X (%s).\n", resellerId, getModelName(resellerId));
	}
}

void changeSWVersion(FILE *irdFile, uint32_t SWVersion)
{
	uint32_t oldSWversion1;
	uint32_t oldSWversion2;
	uint8_t *dataBuf;
	uint32_t headerDataBlockLen;

	headerDataBlockLen = getHeaderLen(irdFile);
	dataBuf = getHeader(irdFile);

	oldSWversion1 = extract_2Bytes(dataBuf, 12);  // Get current SW version hi from file
	oldSWversion2 = extract_2Bytes(dataBuf, 14);  // Get current SW version lo from file

	// Update Software version number
	insertint16_t(&dataBuf, 12, (SWVersion >> 16));
	insertint16_t(&dataBuf, 14, (SWVersion & 0x0000FFFF));

	setHeader(irdFile, headerDataBlockLen - 2, dataBuf);

	if (verbose)
	{
		printf("Changed software version number ");
		printf("from V%X.%02X.%02X ", oldSWversion1 & 0xFFFF, oldSWversion2 >> 8, oldSWversion2 & 0xFF);
		printf("to V%X.%02X.%02X\n", SWVersion >> 16, (SWVersion & 0x0000FFFF) >> 8, SWVersion & 0xFF);
	}
}


int32_t main(int32_t argc, char* argv[])
{
	if ((argc == 3 && strlen(argv[1]) == 2 && strncmp(argv[1], "-i", 2) == 0))
	{  // -i: display file info
		uint32_t i, j;
		uint32_t resellerId;
		uint32_t SWVersion;
		uint8_t *dataBuffer;
		FILE *irdFile;
		uint16_t generation;
		struct tPartition *tableAddr;
		uint32_t Offset[MAX_PART_NUMBER];
		uint32_t Size[MAX_PART_NUMBER];

		irdFile = fopen(argv[2], "r");
		if (irdFile == NULL)
		{
			printf("ERROR: cannot open input file %s.\n", argv[2]);
			return -1;
		}
		printf("\nInformation on flash file %s\n\n", argv[2]);
		printf("Header info:\n");
		resellerId = getresellerID(irdFile);
		printf("  Reseller ID      : 0x%08X\n", resellerId);
		printf("  Reseller model   : %s\n", getModelName(resellerId));
		SWVersion = getSWversion(irdFile);
		printf("  Software version : V%X.%02X.%02X\n", SWVersion >> 16, (SWVersion & 0x0000FF00) >> 8, SWVersion & 0xFF);
		dataBuffer = getHeader(irdFile);
		i = ((extract_2Bytes(dataBuffer, 6)) << 16) + extract_2Bytes(dataBuffer, 8);  // block count
		printf("  Number of blocks : %d (0x%04X)\n", i, i);
#if 0 //not defined USE_ZLIB
		j = extract_2Bytes(dataBuffer, 0) >> 1;
		printf("  File size        : %d (Calculated from # of blocks)\n", ((i << 18) + j));
#endif
		fseek(irdFile, 0, SEEK_SET);
		partcount = 0;
		loaderFound = 0;
		scanBlocks(irdFile);

		// Check if .ird file contains a loader partition
		if (loaderFound == 2)
		{
			printf("Loader info:\n");
			printf("  Loader reseller ID : 0x%08X ", loaderId);
			if (loaderId == resellerId)
			{
				printf(" (same model)");
			}
			else
			{
				printf ("(%s)", getModelName(loaderId));
			}
			printf("\n  Loader version     : V%X.%02X.%02X\n", (loaderSW >> 16) & 0xff, (loaderSW >> 8) & 0xff, loaderSW & 0xff);
			if ((loaderId & 0xff00ff5f) != (resellerId & 0xff00ff5f))
			{
				printf("\nCAUTION: Loader is NOT compatible with hardware for resellerID %08X!\n", resellerId);
				printf("         Loader is for a(n) %s (%08X).\n", getModelName(loaderId), loaderId);
			}
			else if ((loaderId & 0x00ff00ff) != (resellerId & 0x00ff00ff))
			{
				printf("\nCAUTION: After flashing this file, reseller code is changed to %08X.\n", loaderId);
#if 0
				if ((loaderId & 0xffffff5f) != (resellerId & 0xffffff5f))
				{
					printf("         Reseller code %08X is for a(n) %s.\n", loaderId, getModelName(loaderId));
				}
#endif
			}
		}
		tableAddr = getTableAddr(resellerId, SWVersion);
		generation = getGeneration(resellerId);

		// Handle generation 2 & generation 5 (Atemio AM 520/530 HD) (possible variable layout)
		if (((generation == 2) && (resellerId & 0xf0) == 0xa0)
		||  ((generation == 5) && (resellerId & 0xff) == 0xa5)
		||  ((generation == 5) && (resellerId & 0xff) == 0xaa))
		{
			// Step one: round the mtd sizes up to the next erase boundary
			for (i = 0; i < MAX_PART_NUMBER; i++)
			{
				if ((ucLen[t_has[i]] % ERASE_SIZE != 0) && (tableAddr[t_has[i]].Size == 0x7f))
				{
					j = (ucLen[t_has[i]] % ERASE_SIZE);
					ucLen[t_has[i]] -= j;
					ucLen[t_has[i]] += ERASE_SIZE;
				}
			}
			// Step two: Calculate the offsets and sizes in case they are variable;
			// because the partitions can appear in the flash file in any order, this
			// has to be done in sequence of flashing for types 8, 7 and 1.
			for (i = 0; i < MAX_PART_NUMBER; i++)
			{
				if (t_has[i] == 0x10)  // loader
				{
					// Handle loader (type0, fixed offset, fixed length)
					Offset[0] = tableAddr[0].Offset;
					Size[0] = tableAddr[0].Size;
				}
				if (t_has[i] == 6)  // kernel
				{
					// Handle kernel (type 6, fixed offset, variable length)
					Offset[6] = tableAddr[6].Offset;
					if (tableAddr[6].Size == 0x7f)
					{
						Size[6] = ucLen[6];
					}
					else
					{
						Size[6] = tableAddr[6].Size;
					}
				}
				// Handle Config (types 2, 3, 4 & 5; fixed offset, variable length)
				for (j = 2; j < 6; j++)
				{
					if (t_has[i] == j)
					{
						Offset[j] = tableAddr[j].Offset;
						if (tableAddr[j].Size == 0x7f)
						{
							Size[j] = ucLen[j];
						}
						else
						{
							Size[j] = tableAddr[j].Size;
						}
					}
				}
				// Handle User (type 9; fixed offset, variable length)
				if (t_has[i] == 9)
				{
					Offset[9] = tableAddr[9].Offset;
					Size[9] = ucLen[9];
				}
			}
			if (generation == 5)  // (iBoot flashes type 7 followed by type 8)
			{
				for (i = 0; i < MAX_PART_NUMBER; i++)
				{
					if (t_has[i] == 7)  // dev
					{
						if (tableAddr[7].Offset == 0x7f)
						{
							Offset[7] = Offset[6] + Size[6];  // kernel offset + kernel length
						}
						else
						{
							Offset[6] = tableAddr[6].Offset;
						}
						if (tableAddr[7].Size == 0x7f)
						{
							Size[7] = ucLen[7];
						}
						else
						{
							Size[7] = tableAddr[7].Size;  // dev length
						}
					}
				}
				// Handle rootfs (type 8, variable offset, variable length)
				for (i = 0; i < MAX_PART_NUMBER; i++)
				{
					if (t_has[i] == 8)  // rootfs
					{
						if (tableAddr[8].Offset == 0x7f)
						{
							Offset[8] = Offset[7] + Size[7];  // dev offset + dev length
						}
						else
						{
							Offset[8] = tableAddr[8].Offset;
						}
						if (tableAddr[8].Size == 0x7f)
						{
							Size[8] = ucLen[8];
						}
						else
						{
							Size[8] = tableAddr[8].Size;
						}
					}
				}
			}
			if (generation == 2)  // (Fortis flashes type 8 followed by type 7)
			{
				// Handle rootfs (type 8, variable offset, variable length)
				for (i = 0; i < MAX_PART_NUMBER; i++)
				{
					if (t_has[i] == 8)  // rootfs
					{
						if (tableAddr[8].Offset == 0x7f)
						{
							Offset[8] = Offset[6] + Size[6];  // kernel offset + kernel length
						}
						else
						{
							Offset[8] = tableAddr[8].Offset;
						}
						if (tableAddr[8].Size == 0x7f)
						{
							Size[8] = ucLen[8];
						}
						else
						{
							Size[8] = tableAddr[8].Size;
						}
					}
				}
				// Handle dev (type 7, variable offset & length)
				for (i = 0; i < MAX_PART_NUMBER; i++)
				{
					if (t_has[i] == 7)  // dev
					{
						if (tableAddr[7].Offset == 0x7f)
						{
							Offset[7] = Offset[8] + Size[8];  // rootfs offset + rootfs length
						}
						else
						{
							Offset[7] = tableAddr[7].Offset;
						}
						if (tableAddr[7].Size == 0x7f)
						{
							Size[7] = ucLen[7];
						}
						else
						{
							Size[7] = tableAddr[7].Size;  // dev length
						}
					}
				}
			}
			// Handle app (type 1, variable offset & length)
			for (i = 0; i < MAX_PART_NUMBER; i++)
			{
				if (t_has[i] == 1)  // app
				{
					if (tableAddr[1].Offset == 0x7f)
					{
						Offset[t_has[i]] = Offset[7] + ucLen[7];  // dev offset + devfs length
					}
					else
					{
						Offset[t_has[i]] = tableAddr[t_has[i]].Offset;
					}
					if (tableAddr[t_has[i]].Size == 0x7f)
					{
						Size[t_has[i]] = ucLen[t_has[i]];  // app length
					}
					else
					{
						Size[t_has[i]] = tableAddr[t_has[i]].Size;
					}
				}
			}
		}

		// Step three: display info
		printf("\nPartition data (order as in file):\n");
		printf("  Type mtd  mtdname mtd start  mtd end    mtd size   size(file) FS     flash signed\n");
		printf("  =================================================================================\n");

		for (i = 0; i < MAX_PART_NUMBER; i++)
		{
			if (t_has[i] != 0 && (t_has[i] < MAX_PART_NUMBER || t_has[i] == 0x10))
			{
				t_has[i] = (t_has[i] == 0x10 ? 0 : t_has[i]);
				printf("    %d", t_has[i]);
				printf("  %s", tableAddr[t_has[i]].Extension2);
				printf(" %s", tableAddr[t_has[i]].Description);
				for (j = 0; j < (7 - strlen(tableAddr[t_has[i]].Description)); j++)
				{
					printf(" ");
				}
				if ((generation == 2 && (resellerId & 0xf0) == 0xa0)
				||  (generation == 5 && (resellerId & 0xff) == 0xa5)
				||  (generation == 5 && (resellerId & 0xff) == 0xaa))
				{  // Loader 6.XX: variable offsets
					printf(" 0x%08X 0x%08X 0x%08X 0x%08X", Offset[t_has[i]], (Offset[t_has[i]] + Size[t_has[i]] - 1), Size[t_has[i]], ucLen[i]);
				}
				else
				{
					printf(" 0x%08X", tableAddr[t_has[i]].Offset);
					printf(" 0x%08X", tableAddr[t_has[i]].Offset + tableAddr[t_has[i]].Size - 1);
					printf(" 0x%08X", tableAddr[t_has[i]].Size);
					printf(" 0x%08X", ucLen[t_has[i]]);
				}
				printf(" %s", tableAddr[t_has[i]].FStype);
				for (j = 0; j < (8 - strlen(tableAddr[t_has[i]].FStype)); j++)
				{
					printf(" ");
				}
				printf(" %s", (tableAddr[t_has[i]].Flags & PART_FLASH) ? "Y" : "N");
				printf("     %s", (tableAddr[t_has[i]].Flags & PART_SIGN) ? "Y" : "N");
				printf("\n");
			}
		}
		if (generation == 5)
		{
			if (((SWVersion >> 16) & 0xff) < 3)  // Version 3 and up flash to NOR
			{
				printf("  Note: partition data will be flashed to NAND flash memory");
				if (loaderFound == 2)
				{
					printf(";\n       but loader partition to NOR flash memory.\n");
				}
				else
				{
					printf(".\n");
				}
			}
			else
			{
				printf("  Note: partition data will be flashed to NOR flash memory.\n");
			}
		}
		if ((generation == 2 && (resellerId & 0xf0) == 0xa0)
		||  (generation == 5 && (resellerId & 0xff) == 0xa5)
		||  (generation == 5 && (resellerId & 0xff) == 0xaa))
		{
			printf("  Note: start addresses and sizes are multiples of erase size (0x%X).\n", ERASE_SIZE);
		}
		printf("\n");
	}
	else if ((argc == 2 && strlen(argv[1]) == 2 && strncmp(argv[1], "-d", 2) == 0)
	     ||  (argc == 2 && strlen(argv[1]) == 3 && strncmp(argv[1], "-dv", 3) == 0))
	{   // -d(v): create a dummy squashfs file
		if (strncmp(argv[1], "-dv", 3) == 0)
		{
			verbose = 1;
		}
		else
		{
			verbose = 0;
		}
		createDummy();
	}
	else if ((argc == 3 && strlen(argv[1]) == 2 && strncmp(argv[1], "-s", 2) == 0)
	     ||  (argc == 3 && strlen(argv[1]) == 3 && strncmp(argv[1], "-sv", 3) == 0))  // sign squashfs part
	{   // -s(v): sign squashfs part
		/*
		 * Caution: the CRC calculation is not done over an entire partition
		 *          part, but is done only on every 10000th (DATA_BUFFER_SIZE)
		 *          byte in the partition.
		 *          This means that the buffer size DATA_BUFFER_SIZE will
		 *          influence the results!
		 * Therefore: leave the DATA_BUFFER_SIZE value always at 10000.
		 */
		uint32_t crc = 0;
		char signedFileName[128];
		uint8_t buffer[DATA_BUFFER_SIZE];
		size_t count;
		FILE *inFile;
		FILE *signedFile;

		if (strncmp(argv[1], "-sv", 3) == 0)
		{
			verbose = 1;
		}
		else
		{
			verbose = 0;
		}

		inFile = fopen(argv[2], "r");
		if (inFile == NULL)
		{
			printf("ERROR: cannot open input file %s.\n", argv[2]);
			return -1;
		}

		strcpy(signedFileName, argv[2]);
		strcat(signedFileName, ".signed");
		signedFile = fopen(signedFileName, "wb");
		if (signedFile == NULL)
		{
			printf("ERROR: cannot open output file %s.\n", argv[3]);
			return -1;
		}

		while (!feof(inFile))
		{
			count = fread(buffer, 1, DATA_BUFFER_SIZE, inFile);
			fwrite(buffer, 1, count, signedFile);
			crc = crc32(crc, buffer, 1);  // ! 1 byte, not DATA_BUFFER_SIZE (probably an old bug at Fortis)
		}
		fseek(inFile, 0L, SEEK_END);
		count = ftell(inFile);
		fclose(inFile);

		if (verbose == 1)
		{
			printf("Input file size is %d (0x%X) bytes\n", count, count);
			printf("Output file name is: %s\n", signedFileName);
		}

		fwrite(&crc, 1, 4, signedFile);  // append the CRC to the output file
		fseek(signedFile, 0L, SEEK_END);
		count = ftell(signedFile);
		if (verbose == 1)
		{
			printf("Signature in footer: 0x%04X\n", crc);
			printf("Output file size is: %d (0x%X) bytes\n", count, count);
			printf("Output file name is: %s\n", signedFileName);
		}
		fclose(signedFile);
	}
	else if ((argc == 3 && strlen(argv[1]) == 2 && strncmp(argv[1], "-t", 2) == 0)
	     ||  (argc == 3 && strlen(argv[1]) == 3 && strncmp(argv[1], "-tv", 3) == 0))
	{  // -t(v): check signed squashfs part signature,
		// see note above on peculiar CRC determination
		uint32_t crc = 0;
		uint32_t orgcrc = 0;
		uint8_t buffer[DATA_BUFFER_SIZE];
		FILE *inFile;
		size_t count;

		if (strncmp(argv[1], "-tv", 3) == 0)
		{
			verbose = 1;
		}
		else
		{
			verbose = 0;
		}

		inFile = fopen(argv[2], "r");
		if (inFile == NULL)
		{
			printf("ERROR: cannot open input file %s.\n", argv[2]);
			return -1;
		}

		while (!feof(inFile))
		{
			count = fread(buffer, 1, DATA_BUFFER_SIZE, inFile);
			if (count != DATA_BUFFER_SIZE)  // if last block, get CRC
			{
				orgcrc = (buffer[count - 1] << 24) + (buffer[count - 2] << 16) + (buffer[count - 3] << 8) + (buffer[count - 4]);
			}
			crc = crc32(crc, buffer, 1);  // ! 1 byte, not DATA_BUFFER_SIZE (probably an old bug at Fortis)
		}
		if (verbose == 1)
		{
			fseek(inFile, 0L, SEEK_END);
			count = ftell(inFile);
			printf("File size: %d (0x%X) bytes\n", count, count);
		}
		fclose(inFile);

		if (verbose == 1)
		{
			printf("Correct signature: 0x%08X\n", crc);
			printf("Signature in file: 0x%08X (%s)\n", orgcrc, (crc == orgcrc ? "OK" : "wrong!"));
		}
		else
		{
			if (crc != orgcrc)
			{
				printf("Signature is wrong, correct: 0x%08X, found in file: 0x%08X.\n", crc, orgcrc);
				return -1;
			}
		}
	}
	else if ((argc == 3 && strlen(argv[1]) == 2 && strncmp(argv[1], "-x", 2) == 0)
	     ||  (argc == 3 && strlen(argv[1]) == 3 && strncmp(argv[1], "-xv", 3) == 0))
	{  // -x(v): extract IRD into composing parts
		int32_t i;
		int32_t pos = 0;
		uint16_t len = 0;
		uint8_t firstBlock;
		FILE *file;

		if (strncmp(argv[1], "-xv", 3) == 0)
		{
			verbose = 1;
		}
		else
		{
			verbose = 0;
		}

		for (i = 0; i < MAX_PART_NUMBER; i++)
		{
			has[i] = 0;
			fd[i] = NULL;
		}

		file = fopen(argv[2], "r");
		if (file == NULL)
		{
			printf("ERROR: cannot open input file %s.\n", argv[2]);
			return -1;
		}
		firstBlock = 1;

		while (!feof(file))
		{
			pos = ftell(file);
			len = readBlock(file, argv[2], firstBlock, 1, verbose);
			firstBlock = 0;
			if (len < 0)
			{
				fclose(file);
				return -1;  // error ocurred
			}
			if (len > 0)
			{
				pos += len + 2;
				fseek(file, pos, SEEK_SET);
			}
			else
			{
				break;
			}
		}
		fclose(file);

		for (i = 0; i < MAX_PART_NUMBER; i++)
		{
			if (fd[i] != NULL)
			{
				fclose(fd[i]);
			}
		}
		printVerbose("\n");
//		for (i = 0; i < MAX_PART_NUMBER; i++)
//		{
//			if (has[i] > 1)
//			{
//				printf("CAUTION, unusual condition: partition type %d occurs %d times.\n", i, has[i]);
//			}
//		}
		if (verbose)
		{
			printf("Extracting IRD file %s succesfully completed.\n", argv[2]);
		}
		verbose = 1;
	}
	else if (argc == 2 && strlen(argv[1]) == 2 && strncmp(argv[1], "-v", 2) == 0)
	{  // -v: print version info
		printf("Version: %s  Date: %s\n", VERSION, DATE);
	}
	else if (argc >= 3 && strlen(argv[1]) == 2 && strncmp(argv[1], "-c", 2) == 0)
	{  // -c: create standard Fortis IRD
		int32_t i;
		uint32_t resellerId = 0;
		uint32_t SWVersion = 0;
		uint16_t headerDataBlockLen;
		uint32_t totalBlockCount;
		uint16_t type;
		uint8_t *dataBuf;
		uint8_t appendPartCount;
		uint16_t partBlockcount;
		FILE *infile;
		FILE *irdFile;
		uint16_t temp;

		irdFile = fopen(argv[2], "wb+");
		if (irdFile == NULL)
		{
			printf("ERROR: cannot open output file %s.\n", argv[2]);
			return -1;
		}

		totalBlockCount = 0;
		headerDataBlockLen = 0;

		// construct header
#if 0 //not defined USE_ZLIB
		type = 0x01;
#else
		type = 0x10;
#endif
		// Header
		headerDataBlockLen = writeBlock(irdFile, NULL, 1, type);  // input file = NULL, first block
		headerDataBlockLen += 4;  // allow for length and CRC

		appendPartCount = argc;
		// search for -v
		verbose = 0;
		for (i = 3; i < argc; i++)
		{
			if (strlen(argv[i]) == 2 && (strncmp(argv[i], "-v", 2)) == 0)
			{
				verbose = 1;
			}
		}
		appendPartCount = appendPartCount - verbose - 3;

		if (appendPartCount == 0)
		{
			printf("\nERROR: No input files for output file %s specified.\n", argv[2]);
			fclose(irdFile);
			remove(argv[2]);
			return -1;
		}
		if (verbose)
		{
			printf("Creating flash file %s\n", argv[2]);
		}

		// evaluate suboptions
		for (i = 3; i < appendPartCount + 3 + verbose; i += 2)
		{
			type = 0x99;

			if ((strlen(argv[i]) == 3 && (strncmp(argv[i], "-ll", 3) && strncmp(argv[i], "-00", 3)) == 0)
			||  (strlen(argv[i]) == 13 && (strncmp(argv[i], "-feelinglucky", 13)) == 0))
			{
				type = 0x00;
			}
			else if (strlen(argv[i]) == 2 && (strncmp(argv[i], "-a", 2) && strncmp(argv[i], "-1", 2)) == 0)
			{
				type = 0x01;
			}
			else if ((strlen(argv[i]) == 3 && (strncmp(argv[i], "-c0", 3)) == 0)
			||       (strlen(argv[i]) == 2 && (strncmp(argv[i], "-2", 2)) == 0))
			{
				type = 0x02;
			}
			else if ((strlen(argv[i]) == 3 && (strncmp(argv[i], "-c4", 3)) == 0)
			||       (strlen(argv[i]) == 2 && (strncmp(argv[i], "-3", 2)) == 0))
			{
				type = 0x03;
			}
			else if ((strlen(argv[i]) == 3 && (strncmp(argv[i], "-c8", 3)) == 0)
			||       (strlen(argv[i]) == 2 && (strncmp(argv[i], "-4", 2)) == 0))
			{
				type = 0x04;
			}
			else if ((strlen(argv[i]) == 3 && (strncmp(argv[i], "-ca", 3)) == 0)
			||       (strlen(argv[i]) == 2 && (strncmp(argv[i], "-5", 2)) == 0))
			{
				type = 0x05;
			}
			else if (strlen(argv[i]) == 2 && (strncmp(argv[i], "-k", 2) && strncmp(argv[i], "-6", 2)) == 0)
			{
				type = 0x06;
			}
			else if (strlen(argv[i]) == 2 && (strncmp(argv[i], "-d", 2) && strncmp(argv[i], "-7", 2)) == 0)
			{
				type = 0x07;
			}
			else if (strlen(argv[i]) == 2 && (strncmp(argv[i], "-r", 2) && strncmp(argv[i], "-8", 2)) == 0)
			{
				type = 0x08;
			}
			else if (strlen(argv[i]) == 2 && (strncmp(argv[i], "-u", 2) && strncmp(argv[i], "-9", 2)) == 0)
			{
				type = 0x09;
			}
			else if (strlen(argv[i]) == 2 && strncmp(argv[i], "-A", 2) == 0)
			{
				type = 0x0A;
			}
			else if (strlen(argv[i]) == 2 && strncmp(argv[i], "-B", 2) == 0)
			{
				type = 0x0B;
			}
			else if (strlen(argv[i]) == 2 && strncmp(argv[i], "-C", 2) == 0)
			{
				type = 0x0C;
			}
			else if (strlen(argv[i]) == 2 && strncmp(argv[i], "-D", 2) == 0)
			{
				type = 0x0D;
			}
			else if (strlen(argv[i]) == 2 && strncmp(argv[i], "-E", 2) == 0)
			{
				type = 0x0E;
			}
			else if (strlen(argv[i]) == 2 && strncmp(argv[i], "-F", 2) == 0)
			{
				type = 0x0F;
			}
			/*********************************************
			 *
			 * Note: uBoot loader source code defines
			 *       types 0x0a through 0x0f also as
			 *       valid, but these have not been in
			 *       use so far. fup can add these to
			 *       a flash file if so required.
			 *
			 * In addition, at least the loader for the
			 * 4G models accepts type 0x10 as write to
			 * to EEPROM (on these models EEPROM is
			 * simulated in NAND, not a physical device).
			 *
			 * This version of fup is not aware of this
			 * fact and does not support it.
			 *
			 */
			else if (strlen(argv[i]) == 2 && (strncmp(argv[i], "-i", 2)) == 0)
			{
				type = 0x81;
			}
			else if (strlen(argv[i]) == 2 && (strncmp(argv[i], "-s", 2)) == 0)
			{
				type = 0x82;
			}
			else if (strlen(argv[i]) == 2 && (strncmp(argv[i], "-v", 2)) == 0)
			{
				type = 0x88;
				i--;
			}

			if (type == 0x99)
			{
				printf("ERROR: Unknown suboption %s.\n", argv[i]);
				fclose(irdFile);
				irdFile = fopen(argv[2], "rb");
				if (irdFile != NULL)
				{
					fclose (irdFile);
					remove(argv[2]);
				}
				return -1;
			}

			if (type == 0x81)  // handle resellerID
			{
				if (((appendPartCount) % 2 == 1) && (argc == i + 1))
				{
					printf("\nERROR: Reseller ID (4 or 8 digits) for suboption %s not specified.\n", argv[i]);
					return -1;
				}
				else
				{
					if (strlen(argv[i + 1]) != 4 && strlen(argv[i + 1]) != 8)
					{
						printf("ERROR: Reseller ID must be 4 or 8 characters long.\n");
						return -1;
					}
					sscanf(argv[i + 1], "%X", &resellerId);

					if (strlen(argv[i + 1]) == 4)
					{
						resellerId = resellerId << 16;
					}
				}
			}

			if (type == 0x82)  // handle SW version
			{
				if (((appendPartCount) % 2 == 1) && (argc == i + 1))
				{
					printf("\nERROR: Software version for suboption %s not specified.\n", argv[i]);
					return -1;
				}
				else
				{
					sscanf(argv[i + 1], "%X", &SWVersion);
				}
			}

			if ((type != 0x88) && (type != 0x81) && (type != 0x82))
			{
				if (((appendPartCount) % 2 == 1) && (argc == i + 1))
				{
					printf("\nERROR: Input file name for suboption %s not specified.\n", argv[i]);
					return -1;
				}

				infile = fopen(argv[i + 1], "rb");
				if (infile == NULL)
				{
					printf("ERROR: cannot open input file %s.\n", argv[i + 1]);
					return -1;
				}
				column = 0;
				if (verbose)
				{
					printf("Adding type %d block, file name: %s.\n", type, argv[i + 1]);
				}
				partBlockcount = totalBlockCount;
				while (writeBlock(irdFile, infile, 0, type) == DATA_BLOCK_SIZE)  // input file = infile, normal block
				{
					totalBlockCount++;
//					printProgress("w");
//					printProgress(".");
				}
				totalBlockCount++;
				partBlockcount = totalBlockCount - partBlockcount;
				if (verbose)
				{
					printf("\nAdded %d blocks, total is now %d blocks.\n", partBlockcount, totalBlockCount);
				}
				fclose(infile);
			}
		}
		// Refresh Header
		dataBuf = (uint8_t *)malloc(headerDataBlockLen);

		// Read Header Data Block
		fseek(irdFile, 0x04, SEEK_SET);
		temp = fread(dataBuf, 1, headerDataBlockLen, irdFile);
		if (temp != headerDataBlockLen)
		{
			printf("ERROR: Reading back header failed (-c command).\n");
			return -1;
		}

		// Update Blockcount
		insertint16_t(&dataBuf, 0x08, (int16_t)totalBlockCount & 0xFFFF);
		insertint16_t(&dataBuf, 0x0a, (int16_t)totalBlockCount >> 16);  // zero in practice

		setHeader(irdFile, headerDataBlockLen, dataBuf);

		if (resellerId)
		{
			changeResellerID(irdFile, resellerId);
		}

		if (SWVersion)
		{
			changeSWVersion(irdFile, SWVersion);
		}
		fclose(irdFile);

		if (verbose)
		{
			printf("Creating IRD file %s succesfully completed.\n", argv[2]);
		}
		verbose = 1;
	}
	else if (argc >= 3 && strlen(argv[1]) == 3 && strncmp(argv[1], "-ce", 3) == 0)
	{  // -ce: Create Enigma2 IRD (for TDT Maxiboot loader, also used for Neutrino)
		int32_t i;
		uint32_t resellerId = 0;
		uint32_t SWVersion = 0;
		uint16_t type;
		uint16_t totalBlockCount;
		uint16_t headerDataBlockLen;
		uint8_t appendPartCount;
		uint16_t partBlockcount;
		uint8_t oldsquash = 0;
		uint8_t *dataBuf;
		FILE *file;
		FILE *irdFile;
		size_t count;

		verbose = 0;
		irdFile = fopen(argv[2], "wb+");
		if (irdFile == NULL)
		{
			printf("ERROR: cannot open output file %s.\n", argv[2]);
			return -1;
		}

		totalBlockCount = 0;
		headerDataBlockLen = 0;

		// Header
		headerDataBlockLen = writeBlock(irdFile, NULL, 1, 0x10);
		headerDataBlockLen += 4;

		appendPartCount = argc;
		// search for -v and -1G
		for (i = 3; i < argc; i++)
		{
			if (strlen(argv[i]) == 2 && (strncmp(argv[i], "-v", 2)) == 0)
			{
				verbose = 1;
			}
		}
		for (i = 3; i < argc; i++)
		{
			if (strlen(argv[i]) == 3 && (strncmp(argv[i], "-1G", 3)) == 0)
			{
				oldsquash = 1;
			}
		}
		appendPartCount = argc - verbose - oldsquash - 3;

		if (appendPartCount == 0)
		{
			printf("\nERROR: No input files for output file %s specified.\n",argv[2]);
			fclose(irdFile);
			remove(argv[2]);
			return -1;
		}
		if (verbose)
		{
			printf("Creating flash file %s\n", argv[2]);
		}
		for (i = 3; i < appendPartCount + 3 + verbose + oldsquash; i += 2)
		{
			type = 0x99;

			if (strlen(argv[i]) == 2 && (strncmp(argv[i], "-f", 2) && strncmp(argv[i], "-1", 2) == 0))
			{  // Original APP, now FW
				type = 0x01;
			}
			else if (strlen(argv[i]) == 2 && (strncmp(argv[i], "-k", 2) && strncmp(argv[i], "-6", 2)) == 0)
			{  // KERNEL
				type = 0x06;
			}
			else if (strlen(argv[i]) == 2 && (strncmp(argv[i], "-e", 2) && strncmp(argv[i], "-8", 2)) == 0)
			{  // Original ROOT, now EXT
				type = 0x08;
			}
			else if (strlen(argv[i]) == 2 && (strncmp(argv[i], "-g", 2) && strncmp(argv[i], "-7", 2)) == 0)
			{  // Original DEV, now G
				type = 0x07;
			}
			else if (strlen(argv[i]) == 2 && (strncmp(argv[i], "-r", 2) && strncmp(argv[i], "-9", 2)) == 0)
			{  // Original USER, now ROOT
				type = 0x09;
			}
			else if (strlen(argv[i]) == 2 && strncmp(argv[i], "-2", 2) == 0)
			{
				type = 0x02;
			}
			else if (strlen(argv[i]) == 2 && strncmp(argv[i], "-3", 2) == 0)
			{
				type = 0x03;
			}
			else if (strlen(argv[i]) == 2 && strncmp(argv[i], "-4", 2) == 0)
			{
				type = 0x04;
			}
			else if (strlen(argv[i]) == 2 && strncmp(argv[i], "-5", 2) == 0)
			{
				type = 0x05;
			}
			else if (strlen(argv[i]) == 2 && strncmp(argv[i], "-i", 2) == 0)
			{
				type = 0x81;
			}
			else if (strlen(argv[i]) == 2 && strncmp(argv[i], "-s", 2) == 0)
			{
				type = 0x82;
			}
			else if (strlen(argv[i]) == 3 && strncmp(argv[i], "-1G", 3) == 0)
			{
				type = 0x88;
				i--;
			}
			else if (strlen(argv[i]) == 2 && strncmp(argv[i], "-v", 2) == 0)
			{
				type = 0x88;
				i--;
			}

			if (type == 0x99)
			{
				printf("Unknown suboption %s.\n", argv[i]);
				fclose(irdFile);
				irdFile = fopen(argv[2], "rb");
				if (irdFile != NULL)
				{
					fclose (irdFile);
					remove(argv[2]);
				}
				return -1;
			}
			if (type == 0x81)  // check resellerID option for argument
			{
				if (((appendPartCount) % 2 == 1) && (argc == i + 1))
				{
					printf("\nERROR: Reseller ID (4 or 8 digits) for suboption %s not specified.\n", argv[i]);
					return -1;
				}
				else
				{
					if (strlen(argv[i + 1]) != 4 && strlen(argv[i + 1]) != 8)
					{
						printf("ERROR: Reseller ID must be 4 or 8 characters long.\n");
						return -1;
					}
					sscanf(argv[i + 1], "%X", &resellerId);

					if (strlen(argv[i + 1]) == 4)
					{
						resellerId = resellerId << 16;
					}
				}
			}

			if (type == 0x82)  // check SW version option for argument
			{
				if (((appendPartCount) % 2 == 1) && (argc == i + 1))
				{
					printf("\nERROR: Software version for suboption %s not specified.\n", argv[i]);
					return -1;
				}
				else
				{
					sscanf(argv[i + 1], "%X", &SWVersion);
				}
			}

			if ((type != 0x88) && (type != 0x81) && (type != 0x82))
			{
				if (verbose)
				{
					printf("\nNew partition, type %02X\n", type);
				}
				if (((appendPartCount) % 2 == 1) && (argc == i + 1))
				{
					printf("\nERROR: Input file name for option %s not specified.\n",argv[i]);
					return -1;
				}
// TODO: use tables
				if (type == 0x01 || type == 0x08 || type== 0x07)  // these must be signed squashfs
				{
					if (oldsquash == 1)
					{
						// Check if the file dummy.squash30.signed.padded exists. If not, create it
						file = fopen("dummy.squash30.signed.padded", "rb");
						if (file == NULL)
						{
							printVerbose("\nSigned dummy squashfs 3.0 headerfile does not exist.\n");
							printVerbose("Creating it: ");

							file = fopen("dummy.squash30.signed.padded", "w");
							printProgress("o");

							fwrite(dummy30, 1, dummy30_size, file);
							printProgress("w");

							fclose(file);
							printProgress("c");

							file = fopen("dummy.squash30.signed.padded", "rb");
							printProgress(".\n");
							if (file == NULL)
							{
								printf("\nERROR: Could not write signed dummy squashfs3.0 header file.\n");
								remove("dummy.squash30.signed.padded");
								return -1;
							}
							else
							{
								printVerbose("Creating signed dummy squashfs3.0 header file successfully completed.\n\n");
							}
						}
						printVerbose("Adding signed dummy squashfs3.0 header\n");
					}
					else  // new squashfs dummy
					{
						createDummy();  //  Create dummy squashfs header
						printVerbose("Adding signed dummy squashfs header.\n");
						file = fopen("dummy.squash.signed.padded", "rb");
						printProgress(".");
					}

					if (file != NULL)
					{
						partBlockcount = totalBlockCount;
						while (writeBlock(irdFile, file, 0, type) == DATA_BLOCK_SIZE)
						{
							printProgress(".");
							totalBlockCount++;
						}
						totalBlockCount++;
						fclose(file);
// 						printVerbose("Dummy squashfs header written to output file.\n");
 						if (verbose)
						{
							printf("\nAdded %d blocks, total is now %d blocks.\n", totalBlockCount - partBlockcount, totalBlockCount);
						}
					}
					else
					{
						printf("\nCould not read signed dummy squashfs header file.\n");
						remove("dummy.squash.signed.padded");
						return -1;
					}
				}  // squash header added, test if something more to add to this partition
				if ((strlen(argv[i + 1]) == 3 && strncmp(argv[i + 1], "foo", 3) == 0)
				||  (strlen(argv[i + 1]) == 5 && strncmp(argv[i + 1], "dummy", 5) == 0))
				{
					printVerbose("This is a foo partition (squashfs dummy header only).\n");
				}
				else
				{  // append input file
					file = fopen(argv[i + 1], "rb");
					if (file != NULL)
					{
						if (verbose)
						{
							printf("Adding type %d block, file: %s\n", type, argv[i + 1]);
						}
						partBlockcount = totalBlockCount;
						while (writeBlock(irdFile, file, 0, type) == DATA_BLOCK_SIZE)
						{
							totalBlockCount++;
							printProgress(".");
						}
						totalBlockCount++;
						partBlockcount = totalBlockCount - partBlockcount;
						if (verbose)
						{
							printf("\nAdded %d blocks, total is now %d blocks.\n", partBlockcount, totalBlockCount);
						}
						fclose(file);
					}
					else
					{
						printf("\nERROR: cannot not append input file %s.\n", argv[i + 1]);
						printf("\n");
					}
				}
			}
 		}
		// Refresh Header
		dataBuf = (uint8_t *)malloc(headerDataBlockLen);

		// Read Header Data Block
		fseek(irdFile, 0x04, SEEK_SET);
		count = fread(dataBuf, 1, headerDataBlockLen, irdFile);
		if (count != headerDataBlockLen)
		{
			printf("ERROR: Reading back header failed (-ce command).\n");
			return -1;
		}

		// Update Blockcount
		insertint16_t(&dataBuf, 0x08, (int16_t)totalBlockCount & 0xFFFF);
		insertint16_t(&dataBuf, 0x0a, (int16_t)totalBlockCount >> 16);  // zero in practice

		setHeader(irdFile, headerDataBlockLen, dataBuf);

		if (resellerId)
		{
			changeResellerID(irdFile, resellerId);
		}

		if (SWVersion)
		{
			changeSWVersion(irdFile, SWVersion);
		}
		fclose(irdFile);

		if (verbose)
		{
			printf("Creating IRD file %s succesfully completed.\n", argv[2]);
		}
		verbose = 1;

		if (oldsquash)
		{
			file = fopen("dummy.squash30.signed.padded", "rb");
			if (file != NULL)
			{
				fclose(file);
				remove("dummy.squash30.signed.padded");
//				printVerbose("File dummy.squash30.signed.padded deleted.\n");
			}
			else
			{
				printf("ERROR: Cannot delete file dummy.squash30.signed.padded.\n");
			}
		}
		else
		{
			deleteDummy();
		}
		oldsquash = 0;
	}
	else if ((argc == 4 && strlen(argv[1]) == 2 && strncmp(argv[1], "-r", 2) == 0)
	     ||  (argc == 4 && strlen(argv[1]) == 3 && strncmp(argv[1], "-rv", 3) == 0))
	{  // -r(v): Change reseller ID
		uint32_t resellerId;
		FILE *irdFile;

		if (strncmp(argv[1], "-rv", 3) == 0)
		{
			verbose = 1;
		}
		else
		{
			verbose = 0;
		}
		resellerId = 0;

		irdFile = fopen(argv[2], "r+");
		if (irdFile == NULL)
		{
			printf("ERROR: cannot open IRD file %s.\n", argv[2]);
			return -1;
		}
		if (strlen(argv[3]) != 4 && strlen(argv[3]) != 8)
		{
			printf("ERROR: Reseller ID must be 4 or 8 characters long.\n");
			fclose(irdFile);
			return -1;
		}
		sscanf(argv[3], "%X", &resellerId);
		if (strlen(argv[3]) == 4)
		{
			resellerId = (resellerId << 16) | (getresellerID(irdFile) & 0x0000ffff);
		}
		changeResellerID(irdFile, resellerId);
		fclose(irdFile);
	}
	else if ((argc == 4 && strlen(argv[1]) == 3 && strncmp(argv[1], "-rl", 3) == 0)
	     ||  (argc == 4 && strlen(argv[1]) == 4 && strncmp(argv[1], "-rlv", 4) == 0))
	{  // -rl(v): Change loader reseller ID
		uint32_t resellerId;
		uint32_t SWVersion = 0;
		FILE *irdFile;
		FILE *loaderFile;
		uint8_t *dataBuffer;
		uint32_t resellerIdL;
		uint16_t generation;
		uint8_t firstBlock;
		uint16_t totalBlockCount;
		uint16_t partBlockcount;
		uint16_t headerDataBlockLen;
		int32_t pos = 0;
		int32_t i;
		uint16_t len = 0;
		struct tPartition *tableAddr;
		int32_t filesize;
		uint8_t *dataBufL;

		resellerId = 0;

		irdFile = fopen(argv[2], "r");
		if (irdFile == NULL)
		{
			printf("ERROR: cannot open input file %s.\n", argv[2]);
			return -1;
		}
		if (strncmp(argv[1], "-rlv", 4) == 0)
		{
			verbose = 1;
		}
		else
		{
			verbose = 0;
		}
		resellerId = getresellerID(irdFile);
		generation = getGeneration(resellerId);
		SWVersion = getSWversion(irdFile);
		if (verbose)
		{
			printf("Reseller ID of file %s is %08X.\n", argv[2], resellerId);
			printf("Reseller model       : %s\n", getModelName(resellerId));
			printf("Software version     : V%X.%02X.%02X\n", SWVersion >> 16, (SWVersion & 0x0000FF00) >> 8, SWVersion & 0xFF);
		}
		dataBuffer = getHeader(irdFile);
		fseek(irdFile, 0, SEEK_SET);
		partcount = 0;
		loaderFound = 0;
		scanBlocks(irdFile);

		// Check if .ird file contains a loader partition
		if (loaderFound != 2)
		{
			printf("ERROR: This IRD file does not contain a loader partition.\n");
			return -1;
		}
		if (verbose)
		{
			printf("IRD file contains a loader partition.\n");
			printf("Loader version is    : V%X.%02X.%02X\n", (loaderSW >> 16) & 0xff, (loaderSW >> 8) & 0xff, loaderSW & 0xff);
			printf("Loader reseller ID is: %08X ", loaderId);
			if (loaderId == resellerId)
			{
				printf("(same model)");
			}
			else
			{
				printf ("(%s)", getModelName(loaderId));
			}
			printf("\n");
		}
		// Check if loader is compatible with hardware matching the IRD file's reseller ID
		if ((resellerId >> 24 != loaderId >> 24)
		||  (resellerId >>  8 != loaderId >>  8))
		{
			if ((generation == 2 || generation == 3 || generation == 5) && (resellerId & 0xff00ff5f != loaderId & 0xff00ff5f)
			||  (generation != 2 && generation != 3 && generation != 5) && (resellerId & 0xff00ffff != loaderId & 0xff00ffff))
			{
				printf("ERROR: Loader in IRD file is not compatible with hardware indicated by the file's reseller ID.\n");
				printf("       Do NOT flash this file!\n");
				return -1;
			}
		}
		if (strlen(argv[3]) != 4 && strlen(argv[3]) != 8)
		{
			printf("ERROR: Loader reseller ID must be 4 or 8 characters long.\n");
			return -1;
		}
		sscanf(argv[3], "%X", &resellerIdL);
		if (strlen(argv[3]) == 4)
		{
			resellerIdL = (resellerIdL << 16) | (loaderId & 0x0000ffff);
		}
		// Check if specified loader reseller ID differs only in the 2nd byte of the file's reseller ID
		if (generation == 2 || generation == 3 || generation == 5)
		{
				if ((resellerIdL & 0xff00ff5f) != (loaderId & 0xff00ff5f))
				{
					printf("ERROR: Specified loader reseller ID and IRD file's reseller ID are not compatible.\n");
					printf("       The only difference allowed is the value of the 2nd byte and/or the 7th nibble (A or 0).\n");
					return -1;
				}
		}
		else if	(resellerIdL & 0xff00ffff != loaderId & 0xff00ffff)
		{
			printf("ERROR: Specified loader reseller ID and IRD file's reseller ID are not compatible.\n");
			printf("       The only difference allowed is the value of the 2nd byte.\n");
			return -1;
		}
		if (verbose)
		{
			printf("\nChanging loader reseller ID in IRD file %s\n", argv[2]);
			printf("from %08X (%s)\n", loaderId, getModelName(loaderId));
			printf("  to %08X (%s).\n\n", resellerIdL, getModelName(resellerIdL));
		}
		// Reopen IRD file and unpack it in its binary components
		for (i = 0; i < MAX_PART_NUMBER; i++)
		{
			has[i] = 0;  // clear partition indicator flag
			fd[i] = NULL;  // and file name
		}
		irdFile = fopen(argv[2], "r");
//		if (irdFile == NULL)
//		{
//			printf("ERROR: cannot open input file %s.\n", argv[2]);
//			return -1;
//		}
		firstBlock = 1;

		while (!feof(irdFile))
		{
			pos = ftell(irdFile);
			len = readBlock(irdFile, argv[2], firstBlock, 1, verbose);
			firstBlock = 0;
			if (len < 0)
			{
				fclose(irdFile);
				return -1;  // error ocurred
			}
			if (len > 0)
			{
				pos += len + 2;
				fseek(irdFile, pos, SEEK_SET);
			}
			else
			{
				break;
			}
		}
		fclose(irdFile);  // close input file

		// close output files
		for (i = 0; i < MAX_PART_NUMBER; i++)
		{
			if (fd[i] != NULL)
			{
				fclose(fd[i]);
			}
		}
		printVerbose("\n");
//		for (i = 0; i < MAX_PART_NUMBER; i++)
//		{
//			if (has[i] > 1)
//			{
//				printf("CAUTION, unusual condition: partition type %d occurs %d times.\n", i, has[i]);
//			}
//		}
		if (verbose)
		{
			printf("Extracting IRD file %s succesfully completed.\n", argv[2]);
		}

		// open output file with loader binary and change the reseller ID
		loaderFile = fopen(nameOut[0], "r+");
		fseek(loaderFile, 0L, SEEK_END);
		filesize = ftell(loaderFile);

		// Read bootloader file
		dataBufL = (uint8_t *)malloc(filesize);
		fseek(loaderFile, 0, SEEK_SET);
		i = fread(dataBufL, 1, filesize, loaderFile);

		if (i != filesize)
		{
			printf("ERROR: Reading loader file failed.\n");
			return -1;
		}
		fclose(loaderFile);
		remove(argv[2]);

		// determine offset into buffer
		switch (generation)
		{
			case 1:
			{
				i = RESELLER_OFFSET_GEN1;
				break;
			}
			case 2:
			case 3:
			case 5:
			default:
			{
				i = RESELLER_OFFSET_GEN2;
				break;
			}
			case 4:
			{
				i = RESELLER_OFFSET_GEN4;
				break;
			}
		}
		// Update loader Reseller ID
		insertint32_t(&dataBufL, i, resellerIdL);
		loaderFile = fopen(nameOut[0], "wb");
		if (loaderFile == NULL)
		{
			printf("ERROR: cannot open loader partition file %s.\n", nameOut[0]);
			return -1;
		}

		fwrite(dataBufL, 1, filesize, loaderFile);
		fclose(loaderFile);
		free(dataBufL);

		// rewrite IRD file
		irdFile = fopen(argv[2], "w+");

		if (irdFile == NULL)
		{
			printf("ERROR: cannot open output file %s.\n", argv[2]);
			return -1;
		}

		if (verbose)
		{
			printf("Creating new output file %s\n", argv[2]);
		}
		totalBlockCount = 0;
		headerDataBlockLen = 0;

		// construct header
#if 0 //not defined USE_ZLIB
		i = 0x01;
#else
		i = 0x10;
#endif
		headerDataBlockLen = writeBlock(irdFile, NULL, 1, i);  // input file = NULL, first block
		headerDataBlockLen += 4;  // allow for length and CRC

		// Add partitions
		for (i = 0; i < MAX_PART_NUMBER; i++)
		{
			if (has[i])
			{
				fd[i] = fopen(nameOut[i], "rb");
				if (fd[i] == NULL)
				{
					printf("ERROR: cannot open input file %s.\n", nameOut[i]);
					return -1;
				}
				column = 0;
				if (verbose)
				{
					printf("Adding type %d block, file name: %s.\n", i, nameOut[i]);
				}
				partBlockcount = totalBlockCount;
				while (writeBlock(irdFile, fd[i], 0, i) == DATA_BLOCK_SIZE)  // normal block
				{
					totalBlockCount++;
				}
				totalBlockCount++;
				partBlockcount = totalBlockCount - partBlockcount;
				if (verbose)
				{
					printf("\nAdded %d blocks, total is now %d blocks.\n", partBlockcount, totalBlockCount);
				}
				fclose(fd[i]);
			}
		}
		fclose(irdFile);

		// Refresh header
		irdFile = fopen(argv[2], "r+");

		dataBuffer = getHeader(irdFile);

		insertint16_t(&dataBuffer, 0x08, (int16_t)totalBlockCount & 0xFFFF);
		insertint16_t(&dataBuffer, 0x0a, (int16_t)totalBlockCount >> 16);  // zero in practice
		insertint32_t(&dataBuffer, 2, resellerId);
		insertint16_t(&dataBuffer, 0x0c, (SWVersion >> 16));
		insertint16_t(&dataBuffer, 0x0e, (SWVersion & 0x0000FFFF));

		setHeader(irdFile, headerDataBlockLen, dataBuffer);
		fclose(irdFile);

		// Clean up
		for (i = 0; i < MAX_PART_NUMBER; i++)
		{
			if (has[i])
			{
				if (verbose)
				{
					printf("Deleting partition file %s\n", nameOut[i]);
				}
				remove(nameOut[i]);
			}
		}
		if (verbose)
		{
			printf("Changing loader reseller ID in IRD file %s succesfully completed.\n", argv[2]);
		}
	}
	else if ((argc == 4 && strlen(argv[1]) == 2 && strncmp(argv[1], "-n", 2) == 0)
	     ||  (argc == 4 && strlen(argv[1]) == 3 && strncmp(argv[1], "-nv", 3) == 0))
	{  // -n(v): Change SW version number
		uint32_t SWVersion;
		FILE *irdFile;

		if (strncmp(argv[1], "-nv", 3) == 0)
		{
			verbose = 1;
		}
		else
		{
			verbose = 0;
		}

		sscanf(argv[3], "%X", &SWVersion);

		irdFile = fopen(argv[2], "r+");
		if (irdFile == NULL)
		{
			printf("ERROR: cannot open IRD file %s.\n", argv[2]);
			return -1;
		}
		changeSWVersion(irdFile, SWVersion);
		fclose(irdFile);
	}
	else // show usage
	{
		printf("\nfup - management program for Fortis .ird flash files\n");
		printf("\nVersion: %s  Date: %s\n", VERSION, DATE);
		printf("\n");
		printf("Usage: %s -i|-x|-xv|-c|-ce|-s|-sv|-t|-tv|-d|-dv|-r|-rv|-n|-nv|rl|rlv|-v []\n", argv[0]);
		printf("  -i [file.ird]                Display detailed IRD information\n");
		printf("  -x [file.ird]                Extract IRD into composing binaries\n");
		printf("  -xv [file.ird]               As -x, verbose\n");
		printf("  -c [file.ird] Options        Create Fortis IRD\n");
		printf("     Suboptions for -c: NOTE: lettered options and signing info\n");
		printf("                              only correct for 1G and 2G models\n");
		printf("     -ll [file.part]           Append Loader   (0) -> mtd0\n");
		printf("     -k [file.part]            Append Kernel   (6) -> mtd1\n");
		printf("     -a [file.part]            Append App      (1) -> mtd2 (must be signed)\n");
		printf("     -r [file.part]            Append Root     (8) -> mtd3 (must be signed)\n");
		printf("     -d [file.part]            Append Dev      (7) -> mdt4 (must be signed)\n");
		printf("     -c0 [file.part]           Append Config0  (2) -> mtd5, offset 0\n");
		printf("     -c4 [file.part]           Append Config4  (3) -> mtd5, offset 0x40000\n");
		printf("     -c8 [file.part]           Append Config8  (4) -> mtd5, offset 0x80000\n");
		printf("     -ca [file.part]           Append ConfigA  (5) -> mtd5, offset 0xA0000\n");
		printf("     -u [file.part]            Append User     (9) -> mtd6\n");
		printf("     -i [resellerID]           Set resellerID\n");
		printf("     -s [versionnr]            Set SW version\n");
		printf("     -00 [file.part]           Append Type 0   (0) (alias for -ll)\n");
		printf("     -1 [file.part]            Append Type 1   (1) (alias for -a)\n");
		printf("     ...\n");
		printf("     -9  [file.part]           Append Type 9   (9) (alias for -u)\n");
		printf("     -A  [file.part]           Append Type A   (A) (note: upper case)\n");
		printf("     ...\n");
		printf("     -F  [file.part]           Append Type F   (F) (note: upper case)\n");
		printf("     -v                        Verbose operation\n");
		printf("  -ce [update.ird] Options     Create Enigma2 IRD (obsolete, models with TDT Maxiboot only)\n");
		printf("     Subtions for -ce:\n");
		printf("     -k|-6 [file.part]         Append Kernel   (6) -> mtd1\n");
		printf("     -f|-1 [file.part]         Append FW       (1)\n");
		printf("     -r|-9 [file.part]         Append Root     (9)\n");
		printf("     -e|-8 [file.part]         Append Ext      (8)\n");
		printf("     -g|-7 [file.part]         Append G        (7)\n");
		printf("     -i  [resellerID]          Set resellerID\n");
		printf("     -s  [versionnr]           Set SW version\n");
		printf("     -2  [file.part]           Append Config0  (2) -> mtd5, offset 0\n");
		printf("     -3  [file.part]           Append Config4  (3) -> mtd5, offset 0x40000\n");
		printf("     -4  [file.part]           Append Config8  (4) -> mtd5, offset 0x80000\n");
		printf("     -5  [file.part]           Append ConfigA  (5) -> mtd5, offset 0xA0000\n");
		printf("     -1G                       Use squashfs3.0 dummy\n");
		printf("     -v                        Verbose operation\n");
		printf("  -s [unsigned.squashfs]       Sign squashfs part\n");
		printf("  -sv [unsigned.squashfs]      Sign squashfs part, verbose\n");
		printf("  -t [signed.squashfs]         Test signed squashfs part\n");
		printf("  -tv [signed.squashfs]        Test signed squashfs part, verbose\n");
		printf("  -d                           Create squashfs3.3 dummy file\n");
		printf("  -dv                          As -d, verbose\n");
		printf("  -r [file.ird] [resellerID]   Change reseller id (e.g. 230300A0 for Atevio AV7500 L6.00)\n");
		printf("  -rv [file.ird] [resellerID]  As -r, verbose\n");
		printf("  -n [file.ird] [versionnr]    Change SW version number\n");
		printf("  -nv [file.ird] [versionnr]   As -n, verbose\n");
		printf("  -rl [file.ird] [resellerID]  Change reseller id of loader inside file.ird\n");
		printf("  -rlv [file.ird] [resellerID] As -rl, verbose\n");
		printf("  -v                           Display program version\n");
		printf("\n");
		printf("Note: To create squashfs part, use mksquashfs v3.3:\n");
		printf("      ./mksquashfs3.3 squashfs-root flash.rootfs.own.mtd8 -nopad -le\n");
		printf("\n");
		printf("Examples:\n");
		printf("  Creating a Fortis IRD file with rootfs and kernel:\n");
		printf("   %s -c my.ird [-v] -1 flash.rootfs.own.mtd2.signed -6 uimage.mtd6\n", argv[0]);
		printf("\n");
		printf("  Extracting a IRD file:\n");
		printf("   %s -x[v] my.ird\n", argv[0]);
		printf("\n");
		printf("  Signing a squashfs partition:\n");
		printf("   %s -s[v] my.squashfs\n", argv[0]);
		return -1;
	}
	return 0;
}
// vim:ts=4
