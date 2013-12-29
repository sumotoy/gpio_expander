#include <stdio.h>

#include <inttypes.h>

#include <Arduino.h>

#include "mcp23008.h"
#include <../Wire/Wire.h>//this chip uses wire



mcp23008::mcp23008(const uint8_t adrs){

	if (adrs >= 0x20 && adrs <= 0x27){// 0x20...0x27
		_adrs = adrs;
	} else {
		_adrs = 0;
	}
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


void mcp23008::begin(uint8_t protocolInitOverride) {
	if (!protocolInitOverride){
		Wire.begin();
	}	

	delay(100);
	writeByte(IOCON,0b00100000);
	_gpioDirection = 0xFF;//all in
	_gpioState = 0x00;//all low 
}



void mcp23008::writeByte(byte addr, byte data){
	Wire.beginTransmission(addr);
	Wire.write(data);
	Wire.endTransmission();
}



uint8_t mcp23008::readAddress(byte addr){
	Wire.beginTransmission(_adrs);
	Wire.write(addr);
	Wire.endTransmission();
    Wire.requestFrom((uint8_t)_adrs,(uint8_t)1);
	byte low_byte = Wire.read();
	return low_byte;
}



void mcp23008::gpioPinMode(bool mode){
	if (mode == INPUT){
		_gpioDirection = 0xFF;
	} else {
		_gpioDirection = 0x00;
		_gpioState = 0x00;
	}
	writeByte(IODIR,_gpioDirection);
}


void mcp23008::gpioPort(uint8_t value){
	_gpioState = value;
	writeByte(GPIO,_gpioState);
}


uint8_t mcp23008::readGpioPort(){
	return readAddress(GPIO);
}

uint8_t mcp23008::readGpioPortFast(){
	return _gpioState;
}

int mcp23008::gpioDigitalReadFast(uint8_t pin){
	if (pin < 7){//0...7
		int temp = bitRead(_gpioState,pin);
		return temp;
	} else {
		return 0;
	}
}

void mcp23008::gpioPinMode(uint8_t pin, bool mode){
	if (pin < 7){//0...7
		if (mode == INPUT){
			bitSet(_gpioDirection,pin);
		} else {
			bitClear(_gpioDirection,pin);
		}
		writeByte(IODIR,_gpioDirection);
	}
}


void mcp23008::gpioDigitalWrite(uint8_t pin, bool value){
	if (pin < 7){//0...7
		if (value){
			bitSet(_gpioState,pin);
		} else {
			bitClear(_gpioState,pin);
		}
		writeByte(GPIO,_gpioState);
	}
}


int mcp23008::gpioDigitalRead(uint8_t pin){
	if (pin < 7) return (int)(readAddress(GPIO) & 1 << pin);
	return 0;
}

unsigned int mcp23008::gpioRegisterRead(byte reg){
  unsigned int data = 0;
	Wire.beginTransmission(_adrs);
	Wire.write(reg);
	Wire.endTransmission();
	Wire.requestFrom((uint8_t)_adrs,(uint8_t)1);
	data = Wire.read();
  return data;
}


void mcp23008::gpioRegisterWrite(byte reg,byte data){
	writeByte(reg,data);
}
