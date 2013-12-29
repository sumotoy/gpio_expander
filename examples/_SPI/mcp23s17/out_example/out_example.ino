#include <SPI.h>
#include <mcp23s17.h>   // import library

//define CS pin and using HAEN or not
//to use HAEN, address should be 0x20 to 0x27
mcp23s17 mcp(10,0x20);//instance

void setup(){
  mcp.begin();//x.begin(1) will override automatic SPI initialization
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