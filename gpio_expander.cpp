#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#if defined(ENERGIA) // LaunchPad, FraunchPad and StellarPad specific
#include "Energia.h"
#else
#include "Arduino.h"
#endif

#ifdef __AVR__
#include <avr/interrupt.h>
#endif


#include "gpio_expander.h"

gpio_expander::gpio_expander() 
{
 
}






