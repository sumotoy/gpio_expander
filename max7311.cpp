#include <stdio.h>

#include <inttypes.h>

#include <Arduino.h>

#include "max7311.h"
#include <../Wire/Wire.h>//this chip uses wire

max7311::max7311(){

}

max7311::max7311(const uint8_t adrs){
	postSetup(adrs);
}

void max7311::postSetup(const uint8_t adrs){
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
	IPOL = 		0x04;//Polarity Inversion Registers (0x04..0x05)
	GPTIM =     0x08;//Timeout register (bit 0)
}

void max7311::begin(bool protocolInitOverride) {
	if (!protocolInitOverride && !_error){
		Wire.begin();
	}
	delay(100);
	_gpioDirection = 0xFFFF;//all in
	_gpioState = 0x0000;//all low 
}


void max7311::writeByte(byte addr, byte data){
	if (!_error){
		Wire.beginTransmission(_adrs);
		Wire.write(addr);//witch register?
		Wire.write(data);
		Wire.endTransmission();
	}
}

void max7311::writeWord(byte addr, uint16_t data){
	if (!_error){
		Wire.beginTransmission(_adrs);
		Wire.write(addr);//witch register?
		Wire.write(word2lowByte(data));
		Wire.write(word2highByte(data));
		Wire.endTransmission();
	}
}

uint16_t max7311::readAddress(byte addr){
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



void max7311::gpioPinMode(bool mode){
	if (mode == INPUT){
		_gpioDirection = 0xFFFF;
		writeWord(GPIO,_gpioDirection);
	} else {
		_gpioDirection = 0x0000;
		_gpioState = 0x0000;
		writeWord(GPPU,_gpioDirection);
	}
}


void max7311::gpioPort(uint16_t value){
	_gpioState = value;
	writeWord(GPIO,_gpioState);
}

void max7311::gpioPort(byte lowByte, byte highByte){
	_gpioState = byte2word(highByte,lowByte);
	writeWord(GPIO,_gpioState);
}


uint16_t max7311::readGpioPort(){
	return readAddress(GPIO);
}

uint16_t max7311::readGpioPortFast(){
	return _gpioState;
}

int max7311::gpioDigitalReadFast(uint8_t pin){
	if (pin < 15){//0...15
		int temp = bitRead(_gpioState,pin);
		return temp;
	} else {
		return 0;
	}
}

void max7311::gpioPinMode(uint8_t pin, bool mode){
	if (pin < 15){//0...15
		if (mode == INPUT){
			bitSet(_gpioDirection,pin);
		} else {
			bitClear(_gpioDirection,pin);
		}
		writeWord(IODIR,_gpioDirection);
	}
}


void max7311::gpioDigitalWrite(uint8_t pin, bool value){
	if (pin < 15){//0...15
		if (value){
			bitSet(_gpioState,pin);
		} else {
			bitClear(_gpioState,pin);
		}
		writeWord(GPPU,_gpioState);
	}
}


int max7311::gpioDigitalRead(uint8_t pin){
	if (pin < 15) return (int)(readAddress(GPIO) & 1 << pin);
	return 0;
}

unsigned int max7311::gpioRegisterRead(byte reg){
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


void max7311::gpioRegisterWrite(byte reg,byte data){
	writeWord(reg,data);
}
