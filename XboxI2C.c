/***********************************************************
*
*	XboxI2C.c - I2C interface for the xbox console.
*
*	Author: Grimdoomer
*
*	Nov 22nd, 2015
*		- Initial creation.
*
************************************************************/

#include "XboxI2C.h"
#include <bcm2835.h>
#include <string.h>

void XboxI2C_Init(void)
{
	// Initialize the I2C interface on the raspberry pi.
	bcm2835_i2c_begin();
}

void XboxI2C_Cleanup(void)
{
	// Cleanup the I2C interface and reset the pins to their normal state.
	bcm2835_i2c_end();
}

int XboxI2C_ReadEEPROM(char bAddress, char *pbBuffer)
{
	char commandBuffer[1];
	int status;

	// Check that the I2C address of the chip is valid.
	if (bAddress < 0 || bAddress > 128)
		return -1;

	// Check that a valid buffer address was passed.
	if (pbBuffer == 0)
		return -1;

	// Clear the contents of the output buffer.
	memset(pbBuffer, 0, XBOX_EEPROM_SIZE);

	// Set the I2C slave address to the EEPROM chip address.
	bcm2835_i2c_setSlaveAddress(bAddress);

	// Set the target address to the beginning of the EEPROM chip.
	commandBuffer[0] = 0;

	// Send the target address to the xbox EEPROM chip.
	status = bcm2835_i2c_write(commandBuffer, 1);
	if (status != BCM2835_I2C_REASON_OK)
	{
		// An error occured while reading the EEPROM chip.
		return status;
	}

	// Read the EEPROM buffer from the chip
	status = bcm2835_i2c_read(pbBuffer, XBOX_EEPROM_SIZE);
	if (status != BCM2835_I2C_REASON_OK)
	{
		// Failed to read all of the EEPROM data.
		return status;
	}

	// Successfully read the EEPROM chip.
	return status;
}

int XboxI2C_WriteEEPROM(char bAddress, char *pbBuffer)
{
	char commandBuffer[2];
	int status;
	int i;

	// Check that the I2C address of the chip is valid.
	if (bAddress < 0 || bAddress > 128)
		return -1;

	// Check that a valid buffer address was passed.
	if (pbBuffer == 0)
		return -1;

	// Set the I2C slave address to the EEPROM chip address.
	bcm2835_i2c_setSlaveAddress(bAddress);

	// Loop through the buffer to write.
	for (i = 0; i < XBOX_EEPROM_SIZE; i++)
	{
		// Set the target address and data for the current byte.
		commandBuffer[0] = (char)i;
		commandBuffer[1] = pbBuffer[i];

		// Write the data to the chip.
		status = bcm2835_i2c_write(commandBuffer, 2);
		if (status != BCM2835_I2C_REASON_OK)
		{
			// Error writing data to the EEPROM chip.
			return status;
		}

		// Wait before writing the next byte.
		bcm2835_delay(10);
	}

	// Successfully wrote the buffer to the EEPROM chip.
	return status;
}