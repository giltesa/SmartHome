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

void setup()
{
    pinMode(pBTN,       INPUT);
    pinMode(pLED_RED,   OUTPUT);
    pinMode(pLED_GREEN, OUTPUT);
    pinMode(pLED_BLUE,  OUTPUT);
    pinMode(pRELAY_1,   OUTPUT);
    pinMode(pRELAY_2,   OUTPUT);

    //Start:
    led('B'); delay(200); led('0'); delay(100);
    led('B'); delay(200); led('0'); delay(100);
    led('B'); delay(200); led('0'); delay(500);
    led('R');
}

void loop()
{
    if( digitalRead(pBTN) )
    {
        while( digitalRead(pBTN) );
        delay(100);

        if( !digitalRead(pRELAY_1) && !digitalRead(pRELAY_2) )
        {
            digitalWrite(pRELAY_1, HIGH);
            led('G');
        }
        else if( digitalRead(pRELAY_1) && !digitalRead(pRELAY_2) )
        {
            digitalWrite(pRELAY_1, LOW);
            digitalWrite(pRELAY_2, HIGH);
            led('Y');
        }
        else if( !digitalRead(pRELAY_1) && digitalRead(pRELAY_2) )
        {
            digitalWrite(pRELAY_1, HIGH);
            led('W');
        }
        else if( digitalRead(pRELAY_1) && digitalRead(pRELAY_2) )
        {
            digitalWrite(pRELAY_1, LOW);
            digitalWrite(pRELAY_2, LOW);
            led('R');
        }
    }
}

void led( char color )
{
    switch( color )
    {
        case 'R': //Relays off
            digitalWrite(pLED_RED,   HIGH);
            digitalWrite(pLED_GREEN, LOW);
            digitalWrite(pLED_BLUE,  LOW);
            break;
        case 'G':  //Relay 1
            digitalWrite(pLED_RED,   LOW);
            digitalWrite(pLED_GREEN, HIGH);
            digitalWrite(pLED_BLUE,  LOW);
            break;
        case 'B':
            digitalWrite(pLED_RED,   LOW);
            digitalWrite(pLED_GREEN, LOW);
            digitalWrite(pLED_BLUE,  HIGH);
            break;
        case 'Y':  //Relay 2
            digitalWrite(pLED_RED,   HIGH);
            digitalWrite(pLED_GREEN, HIGH);
            digitalWrite(pLED_BLUE,  LOW);
            break;
        case 'W':  //Relay 1 & 2
            digitalWrite(pLED_RED,   HIGH);
            digitalWrite(pLED_GREEN, HIGH);
            digitalWrite(pLED_BLUE,  HIGH);
            break;
        default:
            digitalWrite(pLED_RED,   LOW);
            digitalWrite(pLED_GREEN, LOW);
            digitalWrite(pLED_BLUE,  LOW);
            break;
    }
}