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

#include <DallasTemperature.h>

DallasTemperature ds18b20(new OneWire(pDS18B20));


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