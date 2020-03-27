#include <stdio.h>

#include <inttypes.h>

#include <Arduino.h>

#include "mcp23016.h"
#include <Wire.h>//this chip uses wire

mcp23016::mcp23016(){

}

mcp23016::mcp23016(const uint8_t adrs){
	postSetup(adrs);
}

void mcp23016::postSetup(const uint8_t adrs){
	if (adrs >= 0x20 && adrs <= 0x27){//HAEN works between 0x20...0x27
		_adrs = adrs;
		_error = false;
	} else {
		_error = true;
	}
	//setup register values for this chip
	IOCON = 	0x0A;
	IODIR = 	0x06;
	GPIO = 		0x00;
	IPOL = 		0x04;
	INTCAP = 	0x08;
	OLAT = 		0x02;
}

void mcp23016::begin(bool protocolInitOverride) {
	if (!protocolInitOverride && !_error){
		Wire.begin();
		#if ARDUINO >= 157
			Wire.setClock(400000UL); // Set I2C frequency to 400kHz
		#else
			TWBR = ((F_CPU / 400000UL) - 16) / 2; // Set I2C frequency to 400kHz
		#endif
	}	
	delay(100);
	writeByte(IOCON,0b00000000);//read datasheet for details!
	_gpioDirection = 0xFFFF;//all in
	_gpioState = 0x0000;//all low 
}





uint16_t mcp23016::readAddress(byte addr){
	byte low_byte = 0;
	byte high_byte = 0;
	if (!_error){
		Wire.beginTransmission(_adrs);
		Wire.write(addr);
		Wire.endTransmission();
		Wire.requestFrom((uint8_t)_adrs,(uint8_t)2);
		low_byte = Wire.read();
		high_byte = Wire.read();
	}
	return byte2word(high_byte,low_byte);
}



void mcp23016::gpioPinMode(uint16_t mode){
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

void mcp23016::gpioPinMode(uint8_t pin, bool mode){
	if (pin < 16){//0...15
		mode == INPUT ? _gpioDirection |= (1 << pin) :_gpioDirection &= ~(1 << pin);
		writeWord(IODIR,_gpioDirection);
	}
}

void mcp23016::gpioPort(uint16_t value){
	if (value == HIGH){
		_gpioState = 0xFFFF;
	} else if (value == LOW){	
		_gpioState = 0x0000;
	} else {
		_gpioState = value;
	}
	writeWord(GPIO,_gpioState);
}

void mcp23016::gpioPort(byte lowByte, byte highByte){
	_gpioState = byte2word(highByte,lowByte);
	writeWord(GPIO,_gpioState);
}


uint16_t mcp23016::readGpioPort(){
	return readAddress(GPIO);
}

uint16_t mcp23016::readGpioPortFast(){
	return _gpioState;
}

int mcp23016::gpioDigitalReadFast(uint8_t pin){
	int temp = 0;
	if (pin < 16) temp = bitRead(_gpioState,pin);
	return temp;
}




void mcp23016::gpioDigitalWrite(uint8_t pin, bool value){
	if (pin < 16){//0...15
		value == HIGH ? _gpioState |= (1 << pin) : _gpioState &= ~(1 << pin);
		writeWord(GPIO,_gpioState);
	}
}

void mcp23016::gpioDigitalWriteFast(uint8_t pin, bool value){
	if (pin < 16){//0...15
		value == HIGH ? _gpioState |= (1 << pin) : _gpioState &= ~(1 << pin);
	}
}

void mcp23016::gpioPortUpdate(){
	writeWord(GPIO,_gpioState);
}

int mcp23016::gpioDigitalRead(uint8_t pin){
	if (pin < 16) return (int)(readAddress(GPIO) & 1 << pin);
	return 0;
}

uint8_t mcp23016::gpioRegisterReadByte(byte reg){
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

uint16_t mcp23016::gpioRegisterReadWord(byte reg){
  uint16_t data = 0;
    if (!_error){
		Wire.beginTransmission(_adrs);
		Wire.write(reg);
		Wire.endTransmission();
		Wire.requestFrom((uint8_t)_adrs,(uint8_t)1);
		data = Wire.read();
		data += Wire.read() << 8;//Ironicster
	}
  return data;
}

void mcp23016::gpioRegisterWriteByte(byte reg,byte data){
	writeByte(reg,(byte)data);
}

void mcp23016::gpioRegisterWriteWord(byte reg,word data){
	writeWord(reg,(word)data);
}


/* ------------------------------ Low Level ----------------*/

void mcp23016::writeByte(byte addr, byte data){
	if (!_error){
		Wire.beginTransmission(_adrs);
		Wire.write(addr);
		Wire.write(data);
		Wire.endTransmission();
	}
}

void mcp23016::writeWord(byte addr, uint16_t data){
	if (!_error){
		Wire.beginTransmission(_adrs);
		Wire.write(addr);
		//Wire.write(word2lowByte(data));
		//Wire.write(word2highByte(data));
		Wire.write(data >> 8);
		Wire.write(data & 0xFF);
		Wire.endTransmission();
	}
}