#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h> // Dodano za bool tip i true/false konstante

#define PWM_PIN_1 3 // Pin za prvu PWM diodu
#define PWM_PIN_2 5 // Pin za drugu PWM diodu
#define IC_PIN 2   // Pin za input capture

volatile uint16_t intensity = 0; 
volatile bool increasing = true;  // Varijabla za praćenje smjera promjene intenziteta

void PWM_init() {
  // Postavljanje frekvencije Timer-a 1 na frekvenciju mikroprocesora
  TCCR1B |= (1 << CS10);

  // Postavljanje pinova za PWM kao izlaz
  DDRD |= (1 << DDD3) | (1 << DDD5);

  // Postavljanje 10-bitnog phase-correct PWM-a
  TCCR1A |= (1 << WGM10) | (1 << COM1A1) | (1 << COM1B1);
  TCCR1B |= (1 << WGM12);
}

void ic_init() {
  // Definiranje odgovarajućeg pina Timer-a 1 kao PULL-UP ulaz
  DDRD &= ~(1 << DDD2); // Postavljanje pin-a kao ulaz
  PORTD |= (1 << PORTD2); // Uključivanje pull-up otpora

  // Omogućavanje interrupt pri pritisku gumba spojenog na taj pin
  EICRA |= (1 << ISC01); // Konfiguracija za promjenu na INT0
  EIMSK |= (1 << INT0); // Omogućavanje INT0 interrupta
}

void LED_1_intensity(uint16_t intensity) {
  OCR1A = intensity;
}

void LED_2_intensity(uint16_t intensity) {
  OCR1B = intensity;
}

ISR(INT0_vect) {
  // Trenutni intenzitet prve diode postavi kao intenzitet druge diode
  LED_2_intensity(OCR1A);
}

int main() {
  // Inicijalizacija PWM-a i interrupta
  PWM_init();
  ic_init();
  
  sei(); // Omogućavanje globalnih interrupta

  while(1) {
    // Postavljanje intenziteta prve diode
    LED_1_intensity(intensity);

  // Provjera smjera promjene intenziteta i prilagođavanje
  if (increasing) {
      if (intensity < 1023) {
        intensity++;
      } else {
        increasing = false;
      }
    } else {
      if (intensity > 0) {
        intensity--;
      } else {
        increasing = true;
      }
    }

    // Pauza od 3 milisekunde
    _delay_ms(3);
  }

  return 0;
}
