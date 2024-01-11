#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>

bool pressed() {
    return ((PIND & 0b00000100) == 0b00000100); 
}

int main() {
    DDRA = 0b00000011;
    DDRD = 0x00;
    while(pressed()) {
        PORTA = 0b00000010;
        _delay_ms(2000);
        PORTA = 0b00000001;
        _delay_ms(2000);
    }
}