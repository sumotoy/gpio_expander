#include <stdio.h>

#include <inttypes.h>

#include <Arduino.h>

#include "max7318.h"
#include <../Wire/Wire.h>//this chip uses wire

max7318::max7318(){
}

max7318::max7318(const uint8_t adrs){
	postSetup(adrs);
}

void max7318::postSetup(const uint8_t adrs){
	if (adrs >= 0x20 && adrs <= 0x27){//basic addressing
		_adrs = adrs;
		_error = false;
	} else if (adrs >= 0x28 && adrs <= 0xDE){//advanced addressing
		//todo
		_error = false;
	} else {
		_error = true;
	}
	//setup register values for this chip
	IODIR = 	0x06;//Configuration Registers (0x06..0x07)
	GPPU = 		0x02;//Output Port Registers (0x02..0x03)
	GPIO = 		0x00;//Input Port Registers (0x00..0x01)
	IPOL = 		0x04;//Polarity Inversion Registers (0x04..0x05)(
}

void max7318::begin(bool protocolInitOverride) {
	if (!protocolInitOverride && !_error){
		Wire.begin();
	}
	delay(100);
	_gpioDirection = 0xFFFF;//all in
	_gpioState = 0x0000;//all low 
}




uint16_t max7318::readAddress(byte addr){
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



void max7318::gpioPinMode(uint16_t mode){
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

void max7318::gpioPinMode(uint8_t pin, bool mode){
	if (pin < 15){//0...15
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

void max7318::gpioPort(uint16_t value){
	if (value == HIGH){
		_gpioState = 0xFFFF;
	} else if (value == LOW){	
		_gpioState = 0x0000;
	} else {
		_gpioState = value;
	}
	writeWord(GPIO,_gpioState);
}

void max7318::gpioPort(byte lowByte, byte highByte){
	_gpioState = byte2word(highByte,lowByte);
	writeWord(GPIO,_gpioState);
}


uint16_t max7318::readGpioPort(){
	return readAddress(GPIO);
}

uint16_t max7318::readGpioPortFast(){
	return _gpioState;
}

int max7318::gpioDigitalReadFast(uint8_t pin){
	int temp = 0;
	if (pin < 15) temp = bitRead(_gpioState,pin);
	return temp;
}




void max7318::gpioDigitalWrite(uint8_t pin, bool value){
	if (pin < 15){//0...15
		value == HIGH ? _gpioState |= (1 << pin) : _gpioState &= ~(1 << pin);
		/*
		if (value){
			bitSet(_gpioState,pin);
		} else {
			bitClear(_gpioState,pin);
		}
		*/
		writeWord(GPPU,_gpioState);
	}
}


int max7318::gpioDigitalRead(uint8_t pin){
	if (pin < 15) return (int)(readAddress(GPIO) & 1 << pin);
	return 0;
}

uint8_t max7318::gpioRegisterReadByte(byte reg){
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


void max7318::gpioRegisterWriteByte(byte reg,byte data){
	writeByte(reg,(byte)data);
}

void max7318::gpioRegisterWriteWord(byte reg,word data){
	writeWord(reg,(word)data);
}

/* ------------------------------ Low Level ----------------*/
void max7318::writeByte(byte addr, byte data){
	if (!_error){
		Wire.beginTransmission(_adrs);
		Wire.write(addr);//witch register?
		Wire.write(data);
		Wire.endTransmission();
	}
}

void max7318::writeWord(byte addr, uint16_t data){
	if (!_error){
		Wire.beginTransmission(_adrs);
		Wire.write(addr);//witch register?
		Wire.write(word2lowByte(data));
		Wire.write(word2highByte(data));
		Wire.endTransmission();
	}
}
