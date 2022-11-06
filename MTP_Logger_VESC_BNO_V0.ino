  #include <SPI.h>
#include "SdFat.h"
#include "FreeStack.h"

#include <SD.h>
#include <MTP_Teensy.h>

#include <i2c_t3.h>
#include <Adafruit_SSD1306_i2ct3.h>
#include <Adafruit_GFX.h>

#include <MAX31855.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055_t3.h>

#include <TimeLib.h>
#include <DS1307RTC_i2ct3.h>  // a basic DS1307 library that returns time as a time_t

#include <PID_v1.h>
#include "arduinoFFT.h"

#include "Global_Variable_List.h"  // Edit this include file to change data_t.
#include "VescVariables.h"   // Vesc variables

#include <VescUart.h>    // -------- VESC Communication 
#include <datatypes.h>   // -------- VESC Communication 

#define SCB_AIRCR (*(volatile uint32_t *)0xE000ED0C) // Application Interrupt and Reset Control location

//------------------------------------------------
// HARDWARE DEFINITION
//------------------------------------------------

#define RIGHT_BUTTON_PIN    1
#define MIDDLE_BUTTON_PIN   2
#define LEFT_BUTTON_PIN     3

#define HALL_SENSOR_SPEED_PIN 0   //hallEffectSensorPinSpeed
#define HALL_SENSOR_ONE_PIN   5
#define HALL_SENSOR_TWO_PIN   4

// VESC Communication
//Use pin RX = 7 and TX = 8 for Serial 3 communication

// Module MAX31855 on accelero port
#define MAXDO     16              // Defining the MISO pin
#define MAXCS     14              // Defining the CS pin
#define MAXCLK    15              // Defining the SCK pin

//Accelero Communication
#define ACCELERO_X_PIN   A0
#define ACCELERO_Y_PIN   A1
#define ACCELERO_Z_PIN   A2

//Analog Input 
#define ANALOG_ONE_PIN   A6
#define ANALOG_TWO_PIN   A7

// Chip selects
#define CS_3204          9          // Chip select for the ADC
#define SD_CS_PIN        10         // SD chip select pin.

//------------------------------------------------
// HARDWARE GLOBAL DEF
//------------------------------------------------

#define RESOLUTION_ADC      10      // --------------Teensy Analog input
#define DEBOUNCE_DURATION   100     // Debounce en ms

MAX31855 thermocouple;

#define BNO055_SAMPLERATE_DELAY_MS (100)
Adafruit_BNO055 bno = Adafruit_BNO055(WIRE_BUS, -1, 0x28, I2C_MASTER, I2C_PINS_18_19, I2C_PULLUP_INT, I2C_RATE_600, I2C_OP_MODE_ISR);  

//-----------------------------------------------
// -- SD CARD OPERATIONS
#define LOG_INTERVAL_VAL    12 //Value between whicj we can select the speed of record
uint32_t LOG_INTERVAL_USEC_TABLE[LOG_INTERVAL_VAL]={666,1000,2000,4000,8000,10000,20000,50000,100000,200000,500000,1000000};
int log_interval_selector = 2;      
uint32_t LOG_INTERVAL_USEC = LOG_INTERVAL_USEC_TABLE[log_interval_selector];


// FIFO SIZE - Value; can be 1 4 16
#define FIFO_SIZE_SECTORS 16
// Preallocate 1GiB file.
const uint32_t PREALLOCATE_SIZE_MiB = 1024UL;
// Try max SPI clock for an SD. Reduce SPI_CLOCK if errors occur.
#define SPI_CLOCK SD_SCK_MHZ(50)
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, SPI_CLOCK)  // could be as well SHARED_SPI

const uint64_t PREALLOCATE_SIZE  =  (uint64_t)PREALLOCATE_SIZE_MiB << 20;
// Max length of file name including zero byte.
#define FILE_NAME_DIM 40
// Max number of records to buffer while SD is busy.
const size_t FIFO_DIM = 512*FIFO_SIZE_SECTORS/sizeof(data_t);

typedef SdFat32 sd_t;
typedef File32 file_t;

sd_t sd;
csd_t m_csd;

file_t binFile;
file_t csvFile;
file_t dir;

