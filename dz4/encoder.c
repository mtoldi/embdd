#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t A = 0;
volatile uint8_t A_prev = 0;
volatile uint8_t B = 0;
volatile uint8_t B_prev = 0;
volatile int32_t ticks = 0;

void encoder_init() {
    // Set INT0 (PD2) and INT1 (PD3) as input with pull-up resistors
    DDRD &= ~(1 << DDD2) & ~(1 << DDD3);
    PORTD |= (1 << PORTD2) | (1 << PORTD3);

    // Configure external interrupts for any logical change
    EICRA |= (1 << ISC00) | (1 << ISC10);

    // Enable external interrupts on INT0 and INT1
    EIMSK |= (1 << INT0) | (1 << INT1);

    // Enable global interrupts
    sei();
}

// ISR for INT0 (PD2)
ISR(INT0_vect) {
    A_prev = A;
    B_prev = B;
    A = (PIND & (1 << PIND2)) ? 1 : 0;
    B = (PIND & (1 << PIND3)) ? 1 : 0;

    // Increment or decrement ticks based on state transitions
    if (A_prev == 0 && B_prev == 0 && A == 1 && B == 0) {
        ++ticks;
    } else if (A_prev == 1 && B_prev == 0 && A == 1 && B == 1) {
        ++ticks;
    } else if (A_prev == 1 && B_prev == 1 && A == 0 && B == 1) {
        ++ticks;
    } else if (A_prev == 0 && B_prev == 1 && A == 0 && B == 0) {
        ++ticks;
    } else if (A_prev == 0 && B_prev == 0 && A == 0 && B == 1) {
        --ticks;
    } else if (A_prev == 0 && B_prev == 1 && A == 1 && B == 1) {
        --ticks;
    } else if (A_prev == 1 && B_prev == 1 && A == 1 && B == 0) {
        --ticks;
    } else if (A_prev == 1 && B_prev == 0 && A == 0 && B == 0) {
        --ticks;
    }
}

// ISR for INT1 (PD3)
ISR(INT1_vect) {
    A_prev = A;
    B_prev = B;
    A = (PIND & (1 << PIND2)) ? 1 : 0;
    B = (PIND & (1 << PIND3)) ? 1 : 0;

    // Increment or decrement ticks based on state transitions
    if (A_prev == 0 && B_prev == 0 && A == 1 && B == 0) {
        ++ticks;
    } else if (A_prev == 1 && B_prev == 0 && A == 1 && B == 1) {
        ++ticks;
    } else if (A_prev == 1 && B_prev == 1 && A == 0 && B == 1) {
        ++ticks;
    } else if (A_prev == 0 && B_prev == 1 && A == 0 && B == 0) {
        ++ticks;
    } else if (A_prev == 0 && B_prev == 0 && A == 0 && B == 1) {
        --ticks;
    } else if (A_prev == 0 && B_prev == 1 && A == 1 && B == 1) {
        --ticks;
    } else if (A_prev == 1 && B_prev == 1 && A == 1 && B == 0) {
        --ticks;
    } else if (A_prev == 1 && B_prev == 0 && A == 0 && B == 0) {
        --ticks;
    }
}
