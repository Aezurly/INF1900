#include <avr/io.h>
#define F_CPU 800000
#include <util/delay.h>

bool pressed() {
    if((0b00000100 & PIND) == 0b00000100) {
        _delay_ms(15);
        return ((0b00000100 & PIND) == 0b00000100);
    }
    return false; 
}

void couleur(int n) {
    PORTA = n;
}

int main() {
    DDRA = 0b00000011;
    DDRD = 0;
    int i = 1;
    while(1) {
        if(i > 3) {
            PORTA = 0;
            i = 1;
        }
        couleur(0);
        if(pressed()) {
            if(i > 2) {
                while(pressed()) {
                    couleur(1);
                    _delay_ms(20);
                    couleur(2);
                    _delay_ms(30);
                }
                i++;
            }
            else {
                while(pressed()) {
                    couleur(i);
                    _delay_ms(50);
                }
                i++;
            }
        }
    }
}