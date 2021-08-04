/*
MCP4922 DAC to Teensy 4.1
 
 Circuit:
 LDAC can be tied to VSS
 VREF can be tied to 5V or 3.3V 
 DRDY: pin 6
 MOSI (TX): pin 26
 MISO: pin 1 not connected unidirectional tx
 SCK: pin 27
 */

// the sensor communicates using SPI, so include the library:
#include <SPI.h>
#include <RingBuf.h>
uint8_t j = 0;
  
// the other you need are controlled by the SPI library):
const int selectDAC = 28;
const int LDAC = 24;
const int SHDN = 25;
const int numberoflevels = 256;
const int updaterate = 5;
RingBuf<unsigned int, 256> Buffer1; //max buffer 65535

//Create an IntervalTimer object
IntervalTimer myTimer;

void setup() {
  Serial.begin(9600);
  // start the SPI library:
  SPI1.begin();

  // initalize the  data ready and chip select pins:
  pinMode(selectDAC, OUTPUT);
  digitalWrite(selectDAC, HIGH); 
  
  pinMode(LDAC, OUTPUT);//LDAC
  digitalWrite(LDAC, LOW); //LDAC
  
  pinMode(SHDN, OUTPUT);//SHDN
  digitalWrite(SHDN, HIGH); //SHDN

  // give the sensor time to set up:
  delay(100);
  uint8_t i = 0;
   while(Buffer1.push(int(2000+2000*sin(4*3.14159*i++/numberoflevels))));//+524*sin(2*3*3.14159*i++/numberoflevels))));
  myTimer.begin(updatevalue,updaterate);
}

void updatevalue(){
  //writevalue(false,true,true,true,Buffer1[j]);
  MCP4922_write(0, Buffer1[j]); 
  MCP4922_write(1, 3000); 
  
  j++;
  if(j>256){
    j=0;
  }
}

void loop() {
}

void MCP4922_write(byte dac, int value) {
  byte low = value & 0xff; 
  byte high = (value >> 8) & 0x0f; 
  dac = (dac & 1) << 7; 
  _beginTransmission(selectDAC);
  SPI1.transfer(dac | 0x30 | high); 
  SPI1.transfer(low); 
  _endTransmission(selectDAC);
}

void _beginTransmission(int chipSelectPin) {
  ::digitalWrite(chipSelectPin, 0);
  SPI1.beginTransaction(SPISettings(30000, MSBFIRST, SPI_MODE0));
}

void _endTransmission(int chipSelectPin) {
  SPI1.endTransaction();
  ::digitalWrite(chipSelectPin, 1);
}
