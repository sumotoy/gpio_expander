#include <Wire.h>
#include <mcp23017.h>   // import library

//define CS pin and using HAEN or not
//to use HAEN, address should be 0x20 to 0x27
mcp23017 mcp(0x27);//instance

void setup(){
  delay(100);
  mcp.begin(false);//x.begin(true) will override automatic SPI initialization
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

