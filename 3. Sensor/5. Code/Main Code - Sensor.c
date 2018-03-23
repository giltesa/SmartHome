/**
 * Name:     Smarthome: Sensor
 * Autor:    Alberto Gil Tesa
 * Web:      https://giltesa.com/smarthome
 * License:  CC BY-NC-SA 4.0
 * Version:  1.0
 * Date:     2018/03/24
 *
 */


/**
 *  Pinout V1.0
 */
#define pEXP_A4_SDA  A4 //Reserved for I2C port
#define pEXP_A5_SCL  A5 //Reserved for I2C port
#define pEXP_D3_INT2  3 //Interruption
#define pEXP_D5_PWM   5 //PWM
#define pEXP_A0_ANG  A0
#define pEXP_D2_INT1  2 //Interruption //Shared with magnetic switch.
#define pBATTERY     A1
#define pLED_BLUE     9 //PWM
#define pNRF_CE       7
#define pNRF_CS      10
#define pFLASH_CS     8
#define pATSHA204A   A3



/**
 * MySensor configuration:
 */
#define MY_DEBUG                                    // Enable debug prints to serial monitor
#define MY_BAUD_RATE                    9600        // Serial output baud rate
#define MY_TRANSPORT_WAIT_READY_MS      1000        // Set how long to wait for transport ready in milliseconds
#define MY_RADIO_NRF24                              // Enable and select radio type attached
#define MY_RF24_CE_PIN                  pNRF_CE     // Define this to change the chip enable pin from the default
#define MY_RF24_CS_PIN                  pNRF_CS     // Define this to change the chip select pin from the default
//#define MY_REPEATER_FEATURE                       // Enable repeater functionality for this node
#define MY_SMART_SLEEP_WAIT_DURATION_MS	1000        // The wait period (in ms) before going to sleep when using smartSleep-functions.
#define MY_OTA_FIRMWARE_FEATURE                     // Define this in sketch to allow safe over-the-air firmware updates
#define MY_OTA_FLASH_SS                 pFLASH_CS   // Slave select pin for external flash. (The bootloader must use the same pin)
#define MY_OTA_FLASH_JDECID             0xEF30      // https://forum.mysensors.org/topic/4267/w25x40clsnig-as-flash-for-ota

#define MS_BOARD_NAME                   "Sensor"
#define MS_SOFTWARE_VERSION             "1.0"
#define MS_SLEEP_TIME                   10000
#define MS_DOOR_CHILD_ID                0
#define MS_TEMP_CHILD_ID                1
#define MS_HUMI_CHILD_ID                2
#define MS_LIGHT_CHILD_ID               3

#include <MySensors.h>
#include <HTU21D.h>
#include <BH1750.h>
#include <AsyncTaskLib.h>


MyMessage msgDOOR(MS_DOOR_CHILD_ID,   V_TRIPPED);
MyMessage msgTEMP(MS_TEMP_CHILD_ID,   V_TEMP);
MyMessage msgHUMI(MS_HUMI_CHILD_ID,   V_HUM);
MyMessage msgLIGHT(MS_LIGHT_CHILD_ID, V_LEVEL);

HTU21D htu;
BH1750 bh;

AsyncTask *taskOn, *taskOff;



/**
 * For initialisations that needs to take place before MySensors transport has been setup (eg: SPI devices).
 */
void before()
{
    pinMode(pEXP_D2_INT1, INPUT);
    pinMode(pLED_BLUE,    OUTPUT);

    digitalWrite(pLED_BLUE, LOW);
}



/**
 * Called once at startup, usually used to initialize sensors.
 */
void setup()
{
    analogReference(INTERNAL);

    htu.begin();
    bh.begin();
    bh.configure(BH1750_CONTINUOUS_LOW_RES_MODE);

    taskOn  = new AsyncTask(800, [](){ digitalWrite(pLED_BLUE, HIGH); });
    taskOff = new AsyncTask(400, [](){ digitalWrite(pLED_BLUE, LOW);  });
}



/**
 * This allows controller to re-request presentation and do re-configuring node after startup.
 */
void presentation()
{
    sendSketchInfo(MS_BOARD_NAME, MS_SOFTWARE_VERSION);

    present(MS_DOOR_CHILD_ID,  S_DOOR,        "Door switch");
    present(MS_TEMP_CHILD_ID,  S_TEMP,        "Temperature");
    present(MS_HUMI_CHILD_ID,  S_HUM,         "Humidity");
    present(MS_LIGHT_CHILD_ID, S_LIGHT_LEVEL, "Light");
}



/**
 * This will be called continuously after setup.
 */
void loop()
{
    //In case of problems in the wireless connection, the LED flashes blue.
    if( !isTransportReady() )
    {
        if( !taskOn->IsActive() ){
            taskOn->Start();
        }
        taskOn->Update(*taskOff);
        taskOff->Update(*taskOn);
    }
    else if( taskOn->IsActive() || taskOff->IsActive() )
    {
        taskOn->Stop();
        taskOff->Stop();
        digitalWrite(pLED_BLUE, LOW);
    }
    else
    {
        int valueSleep = smartSleep(digitalPinToInterrupt(pEXP_D2_INT1), HIGH, MS_SLEEP_TIME);

        if( valueSleep == MY_SLEEP_NOT_POSSIBLE )
        {
            #ifdef MY_DEBUG
                Serial.println(">>> Unable to sleep");
            #endif
        }
        else if( valueSleep == digitalPinToInterrupt(pEXP_D2_INT1) )
        {
            //Door:
            #ifdef MY_DEBUG
                Serial.println(">>> Door active");
            #endif
            send(msgDOOR.set(true));
        }
        else if( valueSleep == MY_WAKE_UP_BY_TIMER )
        {
            #ifdef MY_DEBUG
                Serial.println(">>> Sensors active");
            #endif

            //Temperature, Humidity, Light, Battery level:
            send(msgTEMP.set(getControllerConfig().isMetric ? htu.readTemperature() : ((htu.readTemperature() * 9.0) / 5.0 + 32), 1));
            send(msgHUMI.set(htu.readHumidity(), 1));
            send(msgLIGHT.set(bh.readLightLevel()));
            sendBatteryLevel(getBatteryLevel());
        }
        else
        {
            #ifdef MY_DEBUG
                Serial.print(">>> valueSleep = "); Serial.println(valueSleep);
            #endif
        }

    }

    #ifdef MY_DEBUG
        Serial.println(">>> HI?"); //If the node is sleeping correctly, this will only be printed every MS_SLEEP_TIME.
    #endif
}



/**
 *
 */
void receive(const MyMessage &message)
{
}



/**
 * https://www.openhardware.io/view/173/Battery-Powered-Sensors
 *
 * 1M, 470K divider across battery and using internal ADC ref of 1.1V
 * Sense point is bypassed with 0.1 uF cap to reduce noise at that point
 * ((1e6+470e3)/470e3)*1.1 = Vmax = 3.44 Volts
 * 3.44/1023 = Volts per bit = 0.003363075
 */
float getBatteryLevel()
{
    int sensorValue = analogRead(pBATTERY);
    int batteryPcnt = sensorValue / 10;

    #ifdef MY_DEBUG
        float batteryV  = sensorValue * 0.003363075;
        Serial.print("Battery voltage: "); Serial.print(batteryV);    Serial.println(" V");
        Serial.print("Battery percent: "); Serial.print(batteryPcnt); Serial.println(" %");
    #endif

    return batteryPcnt;
}