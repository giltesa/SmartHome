/**
 * Name:     Smarthome: Touch switch
 * Autor:    Alberto Gil Tesa
 * Web:      https://giltesa.com/smarthome
 *           https://giltesa.com/?p=18460
 * License:  CC BY-NC-SA 4.0
 * Version:  1.0
 * Date:     2018/01/02
 *
 */


/**
 *  Pinout Touch Switch Board V2.2 and Relay Switch Board V1.2
 */
#define pBTN        2 //Interruption
#define pZERO       3 //Interruption (Not used)
#define pRELAY_1   A1
#define pRELAY_2   A2
#define pNRF_CE     7
#define pNRF_CS    10
#define pLED_RED    5 //PWM
#define pLED_GREEN  6 //PWM
#define pLED_BLUE   9 //PWM
#define pFLASH_CS   8
#define pDS18B20   A0
#define pATSHA204A A3


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
    static unsigned long time;
    static byte pinLED;

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