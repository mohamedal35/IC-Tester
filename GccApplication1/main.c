#include <util/twi.h>
#include "twi_lcd.h"
#include "ics_conf.c"


#include <util/delay.h>


int main() {
	    // Create an array of function pointers
	int (*ICsfunctionsArray[])() = {IC_7442};

	// Determine the size of the array
	uint8_t numFunctions = sizeof(ICsfunctionsArray) / sizeof(ICsfunctionsArray[0]);
	// Run the IC 7442 test
	while (1) {
		for (uint8_t i = 0; i < numFunctions; i++) {
			// Call each function in the array
			if (ICsfunctionsArray[i]()) {
				if (i == 0) {
					
					twi_init();
					twi_lcd_init();
					twi_lcd_msg("found: 7442 IC");
					_delay_ms(500);

				}
			}else {
			}
		}
	}
	return 0;
}