/**
 * Name:     Smarthome: Touch switch
 * Autor:    Alberto Gil Tesa
 * Web:      https://giltesa.com
 * License:  CC BY-NC-SA 3.0
 * Version:  1.0
 * Date:     2018/01/02
 *
 */

#include <OneWire.h>
#include <DallasTemperature.h>

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


OneWire oneWire(pDS18B20);
DallasTemperature ds18b20(&oneWire);

void setup()
{
  Serial.begin(9600);
  ds18b20.begin();
}

void loop()
{
  ds18b20.requestTemperatures();

  Serial.print("Temperature: ");
  Serial.print( ds18b20.getTempCByIndex(0) );
  Serial.println(" C");
  delay(500);
}