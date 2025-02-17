#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t buttonPressed = 0; // Varijabla za praćenje stanja tipkala

ISR(INT0_vect) {
    buttonPressed = 1; // Postavljanje zastavice kada je tipkalo pritisnuto
}

ISR(INT1_vect) {
    if (!(PIND & (1 << PD2))) { // Provjera je li tipka otpuštena
        buttonPressed = 2; // Postavljanje zastavice kada je tipka otpuštena
    }
}

void initInterrupts() {
    EICRA |= (1 << ISC01); // Postavljanje INT0 na okidač na padajuću ivicu
    EICRA |= (1 << ISC11); // Postavljanje INT1 na okidač na porastajuću ivicu
    EIMSK |= (1 << INT0) | (1 << INT1); // Omogućavanje prekida INT0 i INT1
    sei(); // Omogućavanje globalnih prekida
}

void blinkLED(int times) {
    for (int i = 0; i < times; i++) {
        PORTB |= (1 << PB5); // Uključi LED
        _delay_ms(50);
        PORTB &= ~(1 << PB5); // Isključi LED
        _delay_ms(50);
    }
}

int main() {
    DDRB |= (1 << DDB5); // Postavljanje pin-a kao izlaz za LED
    PORTD |= (1 << PD2); // Aktiviranje internog pull-down otpornika na pinu D2

    initInterrupts(); // Inicijalizacija prekida

    while (1) {
        if (buttonPressed == 1) {
            blinkLED(1); // Blinkaj 1 put ako je tipkalo pritisnuto
            buttonPressed = 0; // Resetiraj zastavicu
        }
        else if (buttonPressed == 2) {
            blinkLED(2); // Blinkaj 2 puta ako je tipka otpuštena
            buttonPressed = 0; // Resetiraj zastavicu
        }
    }

    return 0;
}
