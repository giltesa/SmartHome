/**
 * Name:     Smarthome: Sensor Node
 * Autor:    Alberto Gil Tesa
 * Web:      https://giltesa.com/smarthome
 * License:  CC BY-NC-SA 4.0
 * Version:  1.0
 * Date:     2018/03/13
 *
 */


#define pBTN      2 //Interruption
#define pLED_BLUE 9 //PWM


void setup()
{
    pinMode(pBTN,      INPUT);
    pinMode(pLED_BLUE, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(pBTN), btnInterrupt,  RISING);
}


void loop()
{
}


void btnInterrupt()
{
    static boolean status = false;

    noInterrupts();

    status = !status;
    digitalWrite(pLED_BLUE, status);

    interrupts();
}