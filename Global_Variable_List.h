// Avoid IDE problems by defining struct in septate .h file.
// Pad record so size is a power of two for best write performance.

#ifndef Global_Variable_List_h
#define Global_Variable_List_h

//-----------------------------------------------
//-------------------------- INTERFACE Definition 
//-----------------------------------------------

int quelScreen = 0; // valeur quin permet de choisir quel écran est affiché
#define nbTotalScreen   6 // Number of main interface we have

int  SensorsMode= 0;
#define  nbSensorsMode  4

int vescMode = 0;
#define nbTotalVescMode 3

int IMU_ScreenSelector=0;
#define IMU_NbMode      7

int Vesc_ScreenSelector=0;
#define Vesc_NbMode     4
int Vesc_GraphSelector=0;
#define Vesc_NbGraph    4

int AnalogMode = 0;
#define nbAnalogMode    2
int FFT_GraphSelector = 0;
#define FFT_GraphNb     5

//-----------------------------------------------
//-------------------------- APPLICATION DEFINITION 
//-----------------------------------------------

int logging_application;

#define EXTERNAL_MCP_SPEED  0
#define ANALOG_ACCELERO     1
#define THERMOCOUPLE        2

//--------------------------------------------------------
// --------------------- GLOBAL APPLICATION VARAIBLE LISTE
//--------------------------------------------------------

//---------------------------------------
// ** 1 - SD card fast reader
//---------------------------------------

bool usbWasConnected;

unsigned long int millisS_logStart;
unsigned long int microS_logStart;

struct data_t {
  float time_s;
  unsigned int mcp_0; // On ne lit que 2 valeurs d'adc
  unsigned int mcp_1; // On ne lit que 2 valeurs d'adc
  unsigned int mcp_2; // On ne lit que 2 valeurs d'adc
  unsigned int mcp_3; // On ne lit que 2 valeurs d'adc
  
  float Speed; // The speed is calculated thanks to the hall effect sensor
  unsigned short hallSensorOne;
  unsigned short hallSensorTwo;
  float temperature;

  unsigned int adc_1; // On ne lit que 2 valeurs d'adc
  unsigned int adc_2; // On ne lit que 2 valeurs d'adc
  unsigned int adc_3; // On ne lit que 2 valeurs d'adc
  
  int overrun_in;
};

//---------------------------------------
// ** 2 - SD card fast reader
//---------------------------------------

struct imu_t {
  float time_s;
  unsigned int mcp_0; // On ne lit que 2 valeurs d'adc
  unsigned int mcp_1; // On ne lit que 2 valeurs d'adc
  unsigned int mcp_2; // On ne lit que 2 valeurs d'adc
  unsigned int mcp_3; // On ne lit que 2 valeurs d'adc
  
  float Speed; // The speed is calculated thanks to the hall effect sensor
  unsigned short hallSensorOne;
  unsigned short hallSensorTwo;
  float temperature;

  unsigned int adc_1; // On ne lit que 2 valeurs d'adc
  unsigned int adc_2; // On ne lit que 2 valeurs d'adc
  unsigned int adc_3; // On ne lit que 2 valeurs d'adc
  
  int overrun_in;
};

//---------------------------------------
// ** 3 - WHEEL EFFICENCY + SPEED CHECK
//---------------------------------------

long int currentMagnetTime;
long int previousMagnetTime; //Save the time of the previous magnet in order to save and show the speed
float currentSpeed = 0; // the speed is refreshed each time the sensor see the magnet

#define NbDeDiametre 16 //CE nombre doit matcher la taille du tableau ci dessoius
int ListeDeDiametrePossible[NbDeDiametre] ={54,58,66,70,76,80,110,120,150,175,180,200,203,208,216,250};

int RangDiametre = 9 ; //Set the the diameter o 80 mm. The RangDiametre value can be changed in the setting to adapt the value according to the product we are testing

int nbAimentSurRoue= 1; // We assume that magnet on the wheel are equally spaced

int indX; //indX ecriture dans stockage
//int Stockage[NbrePtsMesure+1]; //= Nombre de points de mesure+1
int Stockage[1000]; //= Nombre de points de mesure+1

bool wheelEffMesure;
bool Fini;
int vitesseMiniFranchie = false; // Variable pour la detection que on part bien au dessus de la vitesse de debut daquisition

bool speedKnown = false; //
int NombrePassageAimant; // Valeur qui ne doit pas être changé, initialise le nombre de passage d'aiment observé

int DataAlreadySaved = false;

// -----------FOR WHEEL EFFICIENCY

int DistanceDeMesure = 45; // en mètre
int NumeroDeSAmple =1; // permet d'identifier chaque échantillions dans le programme wheel efficiency
const int nbPassageAimentParAquisition= 1; // Un nombre supérieur à 1 permet d'augmnenter la distance en diminuant le nombre de points
int NbrePtsMesure = (DistanceDeMesure*nbAimentSurRoue)/(nbPassageAimentParAquisition*(ListeDeDiametrePossible[RangDiametre]/(float)1000)*3.14159); 
//Nombre de points de mesure sur la courbe, doit être au minimum de 2. Au dela de 20 points, des problème de mémoire peut apparaitre avec un arduino Uno

//---------------------------------------
// ** 3 - PPM radio receiver - Remote
// Mostly used for magnet sensor for braking
//---------------------------------------

volatile int pwm_value = 0;
volatile int prev_time_pwm = 0;
int low_pwm_value = 1080;
int high_pwm_value = 2170;
int middle_pwm_value = 1630 ;

#endif  // Global_Variable_List_h
