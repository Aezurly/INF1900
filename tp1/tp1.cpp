#include <avr/io.h>
#define F_CPU 800000
#include <util/delay.h>

bool pressed() {
    return ((0b00000100 & PIND) == 0b00000100); 
}

void couleur(int n) {
    PORTA = n;
}

int main() {
    DDRA = 0b00000011;
    DDRD = 0;
    while(1) {
        PORTA = 0;
        int i = 1;
        while(i <= 3) {
            couleur(0);
            if(pressed()) {
                if(i > 2) {
                    while(pressed()) {
                        for(int i = 0; i < 40; i++) {
                            couleur(1);
                            _delay_ms(20);
                            couleur(2);
                            _delay_ms(50);
                        } 
                    }
                    i++;
                }
                else {
                    while(pressed()) {
                        couleur(i);
                        _delay_ms(2000);
                    }
                    i++;
                }
            }
        }
    }
}