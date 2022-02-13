/******************************************************
*  This example uses the MCP3424 and converts         *
*  all 4 channels, each with a different resolution.  *
*  Then each result is converted to a floating point  *
*  voltage and displayed. The number of digits depends*
*  on the resolution.                                 *
*  Tested using the DF Robot DFR0316 V1.0 eval module * 
*  set to default I2C Address 0x68 hex                *
*******************************************************/
  
  #include <Wire.h>
  #include <MicroChip3424.h>
/* create instance of MCP3424 at default address 0x68 */
  MCP3424 adc = MCP3424();

/* adc results are returned as long */
  long chResult;
  long *thisResult;   //Pointer to current conversion

  /* config values written to MCP3424 register for checking */
  uint8_t configStatus;
  uint8_t rdStatus;

  /* float LSB voltage values for each ADC Resolution */
  const float lsb12 = 1e-3;       //12 bit resolution
  const float lsb14 = 250e-6;     //14 bit resolution
  const float lsb16 = 62.5e-6;    //16 bit resolution
  const float lsb18 = 15.625e-6;  //18 bit resolution
  float volts;        //convert and scale adc result to float
  char vstr[10];      //convert float to string for display 
  
  //default LED for Uno, etc. check your board for the LED pin used 
  int led = 13;
 
void setup() {
  // startup Wire and Serial libraries
  Serial.begin(9600);
  Wire.begin();
  
  // Reset device
  uint8_t resetStatus = generalCallReset();
  delay(1);
    Serial.println ("************** BEGIN ***************************");   
    Serial.print ("Reset Status 0X");
    Serial.println (resetStatus, HEX);

   /*find device address of MCP3424, check if it is correct */
   uint8_t adcAddress = findAddress();
   if (adcAddress != 0) {
    Serial.print ("Found device at 0X");
    Serial.println (adcAddress, HEX);
   }
   else {
    Serial.println ("No device found");
   }
  
  //setup LED and blink once for 2 seconds
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);   // turn the LED on
  delay(1000);               // wait for 1 seconds
  digitalWrite(led, LOW);    // turn the LED off
  delay(1000);               // wait for 1 second 

  Serial.println("***************** Channel 1 *********************");
  configStatus = adc.startConvert (Channel1, Oneshot, Res12, Gain1); 
  Serial.print ("Channel 1 configuration 0x"); 
  Serial.println (configStatus, HEX);
  getResult();                    //read result from ADC  
  Serial.print ("Channel 1 Dec Value ");
  Serial.println (chResult);
  volts = float(chResult)*lsb12;  //convert long to float 12 bit (1mV/bit) 
  displayVolts (3);               //display float voltage with 1 mV resolution

  Serial.println("***************** Channel 2 *********************");
  configStatus = adc.startConvert (Channel2, Oneshot, Res14, Gain1); 
  Serial.print ("Channel 2 configuration 0x"); 
  Serial.println (configStatus, HEX);
  getResult();                //read result from ADC 
  Serial.print ("Channel 2 Dec Value ");
  Serial.println (chResult);
  volts = float(chResult)*lsb14;  //convert long to float 14 bit (250uV/bit) 
  displayVolts (5);               //display float voltage with 10uV resolution

  Serial.println("***************** Channel 3 *********************");
  configStatus = adc.startConvert (Channel3, Oneshot, Res16, Gain1); 
  Serial.print ("Channel 3 configuration 0x"); 
  Serial.println (configStatus, HEX);
  getResult();                //read result from ADC
  Serial.print ("Channel 3 Dec Value ");
  Serial.println (chResult);
  volts = float(chResult)*lsb16;  //convert long to float 16 bit (62.5uV/bit) 
  displayVolts (7);               //display float voltage with 100nV resolution

  Serial.println("***************** Channel 4 *********************");
  configStatus = adc.startConvert (Channel4, Oneshot, Res18, Gain1); 
  Serial.print ("Channel 3 configuration 0x"); 
  Serial.println (configStatus, HEX);
  getResult();                //read result from ADC
  Serial.print ("Channel 4 Dec Value ");
  Serial.println (chResult);
  volts = float(chResult)*lsb18;  //convert long to float 18 bit (15.625uV/bit) 
  displayVolts (7);               //display float voltage with 100nV resolution

 Serial.println ("************** END *************************"); 
}

void loop() {
  digitalWrite(led, HIGH);   // turn the LED on
  delay(1000);               // wait for 1 seconds
  digitalWrite(led, LOW);    // turn the LED off
  delay(1000);               // wait for 1 second 
}

  void displayVolts (uint8_t frac) {
    dtostrf(volts,9,frac,vstr);        //convert float to string
    Serial.print (vstr);
    Serial.println (" Volts");
  }
  
  void getResult () {
    thisResult = &chResult;         //init pointer
    do {
      delay(5);                     //delay before reading
      rdStatus=adc.read (thisResult); 
      } while (rdStatus != Ready);  //then wait for conversion
    Serial.print ("Complete 0X");
    Serial.println (rdStatus, HEX); 
  }
