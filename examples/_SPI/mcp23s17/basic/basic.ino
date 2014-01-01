#include <SPI.h>
#include <mcp23s17.h>   // import library

//define CS pin and using HAEN or not
//to use HAEN, address should be 0x20 to 0x27
mcp23s17 mcp(10,0x27);//instance (address A0,A1,A2 tied to +)

void setup(){
  mcp.begin();//x.begin(1) will override automatic SPI initialization
  mcp.gpioPinMode(OUTPUT);
}

void loop(){
  for (int i = 0; i < 15; i++) {
    mcp.gpioDigitalWrite(i, HIGH);
    delay(10);
  }

  for (int i = 0; i < 15; i++) {
    mcp.gpioDigitalWrite(i, LOW);
    delay(10);
  } 
}

