#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "serial.h" // Assuming you have implemented UART functions

// Define desired states of the motor (in pulses relative to initial state)
int32_t r[] = {500, 0, -500, -1000, 0};
uint8_t state_index = 0;

// PID controller parameters
int32_t k_p = 10;
int32_t k_i = 1;
int32_t k_d = 5;

int main() {
    // Initialize modules
    sei();
    uart_init();
    encoder_init();
    motor_init();
    counter_init();

    // Initialize variables
    int32_t ticks_prev = 0;
    int32_t err = 0;
    int32_t err_prev = 0;
    int32_t err_acc = 0;
    int32_t err_diff = 0;
    int32_t pid = 0;

    while (1) {
        // Calculate error between desired angle (r[state_index]) and current angle (ticks)
        err = r[state_index] - ticks;

        // PID control
        pid = k_p * err + k_i * err_acc + k_d * err_diff;

        // Update motor control signal based on PID output
        OCR1A = pid;

        // Update error variables for next iteration
        err_acc += err;
        err_diff = err - err_prev;
        err_prev = err;

        // Check if 2 seconds have passed to switch to the next desired state
        if (ms % 2000 == 0) {
            state_index = (state_index + 1) % (sizeof(r) / sizeof(r[0]));
        }

        // Send data to Serial Plotter
        uart_putint(ticks);  // Current angle
        uart_putchar(',');
        uart_putint(r[state_index]);  // Desired angle
        uart_putchar('\n');

        // Delay for stable plotting rate
        _delay_ms(100);
    }

    return 0;
}
