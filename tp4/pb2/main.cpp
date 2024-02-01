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

ISR(INT0_vect) {
    buttonPressed = !buttonPressed;
    _delay_ms(30);

    EIFR |= (1 << INTF0) ;
}

ISR('nidufer') {
    gMinuterieExpiree = 1;
}

void initialisation ( void ) {
    cli (); // désactive les interruptions (pause)

    DDRA |= (1 << DDA0) | (1 << DDA1); // A0 et A1 en sortie
    DDRD &= ~(1 << DDD2); // D2 en entrée

    // cette procédure ajuste le registre EIMSK
    // de l’ATmega324PA pour permettre les interruptions externes
    EIMSK |= (1 << INT0) ; // active possibilité d'interuption sur INT0

    // il faut sensibiliser les interruptions externes aux
    // changements de niveau du bouton-poussoir
    // en ajustant le registre EICRA
    EICRA |= (1 << ISC00) ; // interruption sur les deux fronts

    sei (); // active globablement les interruptions (play)
}


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

int main()
{
    initialisation();

    while (true)
    {
        
    }
}