#include <stdio.h>

#include <inttypes.h>

#include <Arduino.h>

#include "mcp23s08.h"
#include <../SPI/SPI.h>//this chip needs SPI



mcp23s08::mcp23s08(const uint8_t csPin,const uint8_t haenAdrs){
	_cs = csPin;
	if (haenAdrs >= 0x20 && haenAdrs <= 0x23){//HAEN works between 0x20...0x23
		_adrs = haenAdrs;
		_useHaen = 1;
	} else {
		_adrs = 0;
		_useHaen = 0;
	}
	_readCmd =  (_adrs << 1) | 1;
	_writeCmd = _adrs << 1;
	//setup register values for this chip
	IOCON = 	0x05;
	IODIR = 	0x00;
	GPPU = 		0x06;
	GPIO = 		0x09;
	GPINTEN = 	0x02;
	IPOL = 		0x01;
	DEFVAL = 	0x03;
	INTF = 		0x07;
	INTCAP = 	0x08;
	OLAT = 		0x0A;
	INTCON = 	0x04;
}


void mcp23s08::begin(bool protocolInitOverride) {
	if (!protocolInitOverride){
		SPI.begin();
		SPI.setClockDivider(SPI_CLOCK_DIV4); 
		SPI.setBitOrder(MSBFIRST);
		SPI.setDataMode(SPI_MODE0);
	}	
	pinMode(_cs, OUTPUT);
	digitalWrite(_cs, HIGH);
	delay(100);
    if (_useHaen){
		writeByte(IOCON,0b00101000);//read datasheet for details!
	} else {
		writeByte(IOCON,0b00100000);
	}
	_gpioDirection = 0xFF;//all in
	_gpioState = 0x00;//all low 
}



void mcp23s08::writeByte(byte addr, byte data){
	startSend(0);
	SPI.transfer(addr);
	SPI.transfer(data);
	endSend();
}



uint8_t mcp23s08::readAddress(byte addr){
	byte low_byte = 0x00;
	startSend(1);
	SPI.transfer(addr);
	low_byte  = SPI.transfer(0x0);
	endSend();
	return low_byte;
}



void mcp23s08::gpioPinMode(bool mode){
	if (mode == INPUT){
		_gpioDirection = 0xFF;
	} else {
		_gpioDirection = 0x00;
		_gpioState = 0x00;
	}
	writeByte(IODIR,_gpioDirection);
}


void mcp23s08::gpioPort(uint8_t value){
	_gpioState = value;
	writeByte(GPIO,_gpioState);
}


uint8_t mcp23s08::readGpioPort(){
	return readAddress(GPIO);
}

uint8_t mcp23s08::readGpioPortFast(){
	return _gpioState;
}

int mcp23s08::gpioDigitalReadFast(uint8_t pin){
	if (pin < 8){//0...7
		int temp = bitRead(_gpioState,pin);
		return temp;
	} else {
		return 0;
	}
}

void mcp23s08::gpioPinMode(uint8_t pin, bool mode){
	if (pin < 8){//0...7
		if (mode == INPUT){
			bitSet(_gpioDirection,pin);
		} else {
			bitClear(_gpioDirection,pin);
		}
		writeByte(IODIR,_gpioDirection);
	}
}


void mcp23s08::gpioDigitalWrite(uint8_t pin, bool value){
	if (pin < 8){//0...7
		if (value){
			bitSet(_gpioState,pin);
		} else {
			bitClear(_gpioState,pin);
		}
		writeByte(GPIO,_gpioState);
	}
}


int mcp23s08::gpioDigitalRead(uint8_t pin){
	if (pin < 8) return (int)(readAddress(GPIO) & 1 << pin);
	return 0;
}

unsigned int mcp23s08::gpioRegisterRead(byte reg){
  unsigned int data = 0;
    startSend(1);
    SPI.transfer(reg);
    data = SPI.transfer(0);
    endSend();
  return data;
}


void mcp23s08::gpioRegisterWrite(byte reg,byte data){
	writeByte(reg,data);
}

void mcp23s08::startSend(bool mode){
#if defined(__FASTSPI)
	digitalWriteFast(_cs, LOW);
#else
	digitalWrite(_cs, LOW);
#endif
	if (mode){//IN
		SPI.transfer(_readCmd);
	} else {//OUT
		SPI.transfer(_writeCmd);
	}
}

void mcp23s08::endSend(){
#if defined(__FASTSPI)
	digitalWriteFast(_cs, HIGH);
#else
	digitalWrite(_cs, HIGH);
#endif
}


void mcp23s08::portPullup(bool data) {
	if (data){
		_gpioState = 0xFF;
	} else {
		_gpioState = 0x00;
	}
	writeByte(GPPU, _gpioState);
}