#include <avr/io.h>
#include <util/delay.h>

int main(int argc, char const *argv[])
{   
    // Postavljanje pin-a D4 kao ulaz
    DDRD &= ~(1 << DDD4);

    // Postavljanje ugrađene LED diode kao izlaz
    DDRB |= (1 << DDB5);

    while (1) {
        if( !(PIND & (1 << PIND4)) ) {
            // Ako je D4 pritisnut, isključi LED
            PORTB &= ~(1 << PB5);
        }
        else {
            // Inače, upali LED
            PORTB |= (1 << PB5);
        }

        // Mali kašnjenje
        _delay_ms(10);
    }

    return 0;
}
