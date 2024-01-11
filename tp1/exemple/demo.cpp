#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>

int main() {
    DDRA = 0b00000011;
    while(1) {
        PORTA = 0b00000010;
        _delay_ms(2000);
        PORTA = 0b00000001;
        _delay_ms(2000);
    }
}