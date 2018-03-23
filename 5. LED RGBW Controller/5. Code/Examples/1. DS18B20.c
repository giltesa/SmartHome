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

#include <DallasTemperature.h>


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