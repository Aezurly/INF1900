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

volatile uint8_t counter = 0;
const uint16_t TIMER_INTERVAL = 781; // 0.1s pour le problème 1
const uint8_t MAX_COUNTER = 120;
const uint8_t FLASHING_TIME_INTERVAL = 250; // en ms
const uint16_t WAIT_TIME_INTERVAL = 2000;   // em ms
volatile bool isPressed = false;

enum class States
{
    INIT,
    GREEN,
    ORANGE,
    RED
};

volatile States currentState = {States::INIT};

void ajustementPwm(int frequence)
{
    cli();
    TCNT1 = 0; // valeur initiale du compteur

    // mise à un des sorties OC1A et OC1B sur comparaison
    // réussie en mode PWM 8 bits, phase correcte
    // et valeur de TOP fixe à 0xFF (mode #1 de la table 16-5
    // page 130 de la description technique du ATmega324PA)

    OCR1A = frequence;
    //OCR1B = 256 - frequence;
    // TOP = 0xFF;

    // division d'horloge par 8 - implique une fréquence de PWM fixe

    TCCR1A |= (1 << WGM10) | (1 << COM1A1) | (1 << COM1B1); // PWM phase correct + clear phase
    TCCR1B |= (1 << CS12) | (1 << CS10);
    TCCR1C = 0;

    sei();
}

void setLedOff()
{
    ajustementPwm(256);
}

void setLedGreen()
{
    //TCCR1A |= (1 << COM1A0);
    ajustementPwm(0);
};

void setLedRed()
{
    //TCCR1A &= ~(1 << COM1A0);
    ajustementPwm(0);
};

void setLedAmber()
{
    ajustementPwm(128);
}

void initialisation(void)
{
    DDRA &= ~(1 << DDA0) | ~(1 << DDA1); // A0 et A1 en entrée
    // DDRB |= (1 << DDB0) | (1 << DDB1); // B0 et B1 en sortie
    DDRD |= (1 << DDD5) | (1 << DDD6) | (1 << DDD4) | (1 << DDD7); // D4, D5, D6, D7 en sortie
}

int main()
{
    initialisation();

    while (true)
    {
        setLedAmber();
        _delay_ms(1000);
        setLedOff();
        _delay_ms(500);
        setLedGreen();
        _delay_ms(1000);
        setLedOff();
        _delay_ms(500);
        setLedRed();
        _delay_ms(1000);
        setLedOff();
        _delay_ms(500);
    }
}