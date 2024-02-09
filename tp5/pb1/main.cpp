/**
Auteurs : Luca Bedel et Alice Vergeau
Matricules : 2297114 et 2211118
Date : 

TP2
Section #6
Équipe 142
**/

#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "memoire_24.h"
#include <string.h>


const uint8_t MASK_D2 = (1 << PD2);
const uint8_t DEBOUNCE_DELAY = 10;
const uint8_t BLINK_AMBER_DELAY = 10;

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

int main()
{
    DDRA |= (1 << DDA0) | (1 << DDA1); // A0 et A1 en sortie
    
    Memoire24CXXX memoire = Memoire24CXXX();
    const char* text = "*P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*";
    char resultat[46];
    memoire.ecriture(0x0000,(uint8_t*)text, strlen(text));
    
    // for(int i = 0; i < 64; i++) {
    //     memoire.ecriture(i, int(text[i]));
    // }
    _delay_ms(5);
    // uint8_t* temp;
    // auto lect = new char[64];
    // for(int i = 0; i < 64; i++) {
    //     temp = 0;
    //     memoire.lecture(i, temp);
    //     lect[i] = char(*temp);
    // }

    memoire.lecture(0x0000,(uint8_t*)resultat, strlen(text));

    resultat[7] = 'c';
    resultat[1] = 'c';
    resultat[44] = 'k';
    
    _delay_ms(5);
    for (uint8_t i=0 ;i < 45;i++)
  {
    if(text[i] == resultat[i])
      setLedGreen();
    else
      setLedRed();
    
    _delay_ms(300);
    setLedOff();
    _delay_ms(50);
  }
}