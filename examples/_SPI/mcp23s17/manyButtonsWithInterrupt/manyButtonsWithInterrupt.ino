/*
In this example we will use a MCP23S17 with 16 push buttons.
 The example show how to deal with registers to set-up the GPIO chip in different mode than default.
 It use a processor interrupt to trig the key decode.
 */
#include <SPI.h>
#include <mcp23s17.h>   // import library

#define GPIO_ADRS          0x20//SPI

const uint8_t gpio_cs_pin = 10;
const uint8_t gpio_int_pin = 2;//pin connected to INTA mcp23s17
const uint8_t gpio_int = 2;//the corrispondent interrupt connected to the pin above

volatile boolean keyPressed     = false;

#if defined (SPI_HAS_TRANSACTION)
mcp23s17 mcp(gpio_cs_pin,GPIO_ADRS,30000000);//using transactions?
#else
mcp23s17 mcp(gpio_cs_pin,GPIO_ADRS);
#endif


void setup() {
  pinMode(gpio_int_pin, INPUT);
  Serial.begin(38400);
#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MKL26Z64__) //teensy series
  long unsigned debug_start = millis ();
  while (!Serial && ((millis () - debug_start) <= 5000));
#endif
  mcp.begin();//it will initialize SPI as well
  //setup the gpio!
  /*        7     6     5	     4     3   2     1    0
   IOCON = BANK MIRROR SEQOP DISSLW HAEN ODR INTPOL -NC- */
  mcp.gpioRegisterWriteByte(mcp.IOCON, 0b01101000);//HAEN,SEQOP,MIRROR
  mcp.gpioRegisterWriteByte(mcp.GPPU, 0xFF, true);//pull-up every port (A&B)
  mcp.gpioRegisterWriteByte(mcp.IPOL, 0xFF, true);//invert polarity on every port (A&B)
  mcp.gpioRegisterWriteByte(mcp.GPINTEN, 0xFF, true);//enable interrups on every port (A&B)
  mcp.gpioRegisterReadByte(mcp.INTCAP);    //read interrupt capture port A (it clear port)
  mcp.gpioRegisterReadByte(mcp.INTCAP + 1);//read interrupt capture port B (it clear port)
  attachInterrupt(gpio_int, keypress, FALLING);//attack interrupt
}

int onKeypress(){
  detachInterrupt(gpio_int);//disconnect int
  unsigned int keyValue = 0;
  delay(50);//debounce
  int result = -1;//default, means no button pressed
  uint8_t i;
  keyPressed = false;//time to reset keypress
  if (mcp.gpioRegisterReadByte(mcp.INTF)) keyValue |= mcp.gpioRegisterReadByte(mcp.INTCAP);//read value port A
  if (mcp.gpioRegisterReadByte(mcp.INTF + 1)) keyValue |= mcp.gpioRegisterReadByte(mcp.INTCAP + 1) << 8;//read value port B
  for (i = 0; i < 16; i++){
    if (keyValue & (1 << i)) result = i;
  }
  attachInterrupt(2, keypress, FALLING);//reconnect INT
  return result;
}

void loop(){
  if (keyPressed) {
    int key = onKeypress();
    if (key > -1)  Serial.println(key);
  }
}

void keypress(){
  keyPressed = true;
}

