#include <stdio.h>

#include <inttypes.h>

#if defined(ENERGIA) // LaunchPad, FraunchPad and StellarPad specific
#include "Energia.h"
#else
#include "Arduino.h"
#endif

#include "mcp23017.h"
#include <../Wire/Wire.h>

mcp23017::mcp23017(){
	
}


mcp23017::mcp23017(const uint8_t adrs){
	postSetup(adrs);
}

void mcp23017::postSetup(const uint8_t adrs){
	if (adrs >= 0x20 && adrs <= 0x27){//HAEN works between 0x20...0x27
		_adrs = adrs;
		_error = false;
	} else {
		_error = true;
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

void mcp23017::begin(bool protocolInitOverride) {
	if (!protocolInitOverride && !_error){
		Wire.begin();
	}
	delay(100);
	writeByte(IOCON,0b00100000);//read datasheet for details!
	_gpioDirection = 0xFFFF;//all in
	_gpioState = 0x0000;//all low 
}




uint16_t mcp23017::readAddress(byte addr){
	byte low_byte = 0;
	byte high_byte = 0;
	if (!_error){
		Wire.beginTransmission(_adrs);
		Wire.write(addr);//witch register?
		Wire.endTransmission();
		Wire.requestFrom((uint8_t)_adrs,(uint8_t)2);
		low_byte = Wire.read();
		high_byte = Wire.read();
	}
	return byte2word(high_byte,low_byte);
}



void mcp23017::gpioPinMode(uint16_t mode){
	if (mode == INPUT){
		_gpioDirection = 0xFFFF;
	} else if (mode == OUTPUT){	
		_gpioDirection = 0x0000;
		_gpioState = 0x0000;
	} else {
		_gpioDirection = mode;
	}
	writeWord(IODIR,_gpioDirection);
}

void mcp23017::gpioPinMode(uint8_t pin, bool mode){
	if (pin < 16){//0...15
		mode == INPUT ? _gpioDirection |= (1 << pin) :_gpioDirection &= ~(1 << pin);
		/*
		if (mode == INPUT){
			bitSet(_gpioDirection,pin);
		} else {
			bitClear(_gpioDirection,pin);
		}
		*/
		writeWord(IODIR,_gpioDirection);
	}
}

void mcp23017::gpioPort(uint16_t value){
	if (value == HIGH){
		_gpioState = 0xFFFF;
	} else if (value == LOW){	
		_gpioState = 0x0000;
	} else {
		_gpioState = value;
	}
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
	int temp = 0;
	if (pin < 16) temp = bitRead(_gpioState,pin);
	return temp;
}


void mcp23017::portPullup(uint16_t data) {
	if (data == HIGH){
		_gpioState = 0xFFFF;
	} else if (data == LOW){	
		_gpioState = 0x0000;
	} else {
		_gpioState = data;
	}
	writeWord(GPPU, _gpioState);
}

void mcp23017::gpioDigitalWrite(uint8_t pin, bool value){
	if (pin < 16){//0...15
		value == HIGH ? _gpioState |= (1 << pin) : _gpioState &= ~(1 << pin);
		/*
		if (value){
			bitSet(_gpioState,pin);
		} else {
			bitClear(_gpioState,pin);
		}
		*/
		writeWord(GPIO,_gpioState);
	}
}


int mcp23017::gpioDigitalRead(uint8_t pin){
	if (pin < 16) return (int)(readAddress(GPIO) & 1 << pin);
	return 0;
}

uint8_t mcp23017::gpioRegisterReadByte(byte reg){
  uint8_t data = 0;
	if (!_error){
		Wire.beginTransmission(_adrs);
		Wire.write(reg);
		Wire.endTransmission();
		Wire.requestFrom((uint8_t)_adrs,(uint8_t)1);
		data = Wire.read();
	}
  return data;
}


uint16_t mcp23017::gpioRegisterReadWord(byte reg){
  uint16_t data = 0;
	if (!_error){
		Wire.beginTransmission(_adrs);
		Wire.write(reg);
		Wire.endTransmission();
		Wire.requestFrom((uint8_t)_adrs,(uint8_t)1);
		data = Wire.read();
		data = Wire.read() << 8;
	}
  return data;
}


void mcp23017::gpioRegisterWriteByte(byte reg,byte data){
	writeByte(reg,(byte)data);
}


void mcp23017::gpioRegisterWriteWord(byte reg,word data){
	writeWord(reg,(word)data);
}

/* ------------------------------ Low Level ----------------*/

void mcp23017::writeByte(byte addr, byte data){
	if (!_error){
		Wire.beginTransmission(_adrs);
		Wire.write(addr);//witch register?
		Wire.write(data);
		Wire.endTransmission();
	}
}

void mcp23017::writeWord(byte addr, uint16_t data){
	if (!_error){
		Wire.beginTransmission(_adrs);
		Wire.write(addr);//witch register?
		Wire.write(word2lowByte(data));
		Wire.write(word2highByte(data));
		Wire.endTransmission();
	}	
}


