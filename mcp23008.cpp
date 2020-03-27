#include <stdio.h>

#include <inttypes.h>

#include <Arduino.h>

#include "mcp23008.h"
#include <Wire.h>//this chip uses wire

mcp23008::mcp23008(){

}

mcp23008::mcp23008(const uint8_t adrs){
	postSetup(adrs);
}

void mcp23008::postSetup(const uint8_t adrs){
	if (adrs >= 0x20 && adrs <= 0x27){// 0x20...0x27
		_adrs = adrs;
		_error = false;
	} else {
		_error = true;
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

void mcp23008::begin(bool protocolInitOverride) {
	if (!protocolInitOverride && !_error) {
		Wire.begin();
		#if ARDUINO >= 157
			Wire.setClock(400000UL); // Set I2C frequency to 400kHz
		#else
			TWBR = ((F_CPU / 400000UL) - 16) / 2; // Set I2C frequency to 400kHz
		#endif
	}
	delay(100);
	writeByte(IOCON,0b00100000);
	_gpioDirection = 0xFF;//all in
	_gpioState = 0x00;//all low 
}



uint8_t mcp23008::readAddress(byte addr){
	byte low_byte = 0;
	if (!_error){
		Wire.beginTransmission(_adrs);
		Wire.write(addr);
		Wire.endTransmission();
		Wire.requestFrom((uint8_t)_adrs,(uint8_t)1);
		low_byte = Wire.read();
	}
	return low_byte;
}



void mcp23008::gpioPinMode(uint8_t mode){
	if (mode == INPUT){
		_gpioDirection = 0xFF;
	} else if (mode == OUTPUT){	
		_gpioDirection = 0x00;
		_gpioState = 0x00;
	} else {
		_gpioDirection = mode;
	}
	writeByte(IODIR,_gpioDirection);
}

void mcp23008::gpioPinMode(uint8_t pin, bool mode){
	if (pin < 8){//0...7
		mode == INPUT ? _gpioDirection |= (1 << pin) :_gpioDirection &= ~(1 << pin);
		writeByte(IODIR,_gpioDirection);
	}
}


void mcp23008::gpioPort(uint8_t value){
	if (value == HIGH){
		_gpioState = 0xFF;
	} else if (value == LOW){	
		_gpioState = 0x00;
	} else {
		_gpioState = value;
	}
	writeByte(GPIO,_gpioState);
}


uint8_t mcp23008::readGpioPort(){
	return readAddress(GPIO);
}

uint8_t mcp23008::readGpioPortFast(){
	return _gpioState;
}

int mcp23008::gpioDigitalReadFast(uint8_t pin){
	int temp = 0;
	if (pin < 8) temp = bitRead(_gpioState,pin);//0...7
	return temp;
}

void mcp23008::portPullup(uint8_t data) {
	if (data == HIGH){
		_gpioState = 0xFF;
	} else if (data == LOW){	
		_gpioState = 0x00;
	} else {
		_gpioState = data;
	}
	writeByte(GPPU, _gpioState);
}



void mcp23008::gpioDigitalWrite(uint8_t pin, bool value){
	if (pin < 8){//0...7
		value == HIGH ? _gpioState |= (1 << pin) : _gpioState &= ~(1 << pin);
		writeByte(GPIO,_gpioState);
	}
}

void mcp23008::gpioDigitalWriteFast(uint8_t pin, bool value){
	if (pin < 8){//0...7
		value == HIGH ? _gpioState |= (1 << pin) : _gpioState &= ~(1 << pin);
	}
}

void mcp23008::gpioPortUpdate(){
	writeByte(GPIO,_gpioState);
}

int mcp23008::gpioDigitalRead(uint8_t pin){
	if (pin < 8) return (int)(readAddress(GPIO) & 1 << pin);
	return 0;
}

uint8_t mcp23008::gpioRegisterReadByte(byte reg){
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


void mcp23008::gpioRegisterWriteByte(byte reg,byte data){
	writeByte(reg,(byte)data);
}

/* ------------------------------ Low Level ----------------*/
void mcp23008::writeByte(byte addr, byte data){
	if (!_error){
		Wire.beginTransmission(_adrs);
		Wire.write(addr);
		Wire.write(data);
		Wire.endTransmission();
	}
}
