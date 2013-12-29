#ifndef _GPIO_EXPANDER_H_
#define _GPIO_EXPANDER_H_

#include <inttypes.h>
#include <Arduino.h>


class gpio_expander {

public:
	gpio_expander( );

	virtual void 	begin(uint8_t protocolInitOverride=0) = 0;

	void 			command();								

	
protected:

	inline uint16_t		byte2word(byte high_byte,byte low_byte){return (word)high_byte << 8 | (word)low_byte;};
	inline byte			word2highByte(uint16_t data){return (byte)(data >> 8);};
	inline byte			word2lowByte(uint16_t data){return (byte)(data & 0x00FF);};
private:

};
#endif