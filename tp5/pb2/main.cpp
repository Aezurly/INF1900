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

void initialisationUART(void)
{

  /* VOIR CHAPITRE 19 */

  // 2400 bauds. Nous vous donnons la valeur des deux
  // premiers registres pour vous éviter des complications.
  UBRR0H = 0;
  UBRR0L = 0xCF;

  // permettre la réception et la transmission par le UART0
  // UCSR0A = (1<<RXEN0)|(1<<TXEN0) ;
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);

  // Format des trames: 8 bits, 1 stop bits, sans parité
  UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);

  /* Set frame format: 8data, 2stop bit */
  // UCSRnC = (1<<USBSn)|(3<<UCSZn0);
}

// DE USART VERS PC
void transmissionUART(uint8_t donnee)
{
  /* Wait for empty transmit buffer */
  while (!(UCSR0A & (1 << UDRE0)))
    ;
  /* Put data into buffer, sends the data */
  UDR0 = donnee;
}

int main()
{
  initialisationUART();
  DDRA |= (1 << DDA0) | (1 << DDA1); // A0 et A1 en sortie

  char mots[21] = "Le robot en INF1900\n";
  uint8_t i, j;

  for (i = 0; i < 100; i++)
  {
    for (j = 0; j < 20; j++)
    {
      transmissionUART(mots[j]);
    }
  }
}