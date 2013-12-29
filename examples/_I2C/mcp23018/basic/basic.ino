#include <Wire.h>
#include <mcp23018.h>   // import library

//define CS pin and using HAEN or not
//to use HAEN, address should be 0x20 to 0x27
mcp23018 mcp(0x20);//instance

void setup(){
  mcp.begin();//x.begin(1) will override automatic SPI initialization
}

void loop(){
}
