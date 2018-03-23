/**
 * Name:     Smarthome: LED RGBW Controller
 * Autor:    Alberto Gil Tesa
 * Web:      https://giltesa.com/smarthome
 * License:  CC BY-NC-SA 4.0
 * Version:  1.0
 * Date:     2018/03/22
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



/**
 * MySensor configuration:
 */
#define MY_DEBUG                                // Enable debug prints to serial monitor
#define MY_BAUD_RATE                9600        // Serial output baud rate
#define MY_TRANSPORT_WAIT_READY_MS  1000        // Set how long to wait for transport ready in milliseconds
#define MY_RADIO_NRF24                          // Enable and select radio type attached
#define MY_RF24_CE_PIN              pNRF_CE     // Define this to change the chip enable pin from the default
#define MY_RF24_CS_PIN              pNRF_CS     // Define this to change the chip select pin from the default
//#define MY_REPEATER_FEATURE                   // Enable repeater functionality for this node
#define MY_OTA_FIRMWARE_FEATURE                 // Define this in sketch to allow safe over-the-air firmware updates
#define MY_OTA_FLASH_SS             pFLASH_CS   // Slave select pin for external flash. (The bootloader must use the same pin)
#define MY_OTA_FLASH_JDECID         0xEF30      // https://forum.mysensors.org/topic/4267/w25x40clsnig-as-flash-for-ota

#define MS_BOARD_NAME               "LED RGBW Controller"
#define MS_SOFTWARE_VERSION         "1.0"
#define MS_STATUS_CHILD_ID          0
#define MS_RGB_CHILD_ID             1
#define MS_RGBW_CHILD_ID            2
#define MS_WHITE_CHILD_ID           3
#define MS_TEMP_CHILD_ID            4

#include <MySensors.h>
#include <DallasTemperature.h>
#include <AsyncTaskLib.h>

MyMessage msgSTAT(MS_STATUS_CHILD_ID, V_STATUS);
MyMessage msgTEMP(MS_TEMP_CHILD_ID,   V_TEMP);

DallasTemperature ds18b20(new OneWire(pDS18B20));
AsyncTask *taskOn, *taskOff, *taskTemp;

struct RGBW{byte r, g, b, w;} led;



/**
 * For initialisations that needs to take place before MySensors transport has been setup (eg: SPI devices).
 */
void before()
{
    pinMode(pLED_RED,   OUTPUT);
    pinMode(pLED_GREEN, OUTPUT);
    pinMode(pLED_BLUE,  OUTPUT);
    pinMode(pLED_WHITE, OUTPUT);

    setLedColor('0');
}



/**
 * Called once at startup, usually used to initialize sensors.
 */
void setup()
{
    ds18b20.begin();

    taskOn  = new AsyncTask(4000, [](){ setLedColor('B'); });
    taskOff = new AsyncTask(1000, [](){ setLedColor('0'); });

    #ifdef MY_DEBUG
        taskTemp = new AsyncTask(60000, true, [](){ sendTemperature(); });
        taskTemp->Start();
    #endif

  //request(MS_STATUS_CHILD_ID, V_STATUS);
  //request(MS_RGB_CHILD_ID,    V_RGB);
  //request(MS_RGBW_CHILD_ID,   V_RGBW);
  //request(MS_WHITE_CHILD_ID,  V_DIMMER);
}



/**
 * This allows controller to re-request presentation and do re-configuring node after startup.
 */
void presentation()
{
    sendSketchInfo(MS_BOARD_NAME, MS_SOFTWARE_VERSION);

    present(MS_STATUS_CHILD_ID, S_BINARY,     "LED On/Off");
    present(MS_RGB_CHILD_ID,    S_RGB_LIGHT,  "LED RGB");
    present(MS_RGBW_CHILD_ID,   S_RGBW_LIGHT, "LED RGBW");
    present(MS_WHITE_CHILD_ID,  S_DIMMER,     "LED White");
    present(MS_TEMP_CHILD_ID,   S_TEMP,       "Temp. Int.");
}



/**
 * This will be called continuously after setup.
 */
