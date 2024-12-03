#include "ics_conf.h"
#include <util/delay.h>

#include <stdio.h>
#include <string.h>


uint16_t bcd_decoder_active_low(uint8_t input) {
	if (input > 9) {
		// Invalid BCD input, return all HIGH (inactive state)
		return 0xFFFF; // 16 bits, all HIGH
	}

	// Set all outputs HIGH
	uint16_t output = 0x03FF; // 10 bits, all HIGH (O0-O9)

	// Set the corresponding output LOW
	output &= ~(1 << input); // Clear the bit corresponding to the decimal value

	return output;
}


int IC_7442() {
		// Configure PA1 to PA4 as outputs
		DDRA |= (1 << PA1) | (1 << PA2) | (1 << PA3) | (1 << PA4);

		// Configure PB0 to PB6 and PA5 to PA7 as inputs
		DDRB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3) |
		(1 << PB4) | (1 << PB5) | (1 << PB6));
		DDRA &= ~((1 << PA5) | (1 << PA6) | (1 << PA7));

		// Optional: Enable pull-up resistors (if required)
		PORTB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3) |
		(1 << PB4) | (1 << PB5) | (1 << PB6);
		PORTA |= (1 << PA5) | (1 << PA6) | (1 << PA7);

		uint8_t count = 0;      // Counter for PA1 to PA4 output
		uint8_t passed = 1;      // Counter for PA1 to PA4 output
		uint16_t combined_input; // Variable to store the 10-bit combined input
		
		while (1) {
			// --- Step 1: Output 0 to 9 on PA1 to PA4 ---
			PORTA = (PORTA & 0xE1) | ((count << 1) & 0x1E); // Update PA4-PA1
			
			uint16_t expected = bcd_decoder_active_low(count);
			count++;

			if (count > 9) {
				break;
			}

			// Short delay for visibility of the output
			_delay_ms(200);

			// --- Step 2: Read inputs from PB0 to PB6 and PA5 to PA7 ---
			uint8_t portb_data = PINB & 0x7F;         // Mask PB0 to PB6
			uint8_t porta_data = (PINA >> 5) & 0x07; // Mask and shift PA5 to PA7
			combined_input = (porta_data << 7) | portb_data;

			if (combined_input != expected) {
				passed = 0;
				break;
			}
			// Optional: Use `combined_input` for further logic
			// For example: Debugging or transmission can be added here

			// Small delay before the next cycle
			_delay_ms(100);
		}
		return passed;
}
