/**
 * Name:     Smarthome: Touch Switch Two Light (MySensors)
 * Autor:    Alberto Gil Tesa
 * Web:      https://giltesa.com/smarthome
 *           https://giltesa.com/?p=18460
 * License:  CC BY-NC-SA 4.0
 * Version:  1.2
 * Date:     2018/03/24
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
//#define MY_OTA_FLASH_SS           pFLASH_CS   // Slave select pin for external flash. (The bootloader must use the same pin)
#define MY_OTA_FLASH_JDECID         0xEF30      // https://forum.mysensors.org/topic/4267/w25x40clsnig-as-flash-for-ota

#define MS_BOARD_NAME               "Touch Switch: Two light"
#define MS_SOFTWARE_VERSION         "1.2"
#define MS_RELAY1_CHILD_ID          0
#define MS_RELAY2_CHILD_ID          1
#define MS_LEDPWM_CHILD_ID          2
#define MS_TEMP_CHILD_ID            3

#include <MySensors.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <AsyncTaskLib.h>

MyMessage msgR1(MS_RELAY1_CHILD_ID, V_STATUS);
MyMessage msgR2(MS_RELAY2_CHILD_ID, V_STATUS);
MyMessage msgT1(MS_TEMP_CHILD_ID,   V_TEMP);

DallasTemperature ds18b20(new OneWire(pDS18B20));

AsyncTask *taskOn, *taskOff, *taskTemp;
byte brightnessLed = 255;

         const  unsigned long tDebouncePress = 50;
         const  unsigned long tWaitTwoPress  = 400;
         const  unsigned long tShortPress    = 300;
volatile static unsigned long tBtnPressed1   = 0;
volatile static unsigned long tBtn1Released1 = 0;
volatile static unsigned long tBtnPressed2   = 0;
volatile static unsigned long tBtn1Released2 = 0;
         static boolean       sBtnPressed    = false;



/**
 * For initialisations that needs to take place before MySensors transport has been setup (eg: SPI devices).
 */
void before()
{
    pinMode(pBTN,       INPUT);
    pinMode(pLED_RED,   OUTPUT);
    pinMode(pLED_GREEN, OUTPUT);
    pinMode(pLED_BLUE,  OUTPUT);
    pinMode(pRELAY_1,   OUTPUT);
    pinMode(pRELAY_2,   OUTPUT);

    attachInterrupt(digitalPinToInterrupt(pBTN), btnInterrupt,  CHANGE);

    setLedColor('0');
}



/**
 * Called once at startup, usually used to initialize sensors.
 */
void setup()
{
    ds18b20.begin();

    taskOn  = new AsyncTask(4000, [](){ setLedColor('B');   });
    taskOff = new AsyncTask(1000, [](){ refreshLedStatus(); });

    #ifdef MY_DEBUG
        taskTemp = new AsyncTask(60000, true, [](){ sendTemperature(); });
        taskTemp->Start();
    #endif

    request(MS_RELAY1_CHILD_ID, V_STATUS);
    request(MS_RELAY2_CHILD_ID, V_STATUS);
    request(MS_LEDPWM_CHILD_ID, V_DIMMER);

    //Start:
    setLedColor('R');
}



/**
 * This allows controller to re-request presentation and do re-configuring node after startup.
 */
void presentation()
{
    sendSketchInfo(MS_BOARD_NAME, MS_SOFTWARE_VERSION);

    present(MS_RELAY1_CHILD_ID, S_BINARY, "Light 1");
    present(MS_RELAY2_CHILD_ID, S_BINARY, "Light 2");
    present(MS_LEDPWM_CHILD_ID, S_DIMMER, "LED brightness");
    present(MS_TEMP_CHILD_ID,   S_TEMP,   "Internal temp");
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
    else if( taskOn->IsActive() || taskOff->IsActive() )
    {
        taskOn->Stop();
        taskOff->Stop();
        refreshLedStatus();
    }


    //Logic for the touch button.
    //Depending on the number of button presses and the duration, one or the other action is carried out:
    if( tBtn1Released1 > 0 && millis() - tBtn1Released1 > tWaitTwoPress && tBtn1Released2 == 0 ) //One pulsation
    {
        sBtnPressed = true;
        if( tBtn1Released1 - tBtnPressed1 <= tShortPress ) //Short press
        {
            digitalWrite(pRELAY_1, !digitalRead(pRELAY_1));
            send(msgR1.set(digitalRead(pRELAY_1)));
        }
        else if( tBtn1Released1 - tBtnPressed1 > tShortPress ) //Long press
        {
            boolean status = ( !digitalRead(pRELAY_1) && !digitalRead(pRELAY_2) );
            digitalWrite(pRELAY_1, status);
            digitalWrite(pRELAY_2, status);
            send(msgR1.set(status));
            send(msgR2.set(status));
        }
    }
    else if( tBtn1Released1 > 0 && tBtn1Released2 > 0 ) //Double pulsation
    {
        sBtnPressed = true;
        digitalWrite(pRELAY_2, !digitalRead(pRELAY_2));
        send(msgR2.set(digitalRead(pRELAY_2)));
    }


    if( sBtnPressed )
    {
        sBtnPressed  = false;
        tBtnPressed1 = tBtn1Released1 = tBtnPressed2 = tBtn1Released2 = 0;
        refreshLedStatus();

        #ifndef MY_DEBUG
            sendTemperature();
        #endif
    }

}



