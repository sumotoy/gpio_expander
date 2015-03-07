#include <stdio.h>

#include <inttypes.h>

#include <Arduino.h>

#include "max7301.h"
#include <../SPI/SPI.h>//this chip needs SPI

max7301::max7301(){
#if defined (SPI_HAS_TRANSACTION)
	_spiTransactionsSpeed = MAXSPISPEED;//set to max supported speed (in relation to chip and CPU)
#else
	_spiTransactionsSpeed = 0;
#endif
}

void max7301::setSPIspeed(uint32_t spispeed){
	#if defined (SPI_HAS_TRANSACTION)
	if (spispeed > 0){
		if (spispeed > MAXSPISPEED) {
			_spiTransactionsSpeed = MAXSPISPEED;
		} else {
			_spiTransactionsSpeed = spispeed;
		}
	} else {
		_spiTransactionsSpeed = 0;//disable SPItransactons
	}
	#else
	_spiTransactionsSpeed = 0;
	#endif
}


max7301::max7301(const uint8_t csPin,const uint8_t haenAdrs){
	_spiTransactionsSpeed = 0;
	postSetup(csPin,haenAdrs);
}

max7301::max7301(const uint8_t csPin,const uint8_t haenAdrs,uint32_t spispeed){
	setSPIspeed(spispeed);
	
	postSetup(csPin,haenAdrs);
}


void max7301::postSetup(const uint8_t csPin,const uint8_t haenAdrs){
	_cs = csPin;
	if (haenAdrs > 0x19 && haenAdrs < 0x28){//HAEN works between 0x20...0x27
		_adrs = haenAdrs;
		_useHaen = 1;
	} else {
		_adrs = 0;
		_useHaen = 0;
	}
	_readCmd =  (_adrs << 1) | 1;
	_writeCmd = _adrs << 1;
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


void max7301::begin(bool protocolInitOverride) {
	if (!protocolInitOverride){
		SPI.begin();
		#if defined (SPI_HAS_TRANSACTION)
		if (_spiTransactionsSpeed == 0){//do not use SPItransactons
			SPI.setClockDivider(SPI_CLOCK_DIV4); // 4 MHz (half speed)
			SPI.setBitOrder(MSBFIRST);
			SPI.setDataMode(SPI_MODE0);
		}
		#else//do not use SPItransactons
		SPI.setClockDivider(SPI_CLOCK_DIV4); // 4 MHz (half speed)
		SPI.setBitOrder(MSBFIRST);
		SPI.setDataMode(SPI_MODE0);
		#endif
	}	
	pinMode(_cs, OUTPUT);
	digitalWrite(_cs, HIGH);
	delay(100);
	/*
    if (_useHaen){
		writeByte(IOCON,0b00101000);//read datasheet for details!
	} else {
		writeByte(IOCON,0b00100000);
	}
	*/
	_gpioDirection = 0xFFFF;//all in
	_gpioState = 0xFFFF;//all low 
}





uint16_t max7301::readAddress(byte addr){
	byte low_byte = 0x00;
	startSend(1);
	SPI.transfer(addr);
	low_byte  = SPI.transfer(0x0);
	byte high_byte = SPI.transfer(0x0);
	endSend();
	return byte2word(high_byte,low_byte);
}



void max7301::gpioPinMode(uint16_t mode){
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

void max7301::gpioPinMode(uint8_t pin, bool mode){
	if (pin < 16){//0...15
		mode == INPUT ? _gpioDirection |= (1 << pin) :_gpioDirection &= ~(1 << pin);
		writeWord(IODIR,_gpioDirection);
	}
}


void max7301::gpioPort(uint16_t value){
	if (value == HIGH){
		_gpioState = 0xFFFF;
	} else if (value == LOW){	
		_gpioState = 0x0000;
	} else {
		_gpioState = value;
	}
	writeWord(GPIO,_gpioState);
}

void max7301::gpioPort(byte lowByte, byte highByte){
	_gpioState = byte2word(highByte,lowByte);
	writeWord(GPIO,_gpioState);
}


uint16_t max7301::readGpioPort(){
	return readAddress(GPIO);
}

uint16_t max7301::readGpioPortFast(){
	return _gpioState;
}

void max7301::portPullup(uint16_t data) {
	if (data == HIGH){
		_gpioState = 0xFFFF;
	} else if (data == LOW){	
		_gpioState = 0x0000;
	} else {
		_gpioState = data;
	}
	writeWord(GPPU, _gpioState);
}


void max7301::gpioDigitalWrite(uint8_t pin, bool value){
	if (pin < 16){//0...15
		value == HIGH ? _gpioState |= (1 << pin) : _gpioState &= ~(1 << pin);
		writeWord(GPIO,_gpioState);
	}
}

void max7301::gpioDigitalWriteFast(uint8_t pin, bool value){
	if (pin < 16){//0...15
		value == HIGH ? _gpioState |= (1 << pin) : _gpioState &= ~(1 << pin);
	}
}

void max7301::gpioPortUpdate(){
	writeWord(GPIO,_gpioState);
}

int max7301::gpioDigitalRead(uint8_t pin){
	if (pin < 16) return (int)(readAddress(GPIO) & 1 << pin);
	return 0;
}


int max7301::gpioDigitalReadFast(uint8_t pin){
	if (pin < 16){//0...15
		int temp = bitRead(_gpioState,pin);
		return temp;
	} else {
		return 0;
	}
}

uint8_t max7301::gpioRegisterReadByte(byte reg){
  uint8_t data = 0;
    startSend(1);
    SPI.transfer(reg);
    data = SPI.transfer(0);
    endSend();
  return data;
}

uint16_t max7301::gpioRegisterReadWord(byte reg){
  uint16_t data = 0;
    startSend(1);
    SPI.transfer(reg);
    data = SPI.transfer(0);
	data = SPI.transfer(0) << 8;
    endSend();
  return data;
}

void max7301::gpioRegisterWriteByte(byte reg,byte data){
	writeByte(reg,(byte)data);
}

void max7301::gpioRegisterWriteWord(byte reg,word data){
	writeWord(reg,(word)data);
}

/* ------------------------------ Low Level ----------------*/
void max7301::writeByte(byte addr, byte data){
	startSend(0);
	SPI.transfer(addr);
	SPI.transfer(data);
	endSend();
}

void max7301::writeWord(byte addr, uint16_t data){
	startSend(0);
	SPI.transfer(addr);
	SPI.transfer(word2lowByte(data));
	SPI.transfer(word2highByte(data));
	endSend();
}

void max7301::startSend(bool mode){
#if defined (SPI_HAS_TRANSACTION)
	if (_spiTransactionsSpeed > 0) SPI.beginTransaction(SPISettings(_spiTransactionsSpeed, MSBFIRST, SPI_MODE0));
#endif
#if defined(__FASTWRITE)
	digitalWriteFast(_cs, LOW);
#else
	digitalWrite(_cs, LOW);
#endif
	mode == 1 ? SPI.transfer(_readCmd) : SPI.transfer(_writeCmd);
	/*
	if (mode){//IN
		SPI.transfer(_readCmd);
	} else {//OUT
		SPI.transfer(_writeCmd);
	}
	*/
}

void max7301::endSend(){
#if defined(__FASTWRITE)
	digitalWriteFast(_cs, HIGH);
#else
	digitalWrite(_cs, HIGH);
#endif
#if defined (SPI_HAS_TRANSACTION)
	if (_spiTransactionsSpeed > 0) SPI.endTransaction();
#endif
}




