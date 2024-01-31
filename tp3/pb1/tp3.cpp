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

const uint16_t PERIOD = 500;

unsigned abs(int x)
{
    if (x < 0)
        return -x;
    else
        return x;
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

void waitDelay(uint16_t delay)
{
    for (uint16_t i = 0; i < delay; i++)
    {
        _delay_us(1);
    }
}

int main()
{
    // Établit les différentes entrées et sorties des broches
    DDRA |= (1 << DDA0) | (1 << DDA1);
    DDRD &= ~(1 << DDD2);
    setLedOff();
    int LED_DELAY;
    bool SENS = 1;
    while (true)
    {
        SENS = -SENS + 1;
        for (uint16_t i = 0; i < PERIOD; i=i+10)
        {
            for (uint8_t j = 0; j < 5; j++)
            {
                LED_DELAY = PERIOD - i;
                setLedGreen();

                waitDelay(abs(LED_DELAY - SENS * PERIOD));

                setLedOff();

                waitDelay(abs(LED_DELAY - (-SENS + 1) * PERIOD));
            }
        }
    }
}