void loop()
{
    #ifdef MY_DEBUG
        taskTemp->Update();
    #endif


    //In case of problems in the wireless connection, the LED flashes blue.
    if( !isTransportReady() )
    {
        if( !taskOn->IsActive() ){
            taskOn->Start();
        }
        taskOn->Update(*taskOff);
        taskOff->Update(*taskOn);

    }
    else if( taskOn->IsActive() )
    {
        taskOn->Stop();
        setLedColor('0');
    }
}



/**
 *
 */
void receive(const MyMessage &message)
{
    if( message.sensor == MS_STATUS_CHILD_ID || message.sensor == MS_RGB_CHILD_ID || message.sensor == MS_RGBW_CHILD_ID || message.sensor == MS_WHITE_CHILD_ID )
    {
        if( message.type == V_STATUS )
        {
            bool status = message.getBool();

            if( status && led.r == 0 && led.g == 0 && led.b == 0 && led.w == 0 ){
                led.w = 128;
            }

            analogWrite(pLED_RED,   status ? led.r : 0);
            analogWrite(pLED_GREEN, status ? led.g : 0);
            analogWrite(pLED_BLUE,  status ? led.b : 0);
            analogWrite(pLED_WHITE, status ? led.w : 0);
        }
        else if( message.type == V_RGB || message.type == V_RGBW )
        {
            String hex = message.getString();

            if( hex.startsWith("#") ){
                hex = hex.substring(1, hex.length());
            }

            if( hex.length() == 6 || hex.length() == 8 )
            {
                led.r = strHexToDec(hex.substring(0, 2));
                led.g = strHexToDec(hex.substring(2, 4));
                led.b = strHexToDec(hex.substring(4, 6));
                led.w = hex.length() == 8 ? strHexToDec(hex.substring(6, 8)) : 0;

                analogWrite(pLED_RED,   led.r);
                analogWrite(pLED_GREEN, led.g);
                analogWrite(pLED_BLUE,  led.b);
                analogWrite(pLED_WHITE, led.w);

                send(msgSTAT.set(led.r > 0 || led.g > 0 || led.b > 0 || led.w > 0));
            }
        }
        else if( message.type == V_DIMMER )
        {
            led.w = map(message.getInt(), 0, 100, 0, 255);
            analogWrite(pLED_RED,   0);
            analogWrite(pLED_GREEN, 0);
            analogWrite(pLED_BLUE,  0);
            analogWrite(pLED_WHITE, led.w);

            send(msgSTAT.set(led.w > 0));
        }

        #ifndef MY_DEBUG
            sendTemperature();
        #endif
    }
}



/**
 *
 */
byte strHexToDec(String str)
{
    char charbuf[3];
    str.toCharArray(charbuf, 3);
    return strtol(charbuf, 0, 16);
}



/**
 *
 */
void sendTemperature()
{
    ds18b20.requestTemperatures();
    send(msgTEMP.set(getControllerConfig().isMetric ? ds18b20.getTempCByIndex(0) : ds18b20.getTempFByIndex(0), 1));
}



/**
 * Turn on the led in the indicated color.
 */
void setLedColor( char color )
{
    switch( color )
    {
        case 'R':   //RED
            digitalWrite(pLED_RED,   HIGH);
            digitalWrite(pLED_GREEN, LOW);
            digitalWrite(pLED_BLUE,  LOW);
            digitalWrite(pLED_WHITE, LOW);
            break;
        case 'G':   //GREEN
            digitalWrite(pLED_RED,   LOW);
            digitalWrite(pLED_GREEN, HIGH);
            digitalWrite(pLED_BLUE,  LOW);
            digitalWrite(pLED_WHITE, LOW);
            break;
        case 'B':   //BLUE
            digitalWrite(pLED_RED,   LOW);
            digitalWrite(pLED_GREEN, LOW);
            digitalWrite(pLED_BLUE,  HIGH);
            digitalWrite(pLED_WHITE, LOW);
            break;
        case 'W':   //WHITE
            digitalWrite(pLED_RED,   LOW);
            digitalWrite(pLED_GREEN, LOW);
            digitalWrite(pLED_BLUE,  LOW);
            digitalWrite(pLED_WHITE, HIGH);
            break;
        default:    //OFF
            digitalWrite(pLED_RED,   LOW);
            digitalWrite(pLED_GREEN, LOW);
            digitalWrite(pLED_BLUE,  LOW);
            digitalWrite(pLED_WHITE, LOW);
            break;
    }
}