#include <avr/io.h>
#include <util/delay.h>

#include <stdint.h>

#define SHIFT_CLOCK   PB2
#define STORE_CLOCK   PB1
#define DATA_PIN      PB0

uint8_t digits[10] = {
    0b11111100,
    0b01100000,
    0b11011010,
    0b11110010,
    0b01100110,
    0b10110110,
    0b10111110,
    0b11100000,
    0b11111110,
    0b11110110
};

void setupShiftRegister() {
    // Set pins as output
    DDRB |= (1 << DATA_PIN) | (1 << STORE_CLOCK) | (1 << SHIFT_CLOCK);

    // Reset the register
    sendByte(0);
}

void shiftClockPulse() {
    PORTB |= (1 << SHIFT_CLOCK);
    PORTB &= ~(1 << SHIFT_CLOCK);
}

void storeClockPulse() {
    PORTB |= (1 << STORE_CLOCK);
    PORTB &= ~(1 << STORE_CLOCK);
}

void sendByte(uint8_t byte) {
    for (uint8_t i = 0; i < 8; i++) {
        if (byte & 1) {
            PORTB |= (1 << DATA_PIN);
        } else {
            PORTB &= ~(1 << DATA_PIN);
        }
        shiftClockPulse();
        byte >>= 1;
    }
    storeClockPulse();
}

void displayDigit(uint8_t digit) {
    sendByte(digits[digit]);
}

int main() {
    setupShiftRegister();

    while(1) {
        for(uint8_t i = 0; i <= 9; i++) {
            displayDigit(i);
            _delay_ms(1000);
        }
    }

    return 0;
}
