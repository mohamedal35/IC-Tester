#include "ics_conf.h"
#include <util/delay.h>
#include <stdbool.h>
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
// Function to calculate logical AND of the 4 bits
bool calculate_and(uint8_t value) {
	// Extract individual bits from the 4-bit value
	bool bit1 = (value & (1 << 1)) >> 1; // PA1
	bool bit2 = (value & (1 << 2)) >> 2; // PA2
	bool bit4 = (value & (1 << 4)) >> 4; // PA4
	bool bit5 = (value & (1 << 5)) >> 5; // PA5

	// Perform logical AND operation
	return bit1 && bit2 && bit4 && bit5;
}
bool calculate_and2(uint8_t value) {
    // Extract individual bits from the 4-bit value
    uint8_t bit0 = value & (1 << 0);  // PB0
    uint8_t bit1 = value & (1 << 1);  // PB1
    uint8_t bit3 = value & (1 << 3);  // PB3
    uint8_t bit4 = value & (1 << 4);  // PB4

    // Perform logical AND operation
    return (bit0 && bit1 && bit3 && bit4);
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

			if (count > 10) {
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
int IC_7421() {
	
	uint8_t passed = 1;
	
	
	
	DDRA |= (1 << PA1) | (1 << PA2) | (1 << PA4) | (1 << PA5);

	DDRA &= ~(1 << PA6);

	PORTA |= (1 << PA6);

	
	
	for (uint8_t value = 0; value < 16; value++) {
		PORTA = (PORTA & ~(0b00110110)) | 
		((value & 0x01) << PA1) |
		((value & 0x02) << PA2) | 
		((value & 0x04) << (PA4 - 2)) | 
		((value & 0x08) << (PA5 - 3));  

		// Delay for stability
		_delay_ms(100);

		// Calculate AND of the 4 bits
		bool and_result = calculate_and(value);

		// Read PA6 value
		bool pa6_value = (PINA & (1 << PA6)) >> PA6;

		// Check if PA6 matches the AND result
		if (pa6_value == and_result) {
			// Match found
			// Do something (e.g., toggle an LED, log, etc.)
			} else {
			passed = 0;
			break;
		}
	}
		
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB3) | (1 << PB4);

	DDRB &= ~(1 << PB5);

	PORTB |= (1 << PB5);
	for (uint8_t value = 0; value < 16; value++) {
		PORTB = (PORTB & ~(0b00011011)) |
		((value & 0x01) << PB0) |
		((value & 0x02) << PB1) |
		((value & 0x04) << (PB3 - 2)) |
		((value & 0x08) << (PB4 - 3));

		// Delay for stability
		_delay_ms(100);

		// Calculate AND of the 4 bits
		bool and_result = calculate_and2(value);

		// Read PA6 value
		bool pa6_value = (PINB & (1 << PB5)) >> PB5;

		// Check if PA6 matches the AND result
		if (pa6_value == and_result) {
			// Match found
			// Do something (e.g., toggle an LED, log, etc.)
			} else {
			passed = 0;
			break;
		}
	}
	return passed;
}