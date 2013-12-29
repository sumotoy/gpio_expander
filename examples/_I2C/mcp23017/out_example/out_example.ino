#include <Wire.h>
#include <mcp23017.h>   // import library


//address should be 0x20 to 0x27
mcp23017 mcp(0x20);//instance

void setup(){
  mcp.begin();//x.begin(1) will override automatic wire initialization
  // Set all pins to be outputs (by default they are all inputs)
  mcp.gpioPinMode(OUTPUT);
  // Change all pins at once, 16-bit value
  mcp.gpioPort(0xFFFF);
}

void loop(){
  for (int i=0;i<16;i++){
    mcp.gpioDigitalWrite(i,LOW);
    delay(150);
  }
  for (int i=0;i<16;i++){
    mcp.gpioDigitalWrite(i,HIGH);
    delay(150);
  }
}