 /**********************************************************
 This example reads all four channels of the MCP3424 
 while executing other tasks. An interrupt is set to 
 occur every 100ms. An ISR sets a flag to tell the main 
 loop to poll the MCP3424. The main loop toggles the UNO 
 LED and reads the status of the MCP3424's Ready bit. 
 When a conversion is complete, the result is displayed 
 and a channel counter is incremented. The process repeats
 through all four channels. It runs on an
 Arduino Uno and uses the (original) TimerOne library to
 program the timer. Tested using an Arduino
 Uno and DFRobot DFR0316 V1.0 module. 
 ***********************************************************/

  #include <Wire.h>
  #include <MicroChip3424.h>
  #include <TimerOne.h>
  
/* choose I2C address for MCP3424 */
  uint8_t adcAddress = 0x68;
     MCP3424 adc = MCP3424(adcAddress);
 
/* adc results are returned as long */
  long chResult;                  //Result returned as long
  const float lsb18 = 15.625e-6;  //18 bit resolution LSB
  float volts;                    //convert and scale adc result to float

  /* config and status values written to and read from MCP3424 */
  uint8_t configStatus;
  uint8_t rdStatus;

  /* Channel counter, update flag for ISR, and flag for main loop */
  volatile uint8_t chCount = 1;
  volatile boolean newConv = true;
  volatile boolean updateFlag = false;
  
 
  //default LED for Uno, etc. check your board for the LED pin used 
  int led = 13;
  uint8_t ledStatus = 0;
 
void setup() {
  // startup Wire and Serial libraries
   Serial.begin(9600);
   Wire.begin();  
  
  // Reset device
   uint8_t resetStatus = generalCallReset();
   delay(1);   
   Serial.print ("Reset Status 0X");
   Serial.println (resetStatus, HEX);

   /* check for device present */
   Wire.requestFrom(adcAddress, (uint8_t)1);
    if (Wire.available ()){
      Serial.print ("Found device at 0x");
      Serial.println (adcAddress, HEX);
      }
    else {
      Serial.println ("No device found");
      }

   // initialize timer1 for 100ms interrupts using microseconds
   Timer1.initialize(100000);
  // attach the service routine here  
   Timer1.attachInterrupt(timerISR); 

   // initialize LED and turn off
   pinMode(led, OUTPUT);
   digitalWrite(led, ledStatus);  
}

void loop() {
  //main loop toggles LED and polls the ADC
  if (updateFlag == true) {
    ledStatus = !ledStatus;      // toggle LED
    digitalWrite(led, ledStatus); 
   //the case statement will start a new conversion if the newConv flag is set 
    switch (chCount) {
      case 1:
        if (newConv == true){
          configStatus = adc.startConvert (Channel1,Oneshot,Res18,Gain1);
          newConv = false;
        } 
      break;
      case 2:
        if (newConv == true){
          configStatus = adc.startConvert (Channel2,Oneshot,Res18,Gain1);
          newConv = false;
        } 
       break;
       case 3:
        if (newConv == true){
          configStatus = adc.startConvert (Channel3,Oneshot,Res18,Gain1);
          newConv = false;
        }
       break;
       case 4:
        if (newConv == true){
          configStatus = adc.startConvert (Channel4,Oneshot,Res18,Gain1);
          newConv = false;
        }
       break;   
    }
    updateFlag = false;   //reset timer flag and
    showResult();         //poll the adc
   } 
  }

  void timerISR ()
    {
      // set timer flag 
      updateFlag = true;
    }  

    void showResult () {
      rdStatus=adc.read (&chResult);       //get status of conversion
      if (rdStatus != Ready){              //if conversion not ready, exit
        return;
      }
      Serial.print ("Channel ");
      Serial.print (chCount);
      volts = float(chResult)*lsb18;  //convert long to float 18 bit 
      Serial.print(" Volts = ");
      Serial.println (volts, 7);      //display with 7 digit accuracy
      newConv = true;                 //convert next channel
      chCount ++;                     //by incrementing channel counter
      if (chCount > 4)
        chCount = 1;   
  }
    

  
  
