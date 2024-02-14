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
    START,
    ACTIVE,
    END
};

volatile States currentState = {States::INIT};

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

void startCounter(int time)
{
    cli();
    TCNT1 = 0; // valeur initiale du compteur

    OCR1A = time; // nb cycles pour 0.1s

    TCCR1A &= 0;                         // normal mode
    TCCR1B |= (1 << CS12) | (1 << CS10); // prescaler -> on utilise le timer 1 donc CS1 puis pour 1024 on a 101
    TCCR1C &= 0;

    TIMSK1 |= (1 << OCIE1A); // quand est-ce qu'on veut lancer l'interruption = aux temps A et B

    sei();
}

ISR(TIMER1_COMPA_vect)
{
    if (currentState == States::START)
    {
        counter++;
        startCounter(TIMER_INTERVAL);
    }
}

ISR(INT0_vect)
{
    if (currentState == States::START || currentState == States::INIT)
    {
        if (isPressed)
            currentState = States::ACTIVE;
        else
        {
            currentState = States::START;
            startCounter(TIMER_INTERVAL);
        }
    }
    isPressed = !isPressed;
    // EIFR |= (1 << INTF0) ;
}

void initialisation(void)
{
    cli(); // désactive les interruptions (pause)
    isPressed = false;
    DDRB |= (1 << DDB0) | (1 << DDB1); // A0 et A1 en sortie
    DDRD &= ~(1 << DDD2);              // D2 en entrée

    // cette procédure ajuste le registre EIMSK
    // de l’ATmega324PA pour permettre les interruptions externes
    EIMSK |= (1 << INT0); // active possibilité d'interuption sur INT0

    // il faut sensibiliser les interruptions externes aux
    // changements de niveau du bouton-poussoir
    // en ajustant le registre EICRA
    EICRA |= (1 << ISC00); // interruption sur les deux fronts

    sei(); // active globablement les interruptions (play)
}

int main()
{

    while (true)
    {
        switch (currentState)
        {
        case States::INIT:
            counter = 0;
            initialisation();
            break;

        case States::START:
            if (counter == MAX_COUNTER)
            {
                currentState = States::ACTIVE;
            }

            break;

        case States::ACTIVE:
            for (unsigned i = 0; i < (WAIT_TIME_INTERVAL / (8 * FLASHING_TIME_INTERVAL / 5)); i++)
            {
                setLedGreen();
                _delay_ms(FLASHING_TIME_INTERVAL / 5);
                setLedOff();
                _delay_ms(FLASHING_TIME_INTERVAL / 5);
            }

            _delay_ms(WAIT_TIME_INTERVAL);
            for (unsigned i = 0; i < (counter / 2); i++)
            {
                setLedRed();
                _delay_ms(FLASHING_TIME_INTERVAL);
                setLedOff();
                _delay_ms(FLASHING_TIME_INTERVAL);
            }
            currentState = States::END;
            break;

        case States::END:
            setLedGreen();
            _delay_ms(WAIT_TIME_INTERVAL / 2);
            setLedOff();
            currentState = States::INIT;
            counter = 0;
            break;
        }
    }
}