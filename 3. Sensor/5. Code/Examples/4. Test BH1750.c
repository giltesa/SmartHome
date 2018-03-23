/**
 * Name:     Smarthome: Sensor Node
 * Autor:    Alberto Gil Tesa
 * Web:      https://giltesa.com/smarthome
 * License:  CC BY-NC-SA 4.0
 * Version:  1.0
 * Date:     2018/03/13
 *
 */


#include <BH1750.h>
BH1750 bh;


void setup()
{
    Serial.begin(9600);
    while(!Serial);

    bh.begin();
    bh.configure(BH1750_CONTINUOUS_LOW_RES_MODE);
}


void loop()
{
    Serial.print("Light: ");
    Serial.print(bh.readLightLevel());
    Serial.println(" lux");

    delay(1000);
}