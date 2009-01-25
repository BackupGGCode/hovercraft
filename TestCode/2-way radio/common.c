#include "common.h"
#include <util/delay.h>

void delayFor(unsigned int milliseconds){
	unsigned int l=(milliseconds/40);
	unsigned int i;
	for(i=0; i < l; ++i) _delay_ms(40);
}
