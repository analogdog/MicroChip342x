/**************************************************************************/
/*!
    @file     MicroChip3424.cpp
    @author   Chris Bucsko
    @license  BSD (see license.txt)

    This is a library for the MCP342x 2/4 channel
    A/D converter from Microchip

    @section  HISTORY

    v1.0  - First release
    
*/
/**************************************************************************/

	#include <Arduino.h>
	//#include <Wire.h>
	#include "MicroChip3424.h"

/* ***** findAddress *******************************************/
	uint8_t findAddress (void)
	{
		uint8_t addr = 0; // init to 0
      uint8_t a;
		for (a = addrLo; a <= addrHi; a++)
		{ 
			Wire.requestFrom(a, (uint8_t)1); 
				if (Wire.available())
					addr = a;
		}	
		return addr;
/* if device not found return 0, otherwise return address */
	}
/****** I2C General Calls *************************************/	
	uint8_t generalCallReset(void)
	{
		Wire.beginTransmission(0x00);
		Wire.write(0x06);
		return Wire.endTransmission();
	}

	uint8_t generalCallLatch(void)
	{
		Wire.beginTransmission(0x00);
		Wire.write(0x04);
		return Wire.endTransmission();
	}

	uint8_t generalCallConversion(void)
	{
		Wire.beginTransmission(0x00);
		Wire.write(0x08);
		return Wire.endTransmission();
	}
/****** Constructors *************************************/	
//Default address with address pins open or floating is 0x68
MCP3424::MCP3424 (void) : address(0x68)	
	{
		; 
	}
	
MCP3424::MCP3424 (uint8_t addr) : address(addr)	
	{
		;
	}
	
/****** Configure MCP3424 without setting Ready Bit *****/
uint8_t MCP3424::configADC (uint8_t ch, uint8_t mode, uint8_t res, uint8_t gain)
	{
		config = ch|mode|res|gain;
		Wire.beginTransmission(address);
		Wire.write(config);
	// return coniguration
		Wire.endTransmission();
		return config;
	}

/* Configure and start a conversion by setting Ready Bit */	
uint8_t MCP3424::startConvert (uint8_t ch, uint8_t mode, uint8_t res, uint8_t gain)
	{
	// start the conversion by setting Rdy bit with configuration
		config = Convert|ch|mode|res|gain;
		Wire.beginTransmission(address);
		Wire.write(config);
	// return status of write, 0 if successful
		Wire.endTransmission();
		return config;
	}
	 
 uint8_t MCP3424::read(long *result)
	{
	/* Read 4 bytes, the 4th byte will always be
		a config byte, with resolution info.
		If 18 bit resolution, then 3 bytes are conversion
		result. Otherwise, 1st and 2nd bytes are the result. */
		uint8_t len = 4;
		uint8_t buffer[4] = {};
		uint8_t validBytes;
		uint8_t status;
	//request 4 bytes from device
		Wire.requestFrom(address, len);
		if (Wire.available() != len) 
			return busError;
	//get conversion results into buffer
				for (uint8_t i = 0; i < len; ++i)
					buffer[i] = Wire.read();
	//test last byte for 18 bit conversion		
				if ((buffer[3] & Res18) == Res18) {
					status = buffer[3]; 	// 18 bit conversion
					validBytes = 3;
				}
				else {
					status = buffer[2];	//12, 14, 16 bit conversion
					validBytes = 2;
				}
	//if ready bit is set, conversion not complete 
				if ((status & notReady) == notReady)
					return notReady;
	
	//sign extend MSB for 2's complement 32 bit long result
			long signBit = 0;    // Location of sign bit
			long signExtend = 0; // Bits to be set if sign is set
	//Res18 is 0x0C, mask for resolution bits in config register
			switch (status & Res18) {
				case Res12:
					signBit = 0x800;
					signExtend = 0xFFFFF000;
				break;
				case Res14:
					signBit = 0x2000;
					signExtend = 0xFFFFC000;
				break;
				case Res16:
					signBit = 0x8000;
					signExtend = 0xFFFF0000;
				break;
				case Res18:
					signBit = 0x20000;
					signExtend = 0xFFFC0000;
				break;
				}
	//assemble conversion result with sign extended bits
				*result = 0;
				for (uint8_t i = 0; i < validBytes; ++i) {
					*result <<= 8;
					*result |= (long)buffer[i];
				}
  // Fill/blank remaining bits  
				if ((*result & signBit) != 0) 
					*result |= signExtend; // Sign bit is set, sign-extend
				return Ready;	
	}
   
	



	


