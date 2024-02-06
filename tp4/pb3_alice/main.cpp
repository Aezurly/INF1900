/**
Auteurs : Luca Bedel et Alice Vergeau
Matricules : 2297114 et 2211118
Date : 

TP2
Section #6
Équipe 142

# Description du programme :
Programme qui permet de changer la couleur (rouge,ambre,vert) de la DEL de la carte mère grâce à une machine à état. 
La DEL change de couleur qu'on appuie ou on relache le bouton Interrupt.

# Identification matérielle :
A0 et A1 sont branchés à la DEL libre, et sont donc en sorties.
D2 est utilisé pour contrôler le bouton, et donc est en entrée pour pouvoir 
détecter quand le bouton est pressé

**/

#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>

//volatile uint8_t currentState = 0; // selon le nom de votre variable

const uint8_t MASK_D2 = (1 << PD2);
const uint8_t DEBOUNCE_DELAY = 10;
const uint8_t BLINK_AMBER_DELAY = 10;

volatile bool buttonPressed = false;
volatile uint8_t gMinuterieExpiree; 
volatile bool highPwmSignal = false;

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

void setLedAmber()
{
    setLedGreen(); 
	_delay_ms(BLINK_AMBER_DELAY);
	setLedRed(); 
	_delay_ms(BLINK_AMBER_DELAY);
}

ISR(TIMER1_COMPA_vect) {
    
}

void ajustementPwm (int frequence) {
    cli();
    TCNT1 = 0 ; // valeur initiale du compteur

    // mise à un des sorties OC1A et OC1B sur comparaison
    // réussie en mode PWM 8 bits, phase correcte
    // et valeur de TOP fixe à 0xFF (mode #1 de la table 16-5
    // page 130 de la description technique du ATmega324PA)

    OCR1A = frequence;
    OCR1B = frequence ;
    //TOP = 0xFF;

    // division d'horloge par 8 - implique une fréquence de PWM fixe

    TCCR1A |= (1 << WGM10) | (1 << COM1A1) | (1 << COM1B1) ; // PWM phase correct + clear phase
    TCCR1B |= (1 << CS12) | (1 << CS10);
    TCCR1C = 0;

    sei();
}

int main()
{
    DDRD |= (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7); // A0 et A1 en sortie
    for(int i = 0; i <= 256; i+=64) {
        ajustementPwm (i);
        _delay_ms(2000);
    }
    for(int i = 256; i >= 0; i-=64) {
        ajustementPwm (i);
        _delay_ms(2000);
    }
}


