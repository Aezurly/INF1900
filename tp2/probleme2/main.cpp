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

const uint8_t MASK_D2 = (1 << PD2);
const uint8_t DEBOUNCE_DELAY = 10;
const uint8_t BLINK_AMBER_DELAY = 10;

enum class States
{
    INIT,
    PRESSED_1,
    RELEASED_1,
    PRESSED_2,
    RELEASED_2,
    PRESSED_3,
};

States currentState = {States::INIT};

bool isD2Pressed()
{
    if (MASK_D2 & PIND)
    {
        _delay_ms(DEBOUNCE_DELAY);
        return (MASK_D2 & PIND);
    }
    return false;
};

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
    DDRA |= (1 << DDA0) | (1 << DDA1);
    DDRD &= ~(1 << DDD2);
    //setLedRed();

    while (true)
    {
        switch (currentState)
        {

        case States::INIT:
            if (isD2Pressed())
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
            if (!isD2Pressed())
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
            if (isD2Pressed())
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
            if (!isD2Pressed())
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
            if (isD2Pressed())
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
            if (!isD2Pressed())
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