// You may modify the filename.  Digits before the dot are file versions.
char binName[] = "FastLog00.bin";
String global_CsvName; // this variable halp to acces the name of the last file globaly

// -- SLOW SD SAVING PARAMETERS DEFINITION

#define SLOW_NAME_SIZE "sLog"
const uint8_t BASE_NASS_SIZE = sizeof(SLOW_NAME_SIZE) - 1;
char fileName[13] = SLOW_NAME_SIZE "00.csv";

//SdFile file;
file_t file;

// -- STATS saving variables

int cardType;
uint32_t volFree;
float freeSpace ;
float cardSize ;

//------------------------------------------------
// OPERATIONAL VARIABLES DEFINITION
//------------------------------------------------

// ------------- Digital inputs
//Button and Hall effect Sensors

unsigned long lastButtonAction = millis();
unsigned long lastLeftButtonAction = millis();
unsigned long lastMiddleButtonAction = millis();
unsigned long lastRightButtonAction = millis();

// Buttons actions
int rightButtonPressed = false;     //boolean for the button 
int middleButtonPressed = false;    //boolean for the button 
int leftButtonPressed = false;      //boolean for the button 

// ------------SD card
bool mtpActivated = false;
bool carteSdIn;                 //True ifScCard in, flase if not
int logging = false;            //Affect the interrupt, to prevent error when recording

// ----------- Temperature sensor MAX31855
bool thermocoupleDetected = false;

// ------------IMU BNO055
bool imuDetected;
unsigned long lastImuDataDate;  // Will store the date of last IMU data aquisition (for potential numerical treatement);
sensors_event_t orientationData , angVelocityData , linearAccelData, accelerationData, gravityData, magnetoData ;

//-------------VESC
bool vescConnection ;               //True if connection with Vesc is detected
unsigned long count;                      // Loop counter for Vesc Uart
struct bldcMeasure measuredVal;         // Type pour enregistrement valeurs comm

#define DEBUGSERIAL Serial // usb serial

// VESC Communication
#define SERIALIO Serial3

// BLUETOOTH Communication
#define SERIAL_BLUETOOTH Serial3

//------------------------------------------------
// SCREEN DEF
//------------------------------------------------

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

//------------------------------------------------
// PROG START
//------------------------------------------------

