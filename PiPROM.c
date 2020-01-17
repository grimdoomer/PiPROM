/***********************************************************
*
*	PiPROM.c - PiPROM entry point.
*
*	Author: Grimdoomer
*
*	Jun 15th, 2015
*		- Initial creation.
*
*	Nov 22nd, 2015
*		- Cleaned up for public release.
*
************************************************************/

#include <bcm2835.h>
#include <stdio.h>
#include "XboxI2C.h"
#include <string.h>
#include <stdlib.h>

#define MODE_READ		1
#define MODE_WRITE		2
#define MODE_ERASE		3

void PrintBanner()
{
	// Print the version banner.
	printf("PiPROM v1.0 by Grimdoomer\n");
	printf("\n");
}

void PrintUse()
{
	// Print the use specification.
	PrintBanner();
	printf("Usage: PiPROM -r/w/e <options> <filename>\n");
	printf("\t-r\t\tRead the EEPROM chip contents to <filename>.\n");
	printf("\t-w\t\tWrite the contents of <filename> to the EEPROM chip.\n");
	printf("\t-e\t\tErases the EEPROM filling it with 00s.\n");
	printf("\tfilename\tEEPROM file to read from/write to.\n");

	printf("\nOptional:\n");
	printf("\t-a <address>\tI2C address of the EEPROM chip.\n");

	printf("\n");
}

const char* FindCommendLineArg(int argc, char **argv, const char *psArgument)
{
	int i;

	// Loop through all of the command line arguemtns.
	for (i = 0; i < argc; i++)
	{
		// Check if the current command line argument matches the one provided.
		if (strcmp(argv[i], psArgument) == 0)
			return argv[i];
	}

	// The command line argument provided was not found.
	return 0;
}

const char* FindCommndLineArgValue(int argc, char **argv, const char *psArgument)
{
	int i;

	// Loop through all of the command line arguemtns.
	for (i = 0; i < argc; i++)
	{
		// Check if the current command line argument matches the one provided.
		if (strcmp(argv[i], psArgument) == 0)
		{
			// Check if there is another argument after the current one.
			if (i + 1 <= argc)
				return argv[i + 1];
		}
	}

	// The command line argument provided was not found.
	return 0;
}

int main(int argc, char **argv)
{
	int mode;
	int address;
	char *psFileName;
	FILE *file;
	char pbEEPROM[XBOX_EEPROM_SIZE];
	int status;

	// Parse the command line.
	if (argc >= 2)
	{
		// Check for the read/write argument.
		if (FindCommendLineArg(argc, argv, "-r") != 0)
			mode = MODE_READ;
		else if (FindCommendLineArg(argc, argv, "-w") != 0)
			mode = MODE_WRITE;
		else if (FindCommendLineArg(argc, argv, "-e") != 0)
			mode = MODE_ERASE;

		// Check for the address argument.
		if (FindCommendLineArg(argc, argv, "-a") != 0)
		{
			// Get the address argument value.
			const char *psAddress = FindCommndLineArgValue(argc, argv, "-a");

			// Check that the argument value is not the last argument in the command line.
			if (mode != MODE_ERASE && psAddress == argv[argc - 1])
			{
				// Invalid arguments provided.
				PrintUse();
				return 0;
			}

			// Convert the I2C address to an integer.
			address = strtol(psAddress, NULL, 16);
		}
		else
		{
			// Use the default xbox EEPROM chip address.
			address = XBOX_EEPROM_ADDRESS;
		}

		// Check if we are in erase mode or not.
		if (mode != MODE_ERASE)
		{
			// The last argument is the EEPROM file name.
			psFileName = argv[argc - 1];
		}
	}
	else
	{
		// Invalid arguments provided.
		PrintUse();
		return 0;
	}

	// Print the program banner.
	PrintBanner();

	// Try to open the EEPROM dump file that was provided.
	if (mode == MODE_READ)
		file = fopen(psFileName, "w");
	else if (mode == MODE_WRITE)
		file = fopen(psFileName, "r");

	// Check that the file handle is valid.
	if (mode != MODE_ERASE && file == NULL)
	{
		printf("Error opening file \"%s\"!\n");
		goto Cleanup;
	}

	// Initialize the broadcom processor.
    if (!bcm2835_init())
	{
		printf("Failed to initialize bcm!\n");
		return 1;
	}

	// Initialize the I2C interface.
	XboxI2C_Init();

	// Check if we are reading or writing and handle accordingly.
	if (mode == MODE_READ)
	{
		// Read the EEPROM data from the chip.
		printf("Reading EEPROM data from address 0x%02x...\n", address);
		status = XboxI2C_ReadEEPROM((char)address, pbEEPROM);
		if (status != 0)
		{
			// An error occured while reading the EEPROM chip.
			printf("Error reading EEPROM data %d!\n", status);
			goto Cleanup;
		}

		// Write the data to file.
		printf("Writing EEPROM data to file...\n");
		status = fwrite(pbEEPROM, 1, XBOX_EEPROM_SIZE, file);
		if (status != XBOX_EEPROM_SIZE)
		{
			// Error writing the EEPROM to file.
			printf("Error writing EEPROM to file!\n");
			fclose(file);
			goto Cleanup;
		}

		// Close the file.
		fclose(file);

		// Successfully dumped the EEPROM to file.
		printf("Successfully dumped EEPROM!\n");
		status = 0;
	}
	else if (mode == MODE_WRITE)
	{
		// Read the EEPROM buffer from the file.
		printf("Reading EEPROM from file...\n");
		status = fread(pbEEPROM, 1, XBOX_EEPROM_SIZE, file);
		if (status != XBOX_EEPROM_SIZE)
		{
			// Error reading the EEPROM image from file.
			printf("Error reading EEPROM from file!\n");
			goto Cleanup;
		}

		// Close the file.
		fclose(file);

		// Write the EEPROM data to the console.
		printf("Writing to EEPROM at address 0x%02x...\n", address);
		status = XboxI2C_WriteEEPROM((char)address, pbEEPROM);
		if (status != 0)
		{
			// An error occured while writing the EEPROM data to the chip.
			printf("Error writing EEPROM data %d!\n", status);
			goto Cleanup;
		}

		// Successfully wrote the EEPROM.
		printf("Successfully wrote EEPROM!\n");
		status = 0;
	}
	else if (mode == MODE_ERASE)
	{
		// Initialize the EEPROM data to all 00s.
		memset(pbEEPROM, 0, XBOX_EEPROM_SIZE);

		// Write the EEPROM data to the console.
		printf("Writing to EEPROM at address 0x%02x...\n", address);
		status = XboxI2C_WriteEEPROM((char)address, pbEEPROM);
		if (status != 0)
		{
			// An error occured while writing the EEPROM data to the chip.
			printf("Error writing EEPROM data %d!\n", status);
			goto Cleanup;
		}

		// Successfully erased the EEPROM.
		printf("Successfully erased EEPROM!\n");
		status = 0;
	}

Cleanup:
	// Cleanup the I2C interface.
	XboxI2C_Cleanup();

	return status;
}
