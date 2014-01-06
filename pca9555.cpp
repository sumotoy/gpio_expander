#include <stdio.h>

#include <inttypes.h>

#include <Arduino.h>

#include "pca9555.h"
#include <../Wire/Wire.h>//this chip uses wire

pca9555::pca9555(){

}

pca9555::pca9555(const uint8_t adrs){
	postSetup(adrs);
}

void pca9555::postSetup(const uint8_t adrs){
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

void pca9555::begin(bool protocolInitOverride) {
	if (!protocolInitOverride && !_error){
		Wire.begin();
	}
	delay(100);
	_gpioDirection = 0xFFFF;//all in
	_gpioState = 0x0000;//all low 
}


void pca9555::writeByte(byte addr, byte data){
	if (!_error){
		Wire.beginTransmission(_adrs);
		Wire.write(addr);//witch register?
		Wire.write(data);
		Wire.endTransmission();
	}
}

void pca9555::writeWord(byte addr, uint16_t data){
	if (!_error){
		Wire.beginTransmission(_adrs);
		Wire.write(addr);//witch register?
		Wire.write(word2lowByte(data));
		Wire.write(word2highByte(data));
		Wire.endTransmission();
	}
}

uint16_t pca9555::readAddress(byte addr){
	byte low_byte = 0;
	byte high_byte = 0;
	if (!_error){
		Wire.beginTransmission(_adrs);
		Wire.write(addr);//witch register?
		Wire.endTransmission();
		Wire.requestFrom((uint8_t)_adrs,(uint8_t)2);
		byte low_byte = Wire.read();
		byte high_byte = Wire.read();
	}	
	return byte2word(high_byte,low_byte);
}



void pca9555::gpioPinMode(bool mode){
	if (mode == INPUT){
		_gpioDirection = 0xFFFF;
		writeWord(GPIO,_gpioDirection);
	} else {
		_gpioDirection = 0x0000;
		_gpioState = 0x0000;
		writeWord(GPPU,_gpioDirection);
	}
}


void pca9555::gpioPort(uint16_t value){
	_gpioState = value;
	writeWord(GPIO,_gpioState);
}

void pca9555::gpioPort(byte lowByte, byte highByte){
	_gpioState = byte2word(highByte,lowByte);
	writeWord(GPIO,_gpioState);
}


uint16_t pca9555::readGpioPort(){
	return readAddress(GPIO);
}

uint16_t pca9555::readGpioPortFast(){
	return _gpioState;
}

int pca9555::gpioDigitalReadFast(uint8_t pin){
	if (pin < 15){//0...15
		int temp = bitRead(_gpioState,pin);
		return temp;
	} else {
		return 0;
	}
}

void pca9555::gpioPinMode(uint8_t pin, bool mode){
	if (pin < 15){//0...15
		if (mode == INPUT){
			bitSet(_gpioDirection,pin);
		} else {
			bitClear(_gpioDirection,pin);
		}
		writeWord(IODIR,_gpioDirection);
	}
}


void pca9555::gpioDigitalWrite(uint8_t pin, bool value){
	if (pin < 15){//0...15
		if (value){
			bitSet(_gpioState,pin);
		} else {
			bitClear(_gpioState,pin);
		}
		writeWord(GPPU,_gpioState);
	}
}


int pca9555::gpioDigitalRead(uint8_t pin){
	if (pin < 15) return (int)(readAddress(GPIO) & 1 << pin);
	return 0;
}

unsigned int pca9555::gpioRegisterRead(byte reg){
  unsigned int data = 0;
	if (!_error){
		Wire.beginTransmission(_adrs);
		Wire.write(reg);
		Wire.endTransmission();
		Wire.requestFrom((uint8_t)_adrs,(uint8_t)1);
		data = Wire.read();
	}
  return data;
}


void pca9555::gpioRegisterWrite(byte reg,byte data){
	writeWord(reg,data);
}
