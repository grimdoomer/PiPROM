/***********************************************************
*
*	XboxI2C.h - I2C interface for the xbox console.
*
*	Author: Grimdoomer
*
*	Nov 22nd, 2015
*		- Initial creation.
*
************************************************************/

#ifndef _XBOX_I2C_H
#define _XBOX_I2C_H

/////////////////////////////////////////////////////////////
// Initialization/cleanup for the I2C interface.
/////////////////////////////////////////////////////////////

void XboxI2C_Init(void);

void XboxI2C_Cleanup(void);

/////////////////////////////////////////////////////////////
// Xbox EEPROM functions.
/////////////////////////////////////////////////////////////

#define XBOX_EEPROM_ADDRESS		0x54
#define XBOX_EEPROM_SIZE		256

/* 
	Description: Reads the contents of the xbox EEPROM at address bAddress into pbBuffer.

	Parameters:
		bAddress: I2C address of the xbox EEPROM chip.
		pbBuffer: Buffer to read to, must be at least XBOX_EEPROM_SIZE bytes in size.

	Returns: A bcm2835I2CReasonCodes error code for I2C communication failure, -1 for an
		invalid parameter value, or 0 on success.
*/
int XboxI2C_ReadEEPROM(char bAddress, char *pbBuffer);

/*
	Description: Writes the contents of pbBuffer to the xbox EEPROM at address bAddress.

	Parameters:
		bAddress: I2C address of the xbox EEPROM chip.
		pbBuffer: Buffer to read to, must be at least XBOX_EEPROM_SIZE bytes in size.

	Returns: A bcm2835I2CReasonCodes error code for I2C communication failure, -1 for an
		invalid parameter value, or 0 on success.
*/
int XboxI2C_WriteEEPROM(char bAddress, char *pbBuffer);

#endif