/**
 * Name:     Smarthome: Touch switch
 * Autor:    Alberto Gil Tesa
 * Web:      https://giltesa.com
 * License:  CC BY-NC-SA 3.0
 * Version:  1.0
 * Date:     2018/01/02
 *
 */

#define pBTN        2 //Interruption
#define pZERO       3 //Interruption
#define pLED_BLUE   4
#define pRELAY_1    5 //PWM
#define pRELAY_2    6 //PWM
#define pNRF_CE     7
#define pNRF_CSN    8
#define pLED_RED    9 //PWM
#define pLED_GREEN 10 //PWM
#define pFLASH_SS  A0
#define pDS18B20   A1
#define pATSHA204A A3

unsigned long time;
byte pinLED;


void setup()
{
    pinMode(pBTN,       INPUT);
    pinMode(pLED_RED,   OUTPUT);
    pinMode(pLED_GREEN, OUTPUT);
    pinMode(pLED_BLUE,  OUTPUT);

    //Start:
    led(pLED_GREEN, 100, 100);
    led(pLED_GREEN, 100, 100);
    led(pLED_GREEN, 100, 100);
}

void loop()
{
    if( digitalRead(pBTN) )
    {
        time = millis();

        if( time % 2 == 0 )
            pinLED = pLED_RED;
        else if( time % 3 == 0 )
            pinLED = pLED_GREEN;
        else
            pinLED = pLED_BLUE;

        led(pinLED, 300, 100);
    }
}

void led( byte pin, unsigned long timeHigh, unsigned long timeLow )
{
    digitalWrite(pin, HIGH);
    delay(timeHigh);
    digitalWrite(pin, LOW);
    delay(timeLow);
}