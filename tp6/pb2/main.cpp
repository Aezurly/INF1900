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
#include "can.h"

const uint8_t BLINK_ORANGE_DELAY = 10;
const uint8_t FLASHING_TIME_INTERVAL = 250; // en ms
const uint16_t WAIT_TIME_INTERVAL = 2000;   // em ms

enum class States
{
    INIT,
    GREEN,
    ORANGE,
    RED
};

States currentState = {States::INIT};

void setLedOff()
{
    PORTB &= ~(1 << PORTB1);
    PORTB &= ~(1 << PORTB0);
}

void setLedGreen()
{
    PORTB &= ~(1 << PORTB0);
    PORTB |= (1 << PORTB1);
};

void setLedRed()
{
    PORTB &= ~(1 << PORTB1);
    PORTB |= (1 << PORTB0);
};

void setLedOrange()
{
    setLedGreen(); 
	_delay_ms(BLINK_ORANGE_DELAY);
	setLedRed(); 
	_delay_ms(BLINK_ORANGE_DELAY);
}

void initialisation(void)
{
    DDRA &= ~(1 << DDA0) | ~(1 << DDA1); // A0 et A1 en entrée
    DDRB |= (1 << DDB0) | (1 << DDB1); // B0 et B1 en sortie

    can();
}

void checkState() {
    can analog = can();
    uint8_t table = analog.lecture(0x00) >> 2;

    if(table < 0xC0) {
        currentState = {States::GREEN};
    }
    else if (table < 0xE0) {
        currentState = {States::ORANGE};
    }
    else {
        currentState = {States::RED};
    }
}

int main()
{
    initialisation();
    while (true)
    {
        checkState();
        switch (currentState)
        {
        case States::INIT:
            setLedOff();
            break;

        case States::GREEN:
            setLedGreen();
            break;

        case States::ORANGE:
            setLedOrange();
            break;

        case States::RED:
            setLedRed();
            break;
        }
    }
}