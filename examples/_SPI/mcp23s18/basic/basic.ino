#include <SPI.h>
#include <mcp23s18.h>   // import library

//define CS pin and using HAEN or not
//to use HAEN, address should be 0x20 to 0x27
mcp23s18 mcp(10,0x20);//instance

void setup(){
  mcp.begin();//x.begin(1) will override automatic SPI initialization
}

void loop(){
}
