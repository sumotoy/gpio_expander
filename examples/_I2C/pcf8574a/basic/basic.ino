#include <Wire.h>
#include <pcf8574a.h>   // import library


pcf8574a mcp(0x38);//instance

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