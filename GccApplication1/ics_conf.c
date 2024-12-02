#include "ics_conf.h"
#include <util/delay.h>

#include <stdio.h>
#include <string.h>
int IC_7442() {
	BCD_DDR |= BCD_MASK;
	

	DDRA &= ~OUTPUT_MASK_A;
	DDRB &= ~OUTPUT_MASK_B;
	PORTA &= ~OUTPUT_MASK_A;
	PORTB &= ~OUTPUT_MASK_B;
	LED_DDR |= LED_MASK;
	LED_PORT &= ~LED_MASK; 
	
	uint8_t passed = 1;
	for (uint8_t bcd = 0; bcd <= 9; bcd++) {

		BCD_PORT = (BCD_PORT & ~BCD_MASK) | (bcd & BCD_MASK);
			
		_delay_ms(100);
			

		
	    uint8_t low_nibble = (OUTPUT_PORT_A & OUTPUT_MASK_A) >> 4; // Extract PA4-PA7
	    uint8_t high_nibble = (OUTPUT_PORT_B & OUTPUT_MASK_B);     // Extract PB0-PB5
	    uint8_t outputs = (high_nibble << 4) | low_nibble;         // Combine outputs (high nibble << 4)
		// 
		uint8_t expected = (~(1 << bcd)); // All bits 1 except the active one (active low)
		if (outputs != expected) {
			
			char baseStr[50] = "expected:";
			char intStr[20];
			sprintf(intStr, "%d", expected); // Convert the integer to a string
			strcat(baseStr, intStr);
			
			twi_init();
			twi_lcd_init();
			twi_lcd_msg(baseStr);
			
			passed = 0;
			break;
		}
	}
	
	return passed;
	
}