void setup()
{
  
  analogReadResolution(RESOLUTION_ADC); 

  // --------------- Serial COM Initialization
  SERIALIO.begin(115200);
  SetSerialPort(&SERIALIO);
  
  DEBUGSERIAL.begin(115200); // Serial for Debug
  SetDebugSerialPort(&DEBUGSERIAL);


//-------------------------------------- Pin declaration

  pinMode(MAXDO,INPUT_PULLUP); //MISO Require PullUp to work
  thermocouple.begin(MAXCLK, MAXCS, MAXDO);
  delay(250);
  
  if (thermocouple.read() == STATUS_NO_COMMUNICATION){
    thermocoupleDetected = false;
    DEBUGSERIAL.println("No Thermocoupler");
  }
  else{
    thermocoupleDetected = true;
    DEBUGSERIAL.println("MAX 31855 detected");
  } 

  if (!thermocoupleDetected){  // if no thermoupler, we can allow the pin to work for analog input
    pinMode(ACCELERO_X_PIN,INPUT);
    pinMode(ACCELERO_Y_PIN,INPUT);
    pinMode(ACCELERO_Z_PIN,INPUT);  
  }
  
  pinMode(ANALOG_ONE_PIN,INPUT);
  pinMode(ANALOG_TWO_PIN,INPUT);

  pinMode(RIGHT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(MIDDLE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(LEFT_BUTTON_PIN, INPUT_PULLUP);
  
  pinMode(HALL_SENSOR_SPEED_PIN, INPUT_PULLUP);
  pinMode(HALL_SENSOR_ONE_PIN, INPUT_PULLUP);
  pinMode(HALL_SENSOR_TWO_PIN, INPUT_PULLUP);

  attachInterrupt(RIGHT_BUTTON_PIN,RightBouton,RISING);
  attachInterrupt(MIDDLE_BUTTON_PIN,MiddleBouton,RISING);
  attachInterrupt(LEFT_BUTTON_PIN,LeftBouton,RISING);
  
  attachInterrupt(HALL_SENSOR_SPEED_PIN,OneWheelRotation,FALLING);
  attachInterrupt(HALL_SENSOR_ONE_PIN, pwmCountFalling, FALLING);
  
  pinMode(SD_CS_PIN,OUTPUT);
  pinMode(CS_3204,OUTPUT);
  
  digitalWrite(SD_CS_PIN,HIGH);
  digitalWrite(CS_3204,HIGH);

  //---------------------- SCREEN Initialization
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
  display.clearDisplay();

    
  //-------------------------------TIME 
  // set the Time library to use Teensy 3.0's RTC to keep time
  setSyncProvider(getTeensy3Time);
  
  if (timeStatus()!= timeSet) {
    display.println("Unable to sync with the RTC");
    DEBUGSERIAL.println("Unable to sync with the RTC");
    delay(500);
    } 

  // BootUp Sequence
  ScreenLogo(100);
  
  // ------------------ Speed Sensor initiliazation
  previousMagnetTime =micros();

  //--------------------------------------------------------
  //------------------------------------------SD card system for fast logging
  
  FillStack(); // Function for SD ExFat logger
  if (!sd.begin(SD_CS_PIN, SPI_FULL_SPEED)) {
    carteSdIn = false;
    DEBUGSERIAL.println("Card failed, or not present");  
  }
  else{
    carteSdIn=true;
    DEBUGSERIAL.println("SD Card detected");
    sd.card()->readCSD(&m_csd);
    cardType=sd.fatType();
    volFree=sd.freeClusterCount();
    freeSpace=(0.000512*volFree*sd.sectorsPerCluster());
    cardSize=(0.000512 * sdCardCapacity(&m_csd));
  }

  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV4);

  // USB FILE TRANSFER MODE --  WE enter usb mode if we connect the USB naturally to charge
  
  usbWasConnected=usbIsConnected();
  if (usbIsConnected()&&carteSdIn){
    USB_Communication();
  }

  // ------------------------------------------------

  
  ScreenCheckStatusOne(100);
  
  //--------------------------------------------------------
  // -------------- IMU Initialisation
  if (!bno.begin())
  {
    DEBUGSERIAL.println("No BNO055 detected");
    imuDetected = false;
  }
  else{
    DEBUGSERIAL.println("BNO055 detected");
    imuDetected = true;
  }

  ScreenCheckStatusTwo(100);
  
  //--------------------------------------------------------
  // -------------- VESC Initialisation
  
  if (VescUartGetValue(measuredVal)){
    VescUartGetValue(measuredVal);
    delay (200);
    VescUartGetValue(measuredVal);
    InitialwattHoursLeft = InitialBatteryLevelCalculation(measuredVal.inpVoltage);
    vescConnection=true;
    DEBUGSERIAL.println("VESC detected");
    quelScreen = 4;  // If VESC Detected, we start directly with screen VESC
  }
  
  else{
    vescConnection=false;
    DEBUGSERIAL.println("VESC not detected");
    SERIAL_BLUETOOTH.begin(115200);
    //  SetSerialPort(&SERIAL_BLUETOOTH);
  }
  
  ScreenCheckStatusThree(100);
  ScreenCheckStatusFour(100);
  ResetButton();

  while(!rightButtonPressed){
    delay(10);
  }
     
  ResetButton();
  
}


//-------------------------------------------------------

void loop(void) {

    //------------------------------- GO INTO USB mode by Default if we plug the USB cable
    if (carteSdIn&&usbIsConnected()&&(!usbWasConnected)){
            USB_Communication();
     }
     usbWasConnected=usbIsConnected();  
     
    //------------------------------- ACTION TO DO ------------------------------------
    switch (quelScreen) {
    case 0:
      OnEstPret();   //Landing screen, No action performing in the function
      break;
    case 1:
      Sensors_RedirectingScreen();   //Landing screen, No action performing in the function
      Sensors_BluetoothSend();
      break;
    case 2:
      //SpeedHallScreen();   //Landing screen, No action performing in the function
      screenWheelEfficiencyLanding();
      break;
    case 3:
      IMU_RedirectingScreen();    //Landing screen, No action performing in the function
      break;
    case 4:
      actionVescMode(Vesc_ScreenSelector);   // Mode 0, only screen and Mode 1 is assistance
    break;
    case 5:
      if (thermocoupleDetected){
        Thermocouple_RedirectingScreen();
      }
      else{
        Analogs_RedirectingScreen();
      }  
    break;
  }

            
    if (leftButtonPressed){       //----------------------------------------------------------------------LEFT Button ACTION
      ResetButton();
      
      switch (quelScreen) {  // The programm logData is stopped from inside by pressing the button
        case 0:
          //softRestart();   // Reboot the VESC
          if (carteSdIn&&usbIsConnected()){
            USB_Communication();
          }
          break;
       case 1:
          SensorsMode+= 1;
          SensorsMode= (SensorsMode%nbSensorsMode);
          break;
        case 2:
          SettingsWheelEfficiency();
          break;
        case 3:
          IMU_ScreenSelector+=1; // Change the mode of IMU displayed
          IMU_ScreenSelector=(IMU_ScreenSelector%IMU_NbMode);  
          break;
        case 4:
          Vesc_ScreenSelector+=1;
          Vesc_ScreenSelector=(Vesc_ScreenSelector%Vesc_NbMode);
          break;
        case 5:
          AnalogMode+=1;
          AnalogMode = (AnalogMode%nbAnalogMode);
          break;             
      }
      ResetButton();
    }

    else if (middleButtonPressed){    //----------------------------------------------------------------------MIDDLE Button ACTION
      ResetButton();
      // On change l'écran affiché
      quelScreen+=1;
      quelScreen=(quelScreen%nbTotalScreen);
      
    }       
    
    else if (rightButtonPressed) { //----------------------------------------------------------------------RIGHT Button ACTION
      
      ResetButton();     
      logging=true;
      
      switch (quelScreen) {  // The programm logData is stopped from inside by pressing the button
        case 0:
          if (carteSdIn){
             scrollAndDumpFiles(); // Access all the files and dump through USB 
          }   
          break;        
        case 1:
        if (carteSdIn){
           logging_application = EXTERNAL_MCP_SPEED;
           fastLog_programme();
        }
          break;
        case 2:
          wheelEfficiency(); // Mode Hall effect
          break;
        case 3:
          if (imuDetected&&carteSdIn){
            IMULogData(); // Mode IMU
          }
          break;
        case 4:
          if (vescConnection&&(Vesc_ScreenSelector!=3)){
              VescLogData();  // Mode VESC Logging
          }
          else if (Vesc_ScreenSelector==3){
            Vesc_GraphSelector+=1;
            Vesc_GraphSelector=(Vesc_GraphSelector%Vesc_NbGraph);
          }
          break;
        case 5:
          //The Screen 5 share the mode accelerometer or Thermocouple. We can either slow save or fast save with the thermocouple
          if (carteSdIn&&thermocoupleDetected){
            if (AnalogMode == 0){
              thermocoupleLogData(); 
            }
            else{
              logging_application = THERMOCOUPLE;
              fastLog_programme();
            }
          }
          else if (carteSdIn){
            logging_application = ANALOG_ACCELERO;
            fastLog_programme();
          }
        break;                     
     }
      
      logging=false;     
      ResetButton();
      
    }
  delay(10);
 
}

//-------------------------------------------------------

void USB_Communication(){
    delay(100);
    ResetButton();
    // mandatory to begin the MTP session.
    // while USB not connected > Wait
    
    if (!mtpActivated){
      MTP.begin();
      mtpActivated = true;
      // Add SD Card
      if (SD.begin(SD_CS_PIN)) {
        MTP.addFilesystem(SD, "SD Card");
        Serial.println("Added SD card using built in SDIO, or given SPI CS");
      } else {
        carteSdIn = false;
        Serial.println("No SD Card");
      }
      Serial.println("\nSetup done");
    } 
    Screen_USB_Transfer();
    while((!rightButtonPressed)&&usbIsConnected()){
       MTP.loop();  //This is mandatory to be placed in the loop code.
    }
    ResetButton();
    
    //softRestart();

   if (!sd.begin(SD_CS_PIN, SPI_FULL_SPEED)) {
    carteSdIn = false;
    Serial.println("begin failed");
    displayPopUpMessage("NO SD FAT",1000);
    return;
  }
    
}

//**************************************************************************************
//------------------------------- Bouton interrupt ------------------------------------
//**************************************************************************************

void OneWheelRotation(){
 // set the current speed in km/h
 currentMagnetTime = micros();
 currentSpeed = (1000*3.14159*3.6*ListeDeDiametrePossible[RangDiametre])/((float)(currentMagnetTime-previousMagnetTime)*nbAimentSurRoue);  //On se met en km/h avec passage des micros sce en sec et mm en m
 previousMagnetTime=currentMagnetTime;

 WheelEfficiencyMagnet();
}

void pwmCountRising() {
  attachInterrupt(HALL_SENSOR_ONE_PIN, pwmCountFalling, FALLING);
  prev_time_pwm = micros();
}
 
void pwmCountFalling() {
  attachInterrupt(HALL_SENSOR_ONE_PIN, pwmCountRising, RISING);
  pwm_value = micros()-prev_time_pwm;
  //Serial.println(pwm_value);
}

void ResetButton(){
  leftButtonPressed=false; // Reset button after saving function to prevent any wrong action
  middleButtonPressed=false;
  rightButtonPressed=false;
}


void RightBouton(){
  if ((millis() - lastRightButtonAction) > (DEBOUNCE_DURATION)) {
    rightButtonPressed=true;
  }
  lastRightButtonAction = millis();
  lastButtonAction = millis();
}

void MiddleBouton (){ 
  if ((millis() - lastMiddleButtonAction) > (DEBOUNCE_DURATION)) {
    middleButtonPressed=true;

  }
  lastMiddleButtonAction = millis();
  lastButtonAction = millis();
}


void LeftBouton (){
  if ((millis() - lastLeftButtonAction) > (DEBOUNCE_DURATION)) {
    leftButtonPressed=true;
  }
  lastLeftButtonAction = millis();
  lastButtonAction = millis();
}

void RemoteRightBouton (){
  
  if (digitalRead(HALL_SENSOR_TWO_PIN) == LOW)
  {
    rightButtonPressed=true;
  }

  if (!logging){
    delay (100);
  }
  
}

void softRestart() 
{
  Serial.end();  //clears the serial monitor  if used
  SCB_AIRCR = 0x05FA0004;  //write value for restart
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int OxeloggerBatteryLevel(){
  //read the value on the external ADC and convert in percentage left according to the curve 
  #define sizeOfBuiltInBatCapArray 11
  float percentage;
  float BuiltIn_BatteryMap[sizeOfBuiltInBatCapArray][2] =  
    {  
        {4.2, 100},  
        {4.1, 94},
        {4.0, 85},  
        {3.9, 76},
        {3.8, 66},  
        {3.7, 54},
        {3.6, 26},  
        {3.5, 12},
        {3.4, 5},
        {3.3, 2},  
        {3.2, 0}  }; 
        
  const float BuiltIn_MaxVoltage = BuiltIn_BatteryMap[0][0];
  const float BuiltIn_MinVoltage = BuiltIn_BatteryMap[sizeOfBuiltInBatCapArray-1][0];
  
  // 1 - Voltage Read on 20 points
  
  int somme =0;
  for (int i = 0; i < 20; i++){
    somme+=readADC(2);
    delay(20);
  }
  float voltage = (somme*5.12)/((float)4096*20);
  
  // 2 - Check in maps voltage

  if (voltage>=BuiltIn_MaxVoltage){
    percentage=100;
  }
  else if (voltage<BuiltIn_MinVoltage){
    percentage=0;
  }
  else{ // calculate the voltage left with the battery voltage mapping
    int i =1;
    while (BuiltIn_BatteryMap[i][0]>voltage){
      i=i+1;
    }
    percentage= mapfloat(voltage,BuiltIn_BatteryMap[i][0],BuiltIn_BatteryMap[i-1][0],BuiltIn_BatteryMap[i][1],BuiltIn_BatteryMap[i-1][1]); // Remape the current value from voltage range to percentage range
  }   

  percentage = int(percentage/5)*5;  // Round the number to 5% precision
  
  return (percentage);
}

bool usbIsConnected(){
  return (readADC(3)>3800);
}
