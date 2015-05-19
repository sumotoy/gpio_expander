#ifndef _GPIO_SPI_PARAM_H_
#define _GPIO_SPI_PARAM_H_

//defining max SPI speed (not definitive and can change in relation to chip used)
#if defined (SPI_HAS_TRANSACTION)
#if defined(__MK20DX128__) || defined(__MK20DX256__)//Teensy 3.0 or 3.1
#define MAXSPISPEED				30000000
#elif defined(__MKL26Z64__) //Teensy LC
#define MAXSPISPEED				12000000
#elif defined(ARDUINO) && defined(__arm__) && !defined(CORE_TEENSY)	//DUE	
#define MAXSPISPEED				24000000
#elif defined(__32MX320F128H__) || defined(__32MX795F512L__) //uno and max	chipkit	
#define MAXSPISPEED				8000000
#elif defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
#define MAXSPISPEED				2000000
#elif defined (__AVR_ATmega328P__) || defined (__AVR_ATmega168P__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644PA__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__)
#define MAXSPISPEED				8000000
#else
#define MAXSPISPEED				2000000
#endif
#endif

#endif
