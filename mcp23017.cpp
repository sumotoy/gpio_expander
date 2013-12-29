#include <stdio.h>

#include <inttypes.h>

#include <Arduino.h>

#include "mcp23017.h"
#include <../Wire/Wire.h>//this chip uses wire



mcp23017::mcp23017(const uint8_t adrs){
	if (adrs >= 0x20 && adrs <= 0x27){//HAEN works between 0x20...0x27
		_adrs = adrs;
	} else {
		_adrs = 0x20;
	}
	//setup register values for this chip
	IOCON = 	0x0A;
	IODIR = 	0x00;
	GPPU = 		0x0C;
	GPIO = 		0x12;
	GPINTEN = 	0x04;
	IPOL = 		0x02;
	DEFVAL = 	0x06;
	INTF = 		0x0E;
	INTCAP = 	0x10;
	OLAT = 		0x14;
	INTCON = 	0x08;
}


void mcp23017::begin(uint8_t protocolInitOverride) {
	if (!protocolInitOverride){
		Wire.begin();
	}	
	delay(100);
	writeByte(IOCON,0b00100000);//read datasheet for details!
	_gpioDirection = 0xFFFF;//all in
	_gpioState = 0x0000;//all low 
}



void mcp23017::writeByte(byte addr, byte data){
	Wire.beginTransmission(addr);
	Wire.write(data);
	Wire.endTransmission();
}

void mcp23017::writeWord(byte addr, word data){
	//startSend(0);
	Wire.beginTransmission(addr);
	Wire.write(word2lowByte(data));
	Wire.write(word2highByte(data));
	Wire.endTransmission();
}

uint16_t mcp23017::readAddress(byte addr){
	Wire.beginTransmission(_adrs);
	Wire.write(addr);
	Wire.endTransmission();
    Wire.requestFrom((uint8_t)_adrs,(uint8_t)2);
	byte low_byte = Wire.read();
	byte high_byte = Wire.read();
	return byte2word(high_byte,low_byte);
}



void mcp23017::gpioPinMode(bool mode){
	if (mode == INPUT){
		_gpioDirection = 0xFFFF;
	} else {
		_gpioDirection = 0x0000;
		_gpioState = 0x0000;
	}
	writeWord(IODIR,_gpioDirection);
}


void mcp23017::gpioPort(uint16_t value){
	_gpioState = value;
	writeWord(GPIO,_gpioState);
}

void mcp23017::gpioPort(byte lowByte, byte highByte){
	_gpioState = byte2word(highByte,lowByte);
	writeWord(GPIO,_gpioState);
}


uint16_t mcp23017::readGpioPort(){
	return readAddress(GPIO);
}

uint16_t mcp23017::readGpioPortFast(){
	return _gpioState;
}

int mcp23017::gpioDigitalReadFast(uint8_t pin){
	if (pin < 15){//0...15
		int temp = bitRead(_gpioState,pin);
		return temp;
	} else {
		return 0;
	}
}

void mcp23017::gpioPinMode(uint8_t pin, bool mode){
	if (pin < 15){//0...15
		if (mode == INPUT){
			bitSet(_gpioDirection,pin);
		} else {
			bitClear(_gpioDirection,pin);
		}
		writeWord(IODIR,_gpioDirection);
	}
}


void mcp23017::gpioDigitalWrite(uint8_t pin, bool value){
	if (pin < 15){//0...15
		if (value){
			bitSet(_gpioState,pin);
		} else {
			bitClear(_gpioState,pin);
		}
		writeWord(GPIO,_gpioState);
	}
}


int mcp23017::gpioDigitalRead(uint8_t pin){
	if (pin < 15) return (int)(readAddress(GPIO) & 1 << pin);
	return 0;
}

unsigned int mcp23017::gpioRegisterRead(byte reg){
  unsigned int data = 0;
	Wire.beginTransmission(_adrs);
	Wire.write(reg);
	Wire.endTransmission();
	Wire.requestFrom((uint8_t)_adrs,(uint8_t)1);
	data = Wire.read();
  return data;
}


void mcp23017::gpioRegisterWrite(byte reg,byte data){
	writeWord(reg,data);
}

