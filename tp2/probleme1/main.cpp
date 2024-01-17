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
            _delay_ms(GREEN_LED_ON_DELAY); // 2s
            setLedOff();

            currentState = States::INIT;

            break;
        }
    }
}