/**************************************************************************/
/*!
    @file     MicroChip3424.h
    @author   Chris Bucsko
    @license  BSD (see license.txt)

    This is a library for the MCP342x 2/4 channel
    A/D converter from Microchip

    @section  HISTORY

    v1.0  - First release
    
*/
/**************************************************************************/

   #ifndef MicroChip3424_h
	#define MicroChip3424_h
	#include <Arduino.h>
   #include <Wire.h>
   
   /*=========================================================================
    I2C ADDRESS RANGE FOR MCP 3424 USING A0 A1 pins
    -----------------------------------------------------------------------*/
    #define addrLo               (0x68)   // 0110 1000 A0, A1 Low or Float
    #define addrHi               (0x6F)   // 0110 1111 A0 Float, A1 High                                  
   /*======================================================================*/

/* Configuration Register - Bit 7 is RDY bit    */
	/*=========================================================================
    Channel Select, bits 6, 5
   -----------------------------------------------------------------------*/
	#define Channel1					(0x00)
	#define Channel2					(0x20)
	#define Channel3					(0x40)
	#define Channel4					(0x60)
	/*=========================================================================
    Operation modes, oneshot and continuous, bit 4
   -----------------------------------------------------------------------*/
	#define Convert					(0x80)	// Starts a conversion in one shot mode					
	#define Oneshot					(0x00)	// Oneshot mode
	#define Continuous				(0x10)	// Continuous mode
	/*=========================================================================
    Resolution bits 3,2
   -----------------------------------------------------------------------*/
	#define Res12						(0x00)	// 12 bit resolution
	#define Res14						(0x04)	// 14 bit resolution
	#define Res16						(0x08)	// 16 bit resolution
	#define Res18						(0x0C)	// 18 bit resolution
	/*=========================================================================
    Gain bits 1,0
   ------------------------------------------------------------------------*/
	#define Gain1						(0x00)	// Gain 1
	#define Gain2						(0x01)	// Gain 2
	#define Gain4						(0x02)	// Gain 4
	#define Gain8						(0x03)	// Gain 8
	/*=========================================================================
    Error codes
   ------------------------------------------------------------------------*/
	#define busError					(0xFF)	// I2C bus error
	#define notReady					(0x80) 	// Conversion not complete
	#define Ready						(0x00)	// Conversion complete
	
	/*** Find Address of MCP3424 *********************/
	uint8_t findAddress (void);
	/*************************************************/
	
	/*** These I2C General Calls are supported by the MCP3424 **********************/
	uint8_t generalCallReset(void);			//Reset MCP3424, config default values
   uint8_t generalCallLatch(void);			//Latch address bits, config unchanged
   uint8_t generalCallConversion(void);	//Start conversion with current config
	/*******************************************************************************/
	
	
	
class MCP3424 {
	public:
		//constructors
		MCP3424();
		MCP3424(uint8_t address);
		
		//configure MCP3424 for conversion
		uint8_t config;  
		uint8_t configADC (uint8_t ch, uint8_t mode, uint8_t res, uint8_t gain);
		
		//start a one shot conversion by setting RDY bit to 1
		uint8_t startConvert (uint8_t ch, uint8_t mode, uint8_t res, uint8_t gain);
		
		//read result from MPC3424
		uint8_t read(long *result);
		
		private:
			uint8_t address;
	};
	
	
#endif