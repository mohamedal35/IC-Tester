#ifndef ICS_CONF_H
#define ICS_CONF_H

#include <avr/io.h>

// Pin and Port Definitions
#define F_CPU 8000000UL // 8 MHz CPU clock

// BCD Input Pins
#define BCD_PORT PORTA
#define BCD_DDR DDRA
#define BCD_MASK 0x0F // Mask for PA0 to PA3

// IC Outputs
#define OUTPUT_PORT_A PINA
#define OUTPUT_PORT_B PINB
#define OUTPUT_MASK_A 0xF0 // Mask for PA4 to PA7
#define OUTPUT_MASK_B 0x3F // Mask for PB0 to PB5

// LED Pin
#define LED_PORT PORTC
#define LED_DDR DDRC
#define LED_MASK (1 << PC2) // PC2 for LED

// Function Prototypes
extern void IC_7442(void);

#endif // ICS_CONF_H