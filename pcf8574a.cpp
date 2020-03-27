#include <stdio.h>

#include <inttypes.h>

#include <Arduino.h>

#include "pcf8574a.h"
#include <Wire.h>//this chip uses wire

pcf8574a::pcf8574a(){

}

pcf8574a::pcf8574a(const uint8_t adrs){
	postSetup(adrs);
	_gpioDirection = 0;
	_gpioInputs = 0;
	_gpioState = 0;
}

void pcf8574a::postSetup(const uint8_t adrs){
	if (adrs >= 0x38 && adrs <= 0x3F){// 0x38...0x3F for A variant!
		_adrs = adrs;
		_error = false;
	} else {
		_error = true;
	}
}

void pcf8574a::begin(bool protocolInitOverride) {
	if (!protocolInitOverride && !_error) {
		Wire.begin();
		#if ARDUINO >= 157
			Wire.setClock(400000UL); // Set I2C frequency to 400kHz
		#else
			TWBR = ((F_CPU / 400000UL) - 16) / 2; // Set I2C frequency to 400kHz
		#endif
	}
	delay(100);
	readGpio();
}



void pcf8574a::gpioPinMode(uint8_t mode){
	if (mode == INPUT){
		_gpioDirection = 0xFF;
	} else if (mode == INPUT_PULLUP){	
		_gpioDirection = 0x00;
		_gpioState = 0xFF;
	} else if (mode == OUTPUT){	
		_gpioDirection = 0x00;
		_gpioState = 0x00;
	} else {
		_gpioDirection = mode;
	}
	updateGpio();
}

void pcf8574a::gpioPinMode(uint8_t pin, uint8_t mode){
	if (pin < 8){//0...7
		switch (mode) {
			case INPUT:
			_gpioDirection &= ~(1 << pin);
			_gpioState &= ~(1 << pin);
		break;
		case INPUT_PULLUP:
			_gpioDirection &= ~(1 << pin);
			_gpioState |= (1 << pin);
		break;
		case OUTPUT:
			_gpioDirection |= (1 << pin);
			_gpioState &= ~(1 << pin);
		break;
		default:
		break;
		}
		updateGpio();
	}
}


void pcf8574a::gpioPort(uint8_t value){
	if (value == HIGH){
		_gpioState = 0xFF;
	} else if (value == LOW){	
		_gpioState = 0x00;
	} else {
		_gpioState = value;
	}
	updateGpio();
}


uint8_t pcf8574a::readGpioPort(){
	readGpio();
	return _gpioState;
}

uint8_t pcf8574a::readGpioPortFast(){
	return _gpioState;
}

uint8_t pcf8574a::gpioDigitalRead(uint8_t pin){
	readGpio();
	return (_gpioState & (1 << pin)) ? HIGH : LOW;
}

uint8_t pcf8574a::gpioDigitalReadFast(uint8_t pin){
	int temp = 0;
	if (pin < 8) temp = bitRead(_gpioState,pin);//0...7
	return temp;
}

void pcf8574a::gpioPinToggle(uint8_t pin) {
	_gpioState ^= (1 << pin);
	updateGpio();
}


void pcf8574a::gpioDigitalWrite(uint8_t pin, bool value){
	if (pin < 8){//0...7
		value == HIGH ? _gpioState |= (1 << pin) : _gpioState &= ~(1 << pin);
		updateGpio();
	}
}

void pcf8574a::gpioDigitalWriteFast(uint8_t pin, bool value){
	if (pin < 8){//0...7
		value == HIGH ? _gpioState |= (1 << pin) : _gpioState &= ~(1 << pin);
	}
}

void pcf8574a::gpioPortUpdate(){
	if (!_error){
		//uint8_t data = (_gpioInputs & ~_gpioDirection) | _gpioState;
		Wire.beginTransmission(_adrs);
		Wire.write(_gpioState);
		Wire.endTransmission();
	}
}

void pcf8574a::updateGpio(){
	if (!_error){
		uint8_t data = (_gpioInputs & ~_gpioDirection) | _gpioState;
		Wire.beginTransmission(_adrs);
		Wire.write(data);
		Wire.endTransmission();
	}
}

void pcf8574a::readGpio(){
	if (!_error){
		Wire.requestFrom((uint8_t)_adrs,(uint8_t)1);
		_gpioState = Wire.read();
	}
}
