#include <Wire.h>
#include <max7311.h>   // import library


max7311 mcp(0x20);//instance

void setup(){
  Serial.begin(38400);
  delay(100);
  mcp.begin();//x.begin(true) will override automatic SPI initialization
  mcp.gpioPinMode(OUTPUT);

}

void loop(){

  for (int i = 0; i < 15; i++) {
    mcp.gpioDigitalWrite(i, HIGH);
    delay(100);
  }

  for (int i = 0; i < 15; i++) {
    mcp.gpioDigitalWrite(i, LOW);
    delay(100);
  } 
}
