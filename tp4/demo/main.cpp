#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>

volatile bool buttonPressed = false;
volatile uint8_t gMinuterieExpiree; 

volatile bool firstLedCounterDone, secondLedCounterDone = false;

void setLedOff()
{
    PORTA &= ~(1 << PORTA1);
    PORTA &= ~(1 << PORTA0);
}

void setLedGreen()
{
    PORTA &= ~(1 << PORTA0);
    PORTA |= (1 << PORTA1);
};

void setLedRed()
{
    PORTA &= ~(1 << PORTA1);
    PORTA |= (1 << PORTA0);
};

ISR(TIMER1_COMPA_vect) {firstLedCounterDone = true;}
ISR(TIMER1_COMPB_vect) {secondLedCounterDone = true;}

void partirMinuterie () {
    cli();
    TCNT1 = 0 ; // valeur initiale du compteur

    OCR1A = 7812; // nb cycles pour 3s
    OCR1B = 15625; // nb cycles pour 6s

    TCCR1A |= 0 ; //normal mode
    TCCR1B |= (1 << CS12) | (1 << CS10); // prescaler -> on utilise le timer 1 donc CS1 puis pour 1024 on a 101
    TCCR1C |= 0;

    TIMSK1 |= (1 << OCIE1A) | (1 << OCIE1B) ; // quand est-ce qu'on veut lancer l'interruption = aux temps A et B

    sei();
}

void initialisation() {

    DDRA |= (1<<DDA0) | (1<<DDA1);
    
    partirMinuterie();
}

int main() {

    initialisation();

    while(1) {
        if(firstLedCounterDone)
            setLedGreen();
        if(secondLedCounterDone)
            setLedRed();
    }
}