#include <util/twi.h>
#include "twi_lcd.h"
#include "ics_conf.c"
#include <util/delay.h>


int main() {
	    // Create an array of function pointers
	int (*ICsfunctionsArray[])() = {IC_7442, IC_7421};

	// Determine the size of the array
	uint8_t numFunctions = sizeof(ICsfunctionsArray) / sizeof(ICsfunctionsArray[0]);
	uint8_t found = 0;
	// Run the IC 7442 test
	while (1) {
		for (uint8_t i = 0; i < numFunctions; i++) {
			// Call each function in the array
			if (ICsfunctionsArray[i]()) {
				if (i == 0) {
					found = 1;
					twi_init();
					twi_lcd_init();
					twi_lcd_msg("found: 7442 IC");
					_delay_ms(500);

				}else if (i == 1) {
					found = 1;
					twi_init();
					twi_lcd_init();
					twi_lcd_msg("found: 7421 IC");
					_delay_ms(500);
				}
			}

		}
		if (!found) {
			twi_init();
			twi_lcd_init();
			twi_lcd_msg("Unknown IC");
			_delay_ms(500);
		}
		found = 0;
	}
	return 0;
}