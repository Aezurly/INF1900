/**
Auteurs : Luca Bedel et Alice Vergeau
Matricules : 2297114 et 2211118
Date : 13/02/2024

TP2
Section #6
Équipe 142

# Description du programme :
Programme où, lorsque le bouton est enfoncé, un compteur qui incrémente 10 fois
par seconde est activé. Quand le bouton est relâché ou lorsque le compteur
atteint 120, la lumière clignote vert pendant 1/2 seconde. Ensuite, la carte
mère ne fait rien. Puis, deux secondes plus tard, la lumière rouge s'allume.
Elle devra clignoter (compteur / 2) fois au rythme de 2 fois par seconde.
Ensuite, la lumière tourne au vert pendant une seconde. Finalement, elle
s'éteint et le robot revient à son état original.

En bref, le programme clignote en rouge 5 fois le nombre de seconde que le
bouton a été pressé, et ne dépasse jamais 12 secondes de bouton pressé.

# Identification matérielle :

A0 et A1 sont branchés à la DEL libre, et sont donc en sortie.
D2 est utilisé pour contrôler le bouton, et donc est en entrée pour pouvoir
détecter quand le bouton est pressé. On a retiré le cavalier sur IntEN pour
utiliser notre propre bouton sur le breadboard.

Le breadboard est composé de :
- un bouton-poussoir blanc (Digi-Key: EG1328-ND)
- un petit condensateur (gris ou bleu) de 0.1 µF (Digi-Key: BC1621-ND)
- une résistance de 100K (brun-noir-jaune) (Digi-Key: S100KQTR-ND)

**/

#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>

const uint16_t TIMER_INTERVAL = 781; // nombre de cycles pour 0.1 secondes
const uint8_t MAX_COUNTER = 120;
const uint8_t RED_FLASHING_INTERVAL = 250;  // en ms
const uint16_t WAIT_TIME_INTERVAL = 2000;   // em ms
const uint8_t GREEN_FLASHING_INTERVAL = 50; // en ms
const uint8_t GREEN_FLASHING_TIME = 500;    // en ms
const uint8_t GREEN_LIGHT_DELAY = 1000;     // en ms

volatile bool isPressed = false;
volatile uint8_t counter = 0;

enum class States
{
    INIT,
    COUNTING,
    DISPLAY,
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

    OCR1A = time; // nombre cycles pour 0.1s

    TCCR1A &= 0;                         // normal mode
    TCCR1B |= (1 << CS12) | (1 << CS10); // prescaler de 1024
    TCCR1C &= 0;

    TIMSK1 |= (1 << OCIE1A); // lancer l'interruption au temps OCR1A

    sei();
}

ISR(TIMER1_COMPA_vect)
{
    if (currentState == States::COUNTING)
    {
        counter++;
        startCounter(TIMER_INTERVAL);
    }
}

ISR(INT0_vect)
{
    // Le bouton pressé ne change l'état que lorsqu'on est en COUNTING ou INIT
    if (currentState == States::COUNTING || currentState == States::INIT)
    {
        if (isPressed)
            currentState = States::DISPLAY;
        else
        {
            currentState = States::COUNTING;
            startCounter(TIMER_INTERVAL);
        }
    }
    isPressed = !isPressed;
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

        case States::COUNTING:
            if (counter == MAX_COUNTER)
            {
                currentState = States::DISPLAY;
            }

            break;

        case States::DISPLAY:

            // Lumière clignote en vert pendant GREEN_FLASHING_TIME ms
            for (unsigned i = 0; i < GREEN_FLASHING_TIME / (2 * GREEN_FLASHING_INTERVAL); i++)
            {
                setLedGreen();
                _delay_ms(GREEN_FLASHING_INTERVAL);
                setLedOff();
                _delay_ms(GREEN_FLASHING_INTERVAL);
            }

            // Lumière reste éteinte pendant WAIT_TIME_INTERVAL ms
            _delay_ms(WAIT_TIME_INTERVAL);

            // Lumière clignote en rouge counter/2 fois
            for (unsigned i = 0; i < (counter / 2); i++)
            {
                setLedRed();
                _delay_ms(RED_FLASHING_INTERVAL);
                setLedOff();
                _delay_ms(RED_FLASHING_INTERVAL);
            }

            currentState = States::END;
            break;

        case States::END:

            // Lumière reste verte pendant GREEN_LIGHT_DELAY ms
            setLedGreen();
            _delay_ms(GREEN_LIGHT_DELAY);
            setLedOff();

            currentState = States::INIT;
            counter = 0;
            break;
        }
    }
}