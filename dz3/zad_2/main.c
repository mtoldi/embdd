#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BUTTON_PIN PD2 // Pin za spoj tipke 
#define LED_PIN PB5    // Pin za ugrađenu LED diodu 
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

volatile uint8_t buttonPressCount = 0; // Brojač pritisaka tipke
volatile uint8_t timer0_counter = 0;

void button_init() {
  // Konfiguracija pin-a za tipkalo kao pull-up ulaz
  DDRD &= ~(1 << BUTTON_PIN); 
  PORTD |= (1 << BUTTON_PIN);

  // Omogućavanje prekida pri promjeni stanja pin-a
  PCICR |= (1 << PCIE2);      
  PCMSK2 |= (1 << PCINT18);   
}

void uart_init(unsigned int ubrr) {
  // Postavljanje brzine prijenosa
  UBRR0H = (unsigned char)(ubrr >> 8);
  UBRR0L = (unsigned char)ubrr;
  // Omogućavanje primanja i slanja
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  // Postavljanje formata okvira: 8 bita podataka, 1 bit stopa
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_transmit(unsigned char data) {
  // Čekanje na završetak prijenosa
  while (!(UCSR0A & (1 << UDRE0)));
  // Slanje podataka
  UDR0 = data;
}

void timer0_init() {
  // Postavljanje Timer-a 0 na normalni način rada
  TCCR0A = 0x00;
  TCCR0B = 0x05; // Postavljanje preskalera na 1024
  TIMSK0 = (1 << TOIE0); // Omogućavanje prekida prekoračenja
}

ISR(PCINT2_vect) {
  if (bit_is_clear(PIND, BUTTON_PIN)) { 
    buttonPressCount++; 
    if (buttonPressCount == 4) { 
      PORTB ^= (1 << LED_PIN);  
      buttonPressCount = 0;      
    }
  }
}

ISR(TIMER0_OVF_vect) {
  timer0_counter++;
  if (timer0_counter == 156) { // 156 odašiljanja prekida prekoračenja Timer-a 0 za 1 sekundu (1024 preskaler)
    uart_transmit(timer0_counter);
    timer0_counter = 0;
  }
}

int main() {
  // Postavljanje pinova za LED diodu kao izlaz
  DDRB |= (1 << LED_PIN);

  button_init(); 
  uart_init(MYUBRR);
  timer0_init(); 

  // Omogućavanje globalnih prekida
  sei();

  while (1) {
    
  }

  return 0;
}
