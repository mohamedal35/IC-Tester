#include "ics_conf.h"
#include <util/delay.h>

void IC_7442() {
	BCD_DDR |= BCD_MASK;
	

	DDRA &= ~OUTPUT_MASK_A;
	DDRB &= ~OUTPUT_MASK_B;
	PORTA &= ~OUTPUT_MASK_A;
	PORTB &= ~OUTPUT_MASK_B;
	LED_DDR |= LED_MASK;
	LED_PORT &= ~LED_MASK; 

	while (1) {
		for (uint8_t bcd = 0; bcd <= 9; bcd++) {

			BCD_PORT = (BCD_PORT & ~BCD_MASK) | (bcd & BCD_MASK);
			
			_delay_ms(100);
			

			uint8_t high_nibble = (OUTPUT_PORT_A & OUTPUT_MASK_A) >> 4;
			uint8_t low_nibble = (OUTPUT_PORT_B & OUTPUT_MASK_B);
			uint8_t outputs = (high_nibble << 4) | low_nibble;
			
			if ((outputs & (1 << bcd)) != 0) {
				LED_PORT |= LED_MASK;
				} else {
				LED_PORT &= ~LED_MASK;
			}
		}
	}
}
