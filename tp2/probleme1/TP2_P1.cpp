/**
Auteurs : Luca Bedel et Alice Vergeau
Équipe 142 - Section 6

TP2 - PB1

# Description du programme :
Programme qui paramètre la DEL libre de la carte mère pour rester éteinte au 
début, puis s'allumer pendant 2 secondes après qu'on ait pressé et relaché le 
bouton trois fois.

# Identification matérielle :
A0(+) et A1(-) sont branchés à la DEL libre, et sont donc en sorties.
D2 est utilisé pour contrôler le bouton, et donc est en entrée pour pouvoir 
détecter quand le bouton est pressé

# Table des états du TP2 - PB1

+------------+--------+--------------+------+
| Etat       | Bouton | Etat suivant | LED  |
+------------+--------+--------------+------+
| INIT       | 0      | INIT         | 0    |
| INIT       | 1      | PRESSED_1    | 0    |
| PRESSED_1  | 0      | RELEASED_1   | 0    |
| PRESSED_1  | 1      | PRESSED_1    | 0    |
| RELEASED_1 | 0      | RELEASED_1   | 0    |
| RELEASED_1 | 1      | PRESSED_2    | 0    |
| PRESSED_2  | 0      | RELEASED_2   | 0    |
| PRESSED_2  | 1      | PRESSED_2    | 0    |
| RELEASED_2 | 0      | RELEASED_2   | 0    |
| RELEASED_2 | 1      | PRESSED_3    | 0    |
| PRESSED_3  | 0      | RELEASED_3   | 0    |
| PRESSED_3  | 1      | PRESSED_3    | 0    |
| RELEASED_3 | X      | INIT         | vert |
+------------+--------+--------------+------+

**/

#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
#include <stdint.h>

const uint8_t MASK_D2 = (1 << PD2);
const uint8_t DEBOUNCE_DELAY = 10;

const uint16_t GREEN_LED_ON_DELAY = 2000;


enum class States
{
    INIT,
    PRESSED_1,
    RELEASED_1,
    PRESSED_2,
    RELEASED_2,
    PRESSED_3,
    RELEASED_3
};

States currentState = {States::INIT};


// Vérifie si le bouton sur D2 est pressé, en prenant en compte le rebond.
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

int main()
{
    // Établit les différentes entrées et sorties des broches
    DDRA |= (1 << DDA0) | (1 << DDA1);
    DDRD &= ~(1 << DDD2);
    setLedOff();

    while (true)
    {
        switch (currentState)
        {

        case States::INIT:
            if (isD2Pressed())
                currentState = States::PRESSED_1;
            break;

        case States::PRESSED_1:
            if (!isD2Pressed())
                currentState = States::RELEASED_1;
            break;

        case States::RELEASED_1:
            if (isD2Pressed())
                currentState = States::PRESSED_2;
            break;

        case States::PRESSED_2:
            if (!isD2Pressed())
                currentState = States::RELEASED_2;
            break;

        case States::RELEASED_2:
            if (isD2Pressed())
                currentState = States::PRESSED_3;
            break;

        case States::PRESSED_3:
            if (!isD2Pressed())
                currentState = States::RELEASED_3;
            break;

        case States::RELEASED_3:

            setLedGreen();
            _delay_ms(GREEN_LED_ON_DELAY); 
            setLedOff();

            currentState = States::INIT;

            break;
        }
    }
}