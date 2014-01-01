#include <SPI.h>
#include <mcp23s08.h>   // import library

//define CS pin and using HAEN or not
//to use HAEN, address should be 0x20 to 0x23
mcp23s08 mcp(10,0x23);//instance

void setup(){
  mcp.begin();//x.begin(true) will override automatic SPI initialization
  mcp.gpioPinMode(OUTPUT);
}

void loop(){
  for (int i = 0; i < 8; i++) {
    mcp.gpioDigitalWrite(i, HIGH);
    delay(10);
  }

  for (int i = 0; i < 8; i++) {
    mcp.gpioDigitalWrite(i, LOW);
    delay(10);
  } 
}
