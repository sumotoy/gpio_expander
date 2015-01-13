#include <Wire.h>
#include <pcf8574.h>   // import library


pcf8574 mcp(0x20);//instance

void setup(){
  mcp.begin();
  mcp.gpioPinMode(OUTPUT);
  mcp.gpioDigitalWrite(0,LOW);
}

void loop(){
  mcp.gpioPort(LOW);
  delay(300);
  mcp.gpioPort(HIGH);
  delay(300);
}