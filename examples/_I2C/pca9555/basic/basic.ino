#include <Wire.h>
#include <pca9555.h>   // import library
//will work even for tca9555


pca9555 mcp(0x20);//instance

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
