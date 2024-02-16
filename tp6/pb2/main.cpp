/**
Auteurs : Luca Bedel et Alice Vergeau
Matricules : 2297114 et 2211118
Date : 15/02/2024

TP2
Section #6
Équipe 142

# Description du programme :

Programme qui permet de changer la couleur (rouge,orange,vert) de la DEL de la
carte mère en fonction de la valeur enrengistrée par le CAN (Convertisseur
Analogue - Numérique).
La DEL change de couleur, vert quand la valeur lue est basse, orange lorsqu'elle
est moyenne et rouge lorsqu'elle est haute.
Dans cette exercice en particulier, on utilise une photorésistance pour capter
la lumière. Ainsi, la DEL est verte lorsque la lumière ambiante est très faible,
orange lorsqu'elle est moyenne et rouge lorsqu'elle est forte.


# Identification matérielle :

B0 et B1 sont branchés à la DEL libre, et sont donc en sorties.
A0 et A1 sont branchés par le fil de données à notre circuit sur le breadboard,
et sont donc en entrée pour pouvoir être lue.

Le breadboard est composé de :
- un petit condensateur de 0.1 µF (Digi-Key: BC1621-ND)
- une résistance de 10K (brun-noir-orange) (Digi-Key: S10KQTR-ND)
- une photorésistance (Digi-Key: PDV-P8101-ND)
Deux fils partent du breadboard, un vers un port Vcc et GND, et l'autre est le
fil de données qui est relié à un port analogique (dans notre cas A0 et A1).


# Table des états :

+------+--------------------+--------------+--------+
| Etat | A0                 | Etat Suivant | LED    |
+------+--------------------+--------------+--------+
| x    | < ORANGE_THRESHOLD | GREEN        | GREEN  |
| x    | < RED_THRESHOLD    | ORANGE       | ORANGE |
| x    | > RED_THRESHOLD    | RED          | RED    |
+------+--------------------+--------------+--------+
(soit x = n'importe quel état)

**/

#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "can.h"

const uint8_t BLINK_ORANGE_DELAY = 10;

// Position où l'on va lire analogiquement les données à convertir de manière
// digitale, dans notre cas on lit A0, donc position 0
const uint8_t READING_POSITION = 0;

// décidée arbitrairement. Au dessus, la lumière est orange, en dessous verte.
const uint8_t ORANGE_THRESHOLD = 0xD8;
// décidée arbitrairement. Au dessus, la lumière est red, en dessous orange.
const uint8_t RED_THRESHOLD = 0xF0;

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
    DDRB |= (1 << DDB0) | (1 << DDB1);   // B0 et B1 en sortie
}

void checkState()
{
    // Lecture des données analogues et conversion en données numériques
    can analogValue = can();
    uint8_t digitalValue = analogValue.lecture(READING_POSITION) >> 2;

    // Changement d'états en fonction de ces données.
    if (digitalValue < ORANGE_THRESHOLD)
    {
        currentState = {States::GREEN};
    }
    else if (digitalValue < RED_THRESHOLD)
    {
        currentState = {States::ORANGE};
    }
    else
    {
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