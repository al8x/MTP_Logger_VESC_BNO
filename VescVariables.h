//********************************************************************
// ------------------    VARIABLES  HARDWARE  BEHAVIOR  -----------------------
//********************************************************************

// Brake PIN
const int pinBrake = 4; //Can only be 4 or 5 for digital and A6 or A7 for analog
const int analogPinBrake = A7;  //Can only A6 or A7 for analog

const int minValueBrakeStart = 300;
const int maxValueBrake = 800;


// Motor
const int DiamRoue = 160; //roue arriere 200 mm
const float gearRatio = 1 ; // Equal 1 for HubMotot, gear ration for belt drive

const int MagnetFactor = 10; // Factor of the motor magnets for Erpm to speed convertion
                            // 7 pole on the motor for 140 KV belt motor proto =
                            // 10 for the 150 mm rear hub motor 

const int TachometerConvertionRation = 42; // 90 step per wheel rotation for PRoto monstre grosse roue
                                           // 42 step motor rotation for 140 KV belt motor proto

const int MaxSpeedPossible = 30 ; // Max speed in Km/h


//  -- Battery
const int MaxBatteryVoltage = 42;
const int MinBatteryVoltage = 34;
const int MaxCurrentPossible = 12; // Amp max possible
const int BatteryCapacity = 90 ; // 100% battery equal to 90 Watthours
const int MaxDistancePossible = 9; // Distance max possible avec la batterie pleine

int InitialwattHoursLeft = BatteryCapacity;

//  -- Whatt Hours consumtion / Distance left calculation

#define DISTANCE_AVERAGE_CALCULATION 100 // Distance in m base for average Wh consumtion
#define DISTANCE_STEP_CALCULATION 10  // Distance in m for average Wh consumtion refresh calculation

const int SizeOfWhArray = DISTANCE_AVERAGE_CALCULATION/DISTANCE_STEP_CALCULATION;
float WhArrayValue[SizeOfWhArray]={0};

int lastTachyValue=0;  // Is used to calculate the refreshement of the value
const float Global_AverageWhPerKmdefaultValue = 11 ; // WH/KM to calculate distance left

// -- GLOBAL VARIABLES DECLARE

float Global_WhPerKm_Average;
float Global_DistanceLeft;  // in Km
int Global_BatteryPercentageLeft; // in Percentage between 0 and 100
float Global_DistanceTraveled ; // In KM

// -- Trigger

int calibrationZeroTrigger;
int calibrationMaxTrigger = 820;

//********************************************************************
// ------------------    VARIABLES  ASSITANCE BEHAVIOR  -----------------------
//********************************************************************

const float speedMiniAssist = 3; // km/h  under this value, no order to vesc
const float speedGapFactor = 1.5; // km/h if measured speed 10percent higher = acceleration
const float speedGapIntervalAssit = 0.5; // km/h if measured speed 10percent higher = acceleration

//    --Braking

const int durationRampBraking = 800 ; //en ms, temps pour ateindre le freinage max
const int currentDeceleratMax = 20 ; // En A, courant de deceleration max
int timeStartBraking;
float currentDecelerat;

//    -- Boost

const int durationBoost= 2000; //en ms duree du boost
const int durationBetweenBoost = 10000 ; //en ms entre boost
int lastBoost;
int timeStartBoost;


// Mode 0 = ModeSport , Mode 1 = Mode Eco , Mode 2 = Walk mode
const int speedMaxMode[3] = {22,18,30};
int modeSelector =0;


