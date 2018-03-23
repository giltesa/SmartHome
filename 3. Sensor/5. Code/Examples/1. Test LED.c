/**
 * Name:     Smarthome: Sensor Node
 * Autor:    Alberto Gil Tesa
 * Web:      https://giltesa.com/smarthome
 * License:  CC BY-NC-SA 4.0
 * Version:  1.0
 * Date:     2018/03/13
 *
 */


#define pLED_BLUE 9 //PWM


void setup()
{
    pinMode(pLED_BLUE, OUTPUT);
}


void loop()
{
    digitalWrite(pLED_BLUE, !digitalRead(pLED_BLUE));
    delay(1000);
}