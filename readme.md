# Microchip MCP342x family ADC library

Arduino library to support Microchip MCP342x family of analog to digital
converters. These devices utilise the I2C bus, so this library depends on  the Arduino Wire library for low level comunication. This is a fork of Steve Marple's original MCP342x library with the following revisions:

*  Library is renamed as MicroChip3424 to avoid confusion with the original library.

*  The control bits for resolution, gain, channel number, and conversion mode are #defines.

*  generalCalls (Reset, Latch and Conversion) are simple subroutines that can be called without a class definition.

*  getAddress is replaced by findAddress that searches for devices in the range of 0x68 to 0x6F only.

*  Only 1 read routine which is non-blocking (if the conversion is not ready, it will return a not ready status). You must pass a pointer of type long to the read routine for the result.  

## Supported devices

*   MCP3422: 2 channel, 12, 14, 16, or 18 bit
*   MCP3423: 2 channel, 12, 14, 16, or 18 bit
*   MCP3424: 4 channel, 12, 14, 16, or 18 bit
*   MCP3426: 2 channel, 12, 14, or 16 bit
*   MCP3427: 2 channel, 12, 14, or 16 bit
*   MCP3428: 4 channel, 12, 14, or 16 bit

The MCP3422 and MCP3426 use I2C address 0x68, all other devices can be
configured to use any address in the range 0x68 - 0x6F (inclusive).

## License
Released under the original GNU Lesser General Public License, version 2.1. See
license.txt for details.

## Examples

### Address 
Example of using findAddress to locate a device in the MCP342x's address range.

### Interrupt_Polling
Demonstrate the use of the read function in an interrupt driven program. It uses TimerOne (the original version) to generate an interrupt every 100ms. The main program steps through each channel and polls the ADC and toggles the UNO LED every time the interrupt fires. 

### ReadResolutions
Demonstrates the different resolutions of the MCP3424, converted to a voltage using floating point numbers.

## Testing
Tested using a DF Robot DFR0316 V1.0 Eval module, an LM4040C20DILPR 2.048 voltage reference plus 2 10k resistors. The MCP2424's channels were connected as +full scale, +1/2 scale, -1/2 scale and -full scale voltages.





