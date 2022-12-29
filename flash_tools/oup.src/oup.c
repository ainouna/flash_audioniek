/*****************************************************************************
 * Name :   oup (Opticum update program)                                     *
 *                                                                           *
 *          Management program for Opticum .upd flash files for HD 9600      *
 *          series models:                                                   *
 *          HD 9600          (STx7109, DVB-S(2) only)                        *
 *          HD TS 9600       (STx7109, DVB-S(2) + DVB-T                      *
 *          HD 9600 PRIMA    (STx7105, DVB-S(2) only)                        *
 *          HD 9600 TS PRIMA (STx7105, DVB-S(2) + DVB-T)                     *
 *          HD 9600 MINI     (STx7111, DVB-S(2) only)                        *
 *          HD X550          (STx7100, DVB-S(2) only, not tested)            *
 *          HD X560          (STx7100, DVB-S(2) only, not supported yet)     *
 *                                                                           *
 * Author:  Audioniek, very loosely based on fup by Schischu                 *
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
 * Changes
 *
 * Date     By              Description
 * --------------------------------------------------------------------------
 * 20210825 Audioniek       Initial version.
 * 20211115 Audioniek       Fix typos in usage.
 * 20221228 Audioniek       Minor cleanups, no code changes.
 *
 ****************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "oup.h"
#include "crc32.h"

#define VERSION "1.02"
#define DATE "28.12.2022"

uint32_t verbose;
uint32_t unknown;
struct tHeader header_struct;
uint8_t *header;
uint8_t *payload;
uint8_t hw_version_major;
uint8_t hw_version_minor;

uint32_t getHeader(FILE *updFile)
{
	uint32_t i;
	uint8_t *buf;
	int count = 0;

	// Read Header Data Block
	buf = (uint8_t *)malloc(sizeof(header_struct));
	if (buf == NULL)
	{
		fprintf(stderr, "ERROR: Memory error allocating file buffer\n");
		return -1;
	}
	fseek(updFile, 0x00, SEEK_SET);  // goto beginning of file
	count = fread(buf, 1, sizeof(header_struct), updFile);
	if (count != sizeof(struct tHeader))
	{
		fprintf(stderr, "ERROR: Reading header failed.\n");
		return -1;
	}
	// fill header_struct
	header_struct.file_type = (uint32_t)((buf[3] << 24) + (buf[2] << 16) + (buf[1] << 8) + buf[0]);
	header_struct.header_id = (buf[7] << 24) + (buf[6] << 16) + (buf[5] << 8) + buf[4];
	header_struct.model_code = (buf[11] << 24) + (buf[10] << 16) + (buf[9] << 8) + buf[8];
	header_struct.hw_version_minor = buf[12];
	header_struct.hw_version_major = buf[13];
	header_struct.sw_version_major = buf[14];
	header_struct.sw_version_minor = buf[15];
	header_struct.length = (buf[19] << 24) + (buf[18] << 16) + (buf[17] << 8) + buf[16];
	header_struct.start_addr = (buf[23] << 24) + (buf[22] << 16) + (buf[21] << 8) + buf[20];
	header_struct.crc32 = (buf[27] << 24) + (buf[26] << 16) + (buf[25] << 8) + buf[24];
	header_struct.flag = (buf[31] << 24) + (buf[30] << 16) + (buf[29] << 8) + buf[28];
	free(buf);
	return 0;
}

void setHeader(FILE *updFile)
{
	uint8_t buf[sizeof(header_struct)];

	// reconstruct the header bytes from header_struct
	buf[0]  = (header_struct.file_type >>  0) & 0xff;
	buf[1]  = (header_struct.file_type >>  8) & 0xff;
	buf[2]  = (header_struct.file_type >> 16) & 0xff;
	buf[3]  = (header_struct.file_type >> 24) & 0xff;

	buf[4]  = (header_struct.header_id >>  0) & 0xff;
	buf[5]  = (header_struct.header_id >>  8) & 0xff;
	buf[6]  = (header_struct.header_id >> 16) & 0xff;
	buf[7]  = (header_struct.header_id >> 24) & 0xff;

	buf[8]  = (header_struct.model_code >>  0) & 0xff;
	buf[9]  = (header_struct.model_code >>  8) & 0xff;
	buf[10] = (header_struct.model_code >> 16) & 0xff;
	buf[11] = (header_struct.model_code >> 24) & 0xff;

	buf[12] = header_struct.hw_version_minor;
	buf[13] = header_struct.hw_version_major;
	buf[14]	= header_struct.sw_version_major;
	buf[15] = header_struct.sw_version_minor;

	buf[16] = (header_struct.length >>  0) & 0xff;
	buf[17] = (header_struct.length >>  8) & 0xff;
	buf[18] = (header_struct.length >> 16) & 0xff;
	buf[19] = (header_struct.length >> 24) & 0xff;

	buf[20] = (header_struct.start_addr >>  0) & 0xff;
	buf[21] = (header_struct.start_addr >>  8) & 0xff;
	buf[22] = (header_struct.start_addr >> 16) & 0xff;
	buf[23] = (header_struct.start_addr >> 24) & 0xff;

	buf[24] = (header_struct.crc32 >>  0) & 0xff;
	buf[25] = (header_struct.crc32 >>  8) & 0xff;
	buf[26] = (header_struct.crc32 >> 16) & 0xff;
	buf[27] = (header_struct.crc32 >> 24) & 0xff;

	buf[28] = (header_struct.flag >>  0) & 0xff;
	buf[29] = (header_struct.flag >>  8) & 0xff;
	buf[30] = (header_struct.flag >> 16) & 0xff;
	buf[31] = (header_struct.flag >> 24) & 0xff;

	// Rewrite Header
	fseek(updFile, 0x00, SEEK_SET);  // goto start of file (= start of header)
	fwrite(buf, 1, sizeof(buf), updFile);
}

uint32_t getPayload(FILE *updFile)
{
	uint32_t i;
	uint32_t len;
	uint8_t *buf;

	fseek(updFile, 0x00, SEEK_END);  // goto beginning of file
	len = ftell(updFile);
	payload = (uint8_t *)malloc(len);
	if (payload == NULL)
	{
		fprintf(stderr, "ERROR: Memory error allocating file buffer\n");
		return -1;
	}

	fseek(updFile, sizeof(header_struct), SEEK_SET);  // goto beginning of payload

	i = fread(payload, 1, len - sizeof(header_struct), updFile);
	if (i != len - sizeof(header_struct))
	{
		fprintf(stderr, "ERROR reading input file.\n");
		return -1;
	}
	return i;		
}

const char *getModelName(int model_code)
{
	int32_t i;

	i = 0;

	while (opt9600hdNames[i].model_code != 0 && opt9600hdNames[i].model_code != model_code)
	{
		i++;
	}
	unknown = 0;
	if (opt9600hdNames[i].model_code == 0)
	{
		unknown = 1;
	}
	// set hardware version number defaults
	hw_version_major = opt9600hdNames[i].hw_version >> 8;
	hw_version_minor = opt9600hdNames[i].hw_version & 0xff;
	return opt9600hdNames[i].model_name;
}

int32_t main(int32_t argc, char* argv[])
{
	if (argc == 3 && strlen(argv[1]) == 2 && strncmp(argv[1], "-i", 2) == 0)
	{  // -i: show file info
		int res = -1;
		FILE *updFile;
		uint8_t *buf;
		uint32_t dataCrc;

		updFile = fopen(argv[2], "r");
		if (updFile == NULL)
		{
			fprintf(stderr, "ERROR: Cannot open input file %s.\n", argv[2]);
			return -1;
		}
		res = getHeader(updFile);
		res |= getPayload(updFile);
		dataCrc = 0;
		dataCrc = crc32(dataCrc, payload, header_struct.length);  // get actual payload CRC
		printf("\nInformation on .upd file %s\n\n", argv[2]);
		printf("  File type          : %s\n", (header_struct.file_type == FILE_TYPE_CHANNEL_LIST ? "channel list" : "binary flash"));
//		printf("  Header ID          : %08x\n", header_struct.header_id);
		printf("  Model code         : %08x (%s)\n", header_struct.model_code, getModelName(header_struct.model_code));
		if (header_struct.file_type == FILE_TYPE_FLASH)
		{
			printf("  HW version         : %x.%02x\n", (int)header_struct.hw_version_major & 0xff, (int)header_struct.hw_version_minor & 0xff);
			printf("  SW version         : %1d.%02d\n", (int)((header_struct.sw_version_major & 0xff) >> 4), (int)header_struct.sw_version_minor & 0xff);
		}
		printf("  Payload length     : 0x%08x (%d) bytes\n", header_struct.length, header_struct.length);
		if (header_struct.file_type == FILE_TYPE_FLASH)
		{
			printf("  Flash start address: 0x%08x\n", header_struct.start_addr);
		}
		printf("  CRC32 over payload : 0x%08x (%s)\n", header_struct.crc32, (header_struct.crc32 == dataCrc ? "OK" : "wrong!"));
//		printf("  Flags              : 0x%08x\n", header_struct.flag);
		fclose(updFile);
	}
	else if (argc > 5 && strlen(argv[1]) == 2 && strncmp(argv[1], "-u", 2) == 0)
	{  // -u: create upd flash file
		uint32_t i = 0;
		uint32_t arg_type = 0;
		uint32_t dataCrc = 0;
		uint32_t model_code;
		const char *model_name;
		uint32_t sw_version_major;
		uint32_t sw_version_minor;
		uint32_t flash_offset;
		char *inFileName;
		char *outFileName;
		uint32_t inFilemissing = 1;
		uint32_t outFilemissing = 1;
		uint32_t inFileLen;
		FILE *inFile;
		FILE *outFile;

		// set defaults
		verbose = 0;  // verbose is off
		model_code = MODEL_CODE_HD_9600;
		model_name = getModelName(model_code);  // set default HW version
		sw_version_major = 1;
		sw_version_minor = 0;  // 1.00
		flash_offset = 0x40000;
		// scan arguments given
		for (i = 2; i < argc; i++)
		{
			if (strlen(argv[i]) == 2 && strncmp(argv[i], "-i", 2) == 0) // input file name
			{
				arg_type = 1;
			}
			else if (strlen(argv[i]) == 2 && strncmp(argv[i], "-o", 2) == 0)  // output file name
			{
				arg_type = 2;
			}
			else if (strlen(argv[i]) == 2 && strncmp(argv[i], "-m", 2) == 0)  // model code
			{
				arg_type = 3;
			}
			else if (strlen(argv[i]) == 2 && strncmp(argv[i], "-n", 2) == 0)  // SW version
			{
				arg_type = 4;
			}
			else if (strlen(argv[i]) == 2 && strncmp(argv[i], "-s", 2) == 0)  // flash offset
			{
				arg_type = 5;
			}
			else if (strlen(argv[i]) == 2 && strncmp(argv[i], "-v", 2) == 0)
			{
				arg_type = 0;  // flag no parameter
				verbose = 1;
			}
			else
			{
				fprintf(stderr, "ERROR: Unknown argument %s given.\n", argv[i]);
				return -1;
			}
			if (i < argc - 1 && arg_type != 0)  // at least one more urgument must follow
			{
				i++;  // point to parameter
				switch (arg_type)
				{
					case 1:  // input file name
					{
						if (strlen(argv[i]) == 0)
						{
							fprintf(stderr, "ERROR: Input file name missing.\n");
							return -1;
						}
						inFileName = argv[i];
						inFilemissing = 0;
						break;
					}
					case 2:  // output file name
					{
						if (strlen(argv[i]) == 0)
						{
							fprintf(stderr, "ERROR: Output file name missing.\n");
							return -1;
						}
						outFileName = argv[i];
						outFilemissing = 0;
						break;
					}
					case 3:  // model code
					{
						if (strlen(argv[i]) == 0)
						{
							fprintf(stderr, "ERROR: Model code missing.\n");
							return -1;
						}
						if (strlen(argv[i]) != 8)
						{
							fprintf(stderr, "ERROR: Model code must be 8 characters long.\n");
							return -1;
						}
						sscanf(argv[i], "%X", &model_code);
						model_name = getModelName(model_code);  // set default HW version
						break;
					}
					case 4:  // SW version
					{
						if (strlen(argv[i]) == 0)
						{
							fprintf(stderr, "ERROR: Software version missing.\n");
							return -1;
						}
						if (strlen(argv[i]) != 4)
						{
							fprintf(stderr, "ERROR: SW version must be 4 characters long (format N.NN).\n");
							return -1;
						}
						sscanf(argv[i], "%d.%02d", &sw_version_major, &sw_version_minor);
						if (sw_version_major < 0 || sw_version_major > 9 || sw_version_minor < 0 || sw_version_minor > 99)
						{
							fprintf(stderr, "ERROR: Software version must be between 0.00 and 9.99 (format N.NN)\n");
							return -1;
						}
						break;
					}
					case 5:  // flash offset
					{
						if (strlen(argv[i]) == 0)
						{
							fprintf(stderr, "ERROR: Flash offset missing.\n");
							return -1;
						}
						sscanf(argv[i], "%X", &flash_offset);
						break;
					}
				}
			}
		}
		if (inFilemissing == 1)
		{
			fprintf(stderr, "ERROR: Input file name not specified (use -i)\n");
			return -1;
		}
		if (outFilemissing == 1)
		{
			fprintf(stderr, "ERROR: Output file name not specified (use -o)\n");
			return -1;
		}

		inFile = fopen(inFileName, "r");
		if (inFile == NULL)
		{
			fprintf(stderr, "ERROR: Cannot open input file %s.\n", inFileName);
			return -1;
		}
		fseek(inFile, 0L, SEEK_END);
		inFileLen = ftell(inFile);
		rewind(inFile);

		payload = (uint8_t *)malloc(sizeof(uint8_t) * inFileLen);
		if (payload == NULL)
		{
			fprintf(stderr, "ERROR: Memory error allocating file buffer\n");
			return -1;
		}
		i = fread(payload, 1, inFileLen, inFile);
		if (i != inFileLen)
		{
			fprintf(stderr, "ERROR reading input file %s.\n", inFileName);
			return -1;
		}
		fclose(inFile);

		if (verbose)
		{
			printf("Input file   : %s (length: %x (%d) bytes)\n", inFileName, inFileLen, inFileLen);
			printf("Output file  : %s\n", outFileName);
			printf("Model code   : %08x (%s)\n", model_code, model_name);
			printf("SW version   : %d.%02d\n", sw_version_major, sw_version_minor);
			printf("HW version   : %x.%02x\n", hw_version_major, hw_version_minor);
			printf("Flash offset : 0x%x\n", flash_offset);
			if (unknown)
			{
				printf("\nCAUTION: Unknown model code!\n");
			}
		}

		// compose header
		header_struct.file_type = FILE_TYPE_FLASH;
		header_struct.header_id = HEADER_TYPE_FLASH;
		header_struct.model_code = model_code;
		header_struct.hw_version_major = hw_version_major;
		header_struct.hw_version_minor = hw_version_minor;
		header_struct.sw_version_major = sw_version_major << 4;
		header_struct.sw_version_minor = sw_version_minor;
		header_struct.length = inFileLen;
		header_struct.start_addr = flash_offset;
		dataCrc = 0;
		header_struct.crc32 = crc32(dataCrc, payload, inFileLen);
		header_struct.flag = FLAGS_FLASH;

		outFile = fopen(outFileName, "wb");
		if (outFile == NULL)
		{
			fprintf(stderr, "ERROR: Cannot open output file %s.\n", inFileName);
			return -1;
		}
		setHeader(outFile);

		// add binary payload
		fseek(outFile, sizeof(header_struct), SEEK_SET);
		fwrite(payload, 1, inFileLen, outFile);

		// check if succesfully written
		fseek(outFile, 0L, SEEK_END);
		if (sizeof(header_struct) + inFileLen != ftell(outFile))
		{
			fprintf(stderr, "ERROR: Writing output file %s failed.\n", outFileName);
		}
		else if (verbose)
		{
			printf("Creating file %s succesfully completed.\n", outFileName);
			printf("File length is 0x%x (%d) bytes.\n", sizeof(header_struct) + inFileLen, sizeof(header_struct) + inFileLen);
		}
		fclose(outFile);
		free(payload);
	}
	else if (argc > 5 && strlen(argv[1]) == 2 && strncmp(argv[1], "-c", 2) == 0)
	{  // -c: create .cha flash file
		uint32_t i = 0;
		uint32_t arg_type = 0;
		uint32_t dataCrc = 0;
		uint32_t model_code;
		const char *model_name;
		uint8_t sw_version_major;
		uint8_t sw_version_minor;
		uint32_t flash_offset;
		char *inFileName;
		char *outFileName;
		uint32_t inFilemissing = 1;
		uint32_t outFilemissing = 1;
		uint32_t inFileLen;
		FILE *inFile;
		FILE *outFile;

		// set defaults
		verbose = 0;  // verbose is off
		model_code = MODEL_CODE_HD_9600;
		model_name = getModelName(model_code);

		// scan arguments given
		for (i = 2; i < argc; i++)
		{
			if (strlen(argv[i]) == 2 && strncmp(argv[i], "-i", 2) == 0)	
			{
				arg_type = 1;
			}
			else if (strlen(argv[i]) == 2 && strncmp(argv[i], "-o", 2) == 0)	
			{
				arg_type = 2;
			}
			else if (strlen(argv[i]) == 2 && strncmp(argv[i], "-m", 2) == 0)	
			{
				arg_type = 3;
			}
			else if (strlen(argv[i]) == 2 && strncmp(argv[i], "-v", 2) == 0)	
			{
				arg_type = 0;  // flag no parameter
				verbose = 1;
			}
			else
			{
				fprintf(stderr, "ERROR: Unknown argument %s given.\n", argv[i]);
				return -1;
			}
			if (i < argc - 1 && arg_type != 0)  // at least one more urgument must follow
			{
				i++;  // point to parameter
				switch (arg_type)
				{
					case 1:  // input file name
					{
						if (strlen(argv[i]) == 0)
						{
							fprintf(stderr, "ERROR: Input file name missing.\n");
							return -1;
						}
						inFileName = argv[i];
						inFilemissing = 0;
						break;
					}
					case 2:  // output file name
					{
						if (strlen(argv[i]) == 0)
						{
							fprintf(stderr, "ERROR: Output file name missing.\n");
							return -1;
						}
						outFileName = argv[i];
						outFilemissing = 0;
						break;
					}
					case 3:  // model code
					{
						if (strlen(argv[i]) == 0)
						{
							fprintf(stderr, "ERROR: Model code missing.\n");
							return -1;
						}
						if (strlen(argv[i]) != 8)
						{
							fprintf(stderr, "ERROR: Model code must be 8 characters long.\n");
							return -1;
						}
						sscanf(argv[i], "%X", &model_code);
						break;
					}
				}
			}
		}
		if (inFilemissing == 1)
		{
			fprintf(stderr, "ERROR: Input file name not specified (use -i)\n");
			return -1;
		}
		if (outFilemissing == 1)
		{
			fprintf(stderr, "ERROR: Output file name not specified (use -o)\n");
			return -1;
		}

		inFile = fopen(inFileName, "r");
		if (inFile == NULL)
		{
			fprintf(stderr, "ERROR: Cannot open input file %s.\n", inFileName);
			return -1;
		}
		fseek(inFile, 0L, SEEK_END);
		inFileLen = ftell(inFile);
		rewind(inFile);

		payload = (uint8_t *)malloc(sizeof(uint8_t) * inFileLen);
		if (payload == NULL)
		{
			fprintf(stderr, "ERROR: Memory error allocating file buffer\n");
			return -1;
		}
		i = fread(payload, 1, inFileLen, inFile);
		if (i != inFileLen)
		{
			fprintf(stderr, "ERROR reading input file %s.\n", inFileName);
			return -1;
		}
		fclose(inFile);

		if (verbose)
		{
			printf("Input file  : %s (length: %x (%d) bytes)\n", inFileName, inFileLen, inFileLen);
			printf("Output file : %s\n", outFileName);
			printf("Model code  : %08x (%s)\n", model_code, model_name);
			if (unknown)
			{
				printf("\nCAUTION: Unknown model code!\n");
			}
		}

		// compose header
		header_struct.file_type = FILE_TYPE_CHANNEL_LIST;
		header_struct.header_id = HEADER_TYPE_CHANNEL_LIST;
		header_struct.model_code = model_code;
		header_struct.sw_version_major = SW_CODE_CHANNEL_LIST >> 8;
		header_struct.sw_version_minor = SW_CODE_CHANNEL_LIST & 0xff;
		header_struct.hw_version_major = HW_CODE_CHANNEL_LIST >> 8;
		header_struct.hw_version_minor = HW_CODE_CHANNEL_LIST & 0xff;
		header_struct.length = inFileLen;
		header_struct.start_addr = flash_offset;
		dataCrc = 0;
		header_struct.crc32 = crc32(dataCrc, payload, inFileLen);
		header_struct.flag = FLAGS_CHANNEL_LIST;

		outFile = fopen(outFileName, "wb");
		if (outFile == NULL)
		{
			fprintf(stderr, "ERROR: Cannot open output file %s.\n", inFileName);
			return -1;
		}
		setHeader(outFile);

		// add binary payload
		fseek(outFile, sizeof(header_struct), SEEK_SET);
		fwrite(payload, 1, inFileLen, outFile);

		// check if succesfully written
		fseek(outFile, 0L, SEEK_END);
		if (sizeof(header_struct) + inFileLen != ftell(outFile))
		{
			fprintf(stderr, "ERROR: Writing output file %s failed.\n", outFileName);
		}
		else if (verbose)
		{
			printf("Creating file %s succesfully completed.\n", outFileName);
			printf("File length is 0x%x (%d) bytes.\n", sizeof(header_struct) + inFileLen, sizeof(header_struct) + inFileLen);
		}
		fclose(outFile);
		free(payload);
	}
	else if ((argc == 3 && strlen(argv[1]) == 2 && strncmp(argv[1], "-x", 2) == 0)
	     ||  (argc == 3 && strlen(argv[1]) == 3 && strncmp(argv[1], "-xv", 3) == 0))
	{  // -x(v): extract .upd into composing binary
		int res = -1;
		int32_t i;
		int32_t len = 0;
		FILE *inFile;
		uint8_t *buf;
		char outFileName[128];
		FILE *outFile;

		if (strncmp(argv[1], "-xv", 3) == 0)
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
			fprintf(stderr, "ERROR: Cannot open input file %s.\n", argv[2]);
			fclose(inFile);
			return -1;
		}

		// get header
		res = getHeader(inFile);
		if (header_struct.file_type == FILE_TYPE_CHANNEL_LIST)
		{
			printf("NOTE: file %s is a channel list file.\n", argv[2]);
		}
		// get payload
		if (verbose)
		{
			printf("Binary payload: 0x%x (%d) bytes\n", header_struct.length, header_struct.length);
		}
		len = getPayload(inFile);
		fclose(inFile);

		if (len != header_struct.length)
		{
			fprintf(stderr, "ERROR: File %s: header error (payload length wrong).\n", argv[2]);
			return -1;
		}
		// build output file name
		strcpy(outFileName, argv[2]);
		strcat(outFileName, ".bin");
		outFile = fopen(outFileName, "wb");
		if (outFile == NULL)
		{
			fprintf(stderr, "ERROR: Cannot open output file %s.\n", outFileName);
			return -1;
		}
		fwrite(payload, 1, len, outFile);
		fseek(outFile, 0L, SEEK_END);
		len = ftell(outFile);
		if (verbose)
		{
			printf("Extracting UPD file %s succesfully completed.\n", argv[2]);
			printf("0x%x (%d) bytes written to file %s.\n", len, len, outFileName);
		}
		fclose(outFile);
		verbose = 1;
	}
	else if ((argc == 4 && strlen(argv[1]) == 2 && strncmp(argv[1], "-m", 2) == 0)
	     ||  (argc == 4 && strlen(argv[1]) == 3 && strncmp(argv[1], "-mv", 3) == 0))
	{  // -m(v): Change model code
		uint32_t res;
		uint32_t model_code;
		uint32_t model_code_old;
		const char *model_name;
		FILE *inFile;

		if (strncmp(argv[1], "-mv", 3) == 0)
		{
			verbose = 1;
		}
		else
		{
			verbose = 0;
		}
		model_code = 0;

		if (strlen(argv[3]) != 8)
		{
			fprintf(stderr, "ERROR: Model code must be 8 characters long.\n");
			return -1;
		}
		sscanf(argv[3], "%X", &model_code);

		model_name = getModelName(model_code);
		if (verbose && unknown)
		{
			printf("CAUTION: You are changing the model code to an unknown one.\n");
		}

		inFile = fopen(argv[2], "r+");
		if (inFile == NULL)
		{
			fprintf(stderr, "ERROR: Cannot open input file %s.\n", argv[2]);
			return -1;
		}
		res = getHeader(inFile);
		model_code_old = header_struct.model_code;

		header_struct.model_code = model_code;  // set new model code
		setHeader(inFile);
		fclose(inFile);

		if (verbose)
		{
			printf("Changed model code\n");
			printf("from %08X (%s)\n", model_code_old, getModelName(model_code_old));
			printf("  to %08X (%s).\n", model_code, getModelName(model_code));
		}
	}
	else if ((argc == 4 && strlen(argv[1]) == 2 && strncmp(argv[1], "-n", 2) == 0)
	     ||  (argc == 4 && strlen(argv[1]) == 3 && strncmp(argv[1], "-nv", 3) == 0))
	{  // -n(v): Change SW version
		uint32_t res;
		uint32_t SW_major;
		uint32_t SW_minor;
		uint32_t SW_major_old;
		uint32_t SW_minor_old;
		FILE *inFile;

		if (strncmp(argv[1], "-nv", 3) == 0)
		{
			verbose = 1;
		}
		else
		{
			verbose = 0;
		}
		SW_major = 0;
		SW_minor = 0;

		if (strlen(argv[3]) != 4)
		{
			fprintf(stderr, "ERROR: SW version must be 4 characters long (format N.NN).\n");
			return -1;
		}
		sscanf(argv[3], "%d.%02d", &SW_major, &SW_minor);

		if (SW_major > 9 || SW_major < 0 || SW_minor > 99 || SW_minor < 0)
		{
			fprintf(stderr, "ERROR: SW version must be between 0.00 and 9.99.\n");
			return -1;
		}	

		inFile = fopen(argv[2], "r+");
		if (inFile == NULL)
		{
			fprintf(stderr, "ERROR: Cannot open input file %s.\n", argv[2]);
			return -1;
		}

		res = getHeader(inFile);
		SW_major_old = header_struct.sw_version_major >> 4;  // get old SW major
		SW_minor_old = header_struct.sw_version_minor;  // get old SW minor

		header_struct.sw_version_major = SW_major << 4;  // set SW major
		header_struct.sw_version_minor = SW_minor;  // set SW minor
		setHeader(inFile);
		fclose(inFile);

		if (verbose)
		{
			printf("Changed software version number ");
			printf("from V%d.%02d ", SW_major_old, SW_minor_old);
			printf("to V%d.%02d\n", SW_major, SW_minor);
		}
	}
	else if (argc == 2 && strlen(argv[1]) == 2 && strncmp(argv[1], "-v", 2) == 0)
	{  // -v: print version info
		printf("Version: %s  Date: %s\n", VERSION, DATE);
	}
	else  // show usage
	{
		printf("\noup - management program for Opticum .upd flash files\n");
		printf("\nVersion: %s  Date: %s\n", VERSION, DATE);
		printf("\n");
		printf("Usage: %s -i|-x|-xv|-u|-c|-m|-mv|-n|-nv|-v []\n", argv[0]);
		printf("  -i [file.upd]               Display detailed file information\n");
		printf("  -x [file.upd]               Extract binary payload from file.upd\n");
		printf("  -xv [file.upd]              As -x, verbose\n");
		printf("  -u Suboptions               Create Opticum .upd flash file\n");
		printf("     Suboptions for -u:\n");
		printf("     -i [infile]              Input file name: binary payload\n");
		printf("     -o [outfile.upd]         Output flash file name\n");
		printf("     -m [model_code]          Use model_code (default 07120603)\n");
		printf("     -n [N.NN]                Use version N.NN (default 1.00)\n");
		printf("     -s [flash offset]        Start address in flash (hexadecimal, default 0x40000)\n");
		printf("     -v                       Verbose operation\n");
		printf("  -c Suboptions               Create Opticum .cha flash file\n");
		printf("     Suboptions for -c:\n");
		printf("     -i [infile]              Input file: channel list binary\n");
		printf("     -o [outfile.cha]         Output flash file\n");
		printf("     -m [model_code]          Use model_code (default 07120603)\n");
		printf("     -v                       Verbose operation\n");
		printf("  -m [file.upd] [model code]  Change model code\n");
		printf("  -mv [file.upd] [model_code] As -r, verbose\n");
		printf("  -n [file.upd] [versionnr]   Change SW version number\n");
		printf("  -nv [file.upd] [versionnr]  As -n, verbose\n");
		printf("  -v                          Display program version\n");

	}
	return 0;
}
// vim:ts=4
