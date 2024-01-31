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

Table des états :


+------------+---------+--------------+-------+
|    Etat    | Boutton | Etat suivant |  LED  |
+------------+---------+--------------+-------+
| INIT       |       0 | INIT         | rouge |
| INIT       |       1 | PRESSED_1    | ambre |
| PRESSED_1  |       0 | RELEASED_1   | vert  |
| PRESSED_1  |       1 | PRESSED_1    | ambre |
| RELEASED_1 |       0 | RELEASED_1   | vert  |
| RELEASED_1 |       1 | PRESSED_2    | rouge |
| PRESSED_2  |       0 | RELEASED_2   | off   |
| PRESSED_2  |       1 | PRESSED_2    | rouge |
| RELEASED_2 |       0 | RELEASED_2   | off   |
| RELEASED_2 |       1 | PRESSED_3    | vert  |
| PRESSED_3  |       0 | INIT         | rouge |
| PRESSED_3  |       1 | PRESSED_3    | vert  |
|            |         |              |       |
|            |         |              |       |
|            |         |              |       |
+------------+---------+--------------+-------+

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

enum class States
{
    INIT,
    PRESSED_1,
    RELEASED_1,
    PRESSED_2,
    RELEASED_2,
    PRESSED_3,
};

volatile States currentState = {States::INIT};

ISR(INT0_vect) {
    _delay_ms(30);

    buttonPressed = !buttonPressed;

    EIFR |= (1 << INTF0) ;
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
        switch (currentState)
        {

        case States::INIT:
            if (buttonPressed)
            {
                setLedAmber();
                currentState = States::PRESSED_1;
            }
            else
            {
                setLedRed();
            }
            break;

        case States::PRESSED_1:
            if (!buttonPressed)
            {
                setLedGreen();
                currentState = States::RELEASED_1;
            }
            else
            {
                setLedAmber();
            }
            break;

        case States::RELEASED_1:
            if (buttonPressed)
            {
                setLedRed();
                currentState = States::PRESSED_2;
            }
            else
            {
                setLedGreen();
            }
            break;

        case States::PRESSED_2:
            if (!buttonPressed)
            {
                setLedOff();
                currentState = States::RELEASED_2;
            }
            else
            {
                setLedRed();
            }
            
            break;

        case States::RELEASED_2:
            if (buttonPressed)
            {
                setLedGreen();
                currentState = States::PRESSED_3;
            }
            else
            {
                setLedOff();
            }
            break;

        case States::PRESSED_3:
            if (!buttonPressed)
            {
                setLedRed();
                currentState = States::INIT;
            }
                
            else
            {
                setLedGreen();
            }
            break;
        }
    }
}