/**
 * Name:     Smarthome: Sensor Node
 * Autor:    Alberto Gil Tesa
 * Web:      https://giltesa.com/smarthome
 * License:  CC BY-NC-SA 4.0
 * Version:  1.0
 * Date:     2018/03/13
 *
 */


#include <HTU21D.h>
HTU21D htu;


void setup()
{
    Serial.begin(9600);
    while(!Serial);

    htu.begin();
}


void loop()
{
    Serial.print("Temperature: ");
    Serial.print(htu.readTemperature(), 1);
    Serial.print(" C");

    Serial.print(", Humidity: ");
    Serial.print(htu.readHumidity(), 1);
    Serial.println(" %RH");

    delay(1000);
}