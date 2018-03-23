/**
 * Name:     Smarthome: LED RGBW Controller
 * Autor:    Alberto Gil Tesa
 * Web:      https://giltesa.com/smarthome
 * License:  CC BY-NC-SA 4.0
 * Version:  1.0
 * Date:     2018/03/17
 *
 */


/**
 *  Pinout V1.0
 */
#define pLED_RED    5 //PWM
#define pLED_GREEN  6 //PWM
#define pLED_BLUE   9 //PWM
#define pLED_WHITE  3 //PWM
#define pNRF_CE     7
#define pNRF_CS    10
#define pFLASH_CS   8
#define pDS18B20   A0
#define pATSHA204A A3


void setup()
{
    pinMode(pLED_RED,   OUTPUT);
    pinMode(pLED_GREEN, OUTPUT);
    pinMode(pLED_BLUE,  OUTPUT);
    pinMode(pLED_WHITE, OUTPUT);
}


void loop()
{
    switch( random(1, 5) )
    {
        case 1:
            fadeLed(pLED_RED);
            break;
        case 2:
            fadeLed(pLED_GREEN);
            break;
        case 3:
            fadeLed(pLED_BLUE);
            break;
        case 4:
            fadeLed(pLED_WHITE);
            break;
    }

    delay(10);
}


void fadeLed( byte pLED)
{
    int i;

    for( i=0 ; i < 255 ; i++ )
    {
        analogWrite(pLED, i);
        delay(5);
    }

    for( i ; i >= 0 ; i-- )
    {
        analogWrite(pLED, i);
        delay(5);
    }
}