/**
 * Get the time when the button is pressed and released.
 */
void btnInterrupt()
{
    static boolean sInterrupt = false;

    noInterrupts();
    sInterrupt = !sInterrupt;

    if( sInterrupt ){
        setLedColor('0');
    }

    if( tBtn1Released1 == 0 )
    {
        if( sInterrupt ){
            tBtnPressed1 = millis();
        }else{
            tBtn1Released1 = millis();
        }
    }
    else if( tBtn1Released2 == 0 && millis() - tBtn1Released1 > tDebouncePress )
    {
        if( sInterrupt ){
            tBtnPressed2 = millis();
        }else{
            tBtn1Released2 = millis();
        }
    }

    if( !sInterrupt ){
        refreshLedStatus();
    }
    interrupts();
}



/**
 *
 */
void receive(const MyMessage &message)
{
    if( (message.sensor == MS_RELAY1_CHILD_ID || _msg.destination == 255) && message.type == V_STATUS )
    {
        digitalWrite(pRELAY_1, message.getBool());
        refreshLedStatus();
    }
    else if( message.sensor == MS_RELAY2_CHILD_ID && message.type == V_STATUS )
    {
        digitalWrite(pRELAY_2, message.getBool());
        refreshLedStatus();
    }
    else if( message.sensor == MS_LEDPWM_CHILD_ID && message.type == V_DIMMER )
    {
        brightnessLed = map(message.getInt(), 0, 100, 25, 255);
        refreshLedStatus();
    }
}



/**
 *
 */
void sendTemperature()
{
    ds18b20.requestTemperatures();
    send(msgT1.set(getControllerConfig().isMetric ? ds18b20.getTempCByIndex(0) : ds18b20.getTempFByIndex(0), 1));
}



/**
 * Sets the color of the led according to the state of the relays.
 */
void refreshLedStatus()
{
    if( digitalRead(pRELAY_1) && !digitalRead(pRELAY_2) ){
        setLedColor('G');
    }
    else if( !digitalRead(pRELAY_1) && digitalRead(pRELAY_2) ){
        setLedColor('Y');
    }
    else if( digitalRead(pRELAY_1) && digitalRead(pRELAY_2) ){
        setLedColor('W');
    }
    else if( !digitalRead(pRELAY_1) && !digitalRead(pRELAY_2) ){
        setLedColor('R');
    }
}



/**
 * Turn on the led in the indicated color.
 */
void setLedColor( char color )
{
    switch( color )
    {
        case 'R':   //RED
            analogWrite(pLED_RED,    brightnessLed);
            digitalWrite(pLED_GREEN, LOW);
            digitalWrite(pLED_BLUE,  LOW);
            break;
        case 'G':   //GREEN
            digitalWrite(pLED_RED,   LOW);
            analogWrite(pLED_GREEN,  brightnessLed);
            digitalWrite(pLED_BLUE,  LOW);
            break;
        case 'B':   //BLUE
            digitalWrite(pLED_RED,   LOW);
            digitalWrite(pLED_GREEN, LOW);
            analogWrite(pLED_BLUE,  brightnessLed);
            break;
        case 'Y':   //YELLOW
            analogWrite(pLED_RED,    brightnessLed);
            analogWrite(pLED_GREEN,  brightnessLed);
            digitalWrite(pLED_BLUE,  LOW);
            break;
        case 'W':   //WHITE
            analogWrite(pLED_RED,   brightnessLed);
            analogWrite(pLED_GREEN, brightnessLed);
            analogWrite(pLED_BLUE,  brightnessLed);
            break;
        default:    //OFF
            digitalWrite(pLED_RED,   LOW);
            digitalWrite(pLED_GREEN, LOW);
            digitalWrite(pLED_BLUE,  LOW);
            break;
    }
}