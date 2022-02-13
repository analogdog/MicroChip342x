 /******************************************
 This example uses findAddress to determine
 the location of the MCP3424. Allowable
 addresses are between 0x68 and 0x6F. 
 *******************************************/

  #include <Wire.h>
  #include <MicroChip3424.h>

  //default LED for Uno, etc. check your board for the LED pin used 
  int led = 13;
  
void setup() {
  // startup Wire and Serial libraries
   Serial.begin(9600);
   Wire.begin();
        
   pinMode(led, OUTPUT);
  
  // Reset device
   uint8_t resetStatus = generalCallReset();
   delay(1);   
   Serial.print ("Reset Status 0X");
   Serial.println (resetStatus, HEX);

   Serial.println ("Looking for device");
   uint8_t adcAddress = findAddress();
   
   /* check for device present */
   Wire.requestFrom(adcAddress, (uint8_t)1);
    if (Wire.available ()){
      Serial.print ("Found device at 0x");
      Serial.println (adcAddress, HEX);
      }
    else {
      Serial.println ("No device found");
     } 
}  

void loop() {
  digitalWrite(led, HIGH);   // turn the LED on
  delay(2000);               // wait for 2 seconds
  digitalWrite(led, LOW);    // turn the LED off
  delay(2000);               // wait for 2 second
}
 
    
    

  
  
