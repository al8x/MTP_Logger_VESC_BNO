// INFORMATIONS ON vesc communication

//UART VESC
//RX(Vesc Green cable) => TX teensy Pin 1
//TX (Vesc Blue cable) => RX Teensy pin 0
//
//
//Struct of data recived by the VESC in UART
//    float tempFetFiltered;
//    float tempMotorFiltered;
//    float avgMotorCurrent;
//    float avgInputCurrent;
//    float avgId;
//    float avgIq;
//    float dutyNow;
//    long rpm;
//    float inpVoltage;
//    float ampHours;
//    float ampHoursCharged;
//    float wattHours;
//    float watthoursCharged;
//    long tachometer;
//    long tachometerAbs;
//    int faultCode;
//
//Commandes availables with UART commande
//  VescUartSetCurrent
//  VescUartSetPosition
//  VescUartSetPosition
//  VescUartSetDuty
//  VescUartSetDuty
//  VescUartSetRPM
//  VescUartSetCurrentBrake

//********************************************************************
// ------------------    FUNCTIONS   -----------------------
//********************************************************************

// ------------------    SPEED   -----------------------

double erpmToKmH (float erpm){
  //Round the number at 0.1
  double speedValue = ((DiamRoue*3.1415/1000000)*((erpm/MagnetFactor)*gearRatio)*60); // convert erpm motor in km/h of the wheel
  speedValue = (round(speedValue*10));
  speedValue = speedValue/10;
  return speedValue; 
}

double kmhToRpmWheel (double kmh){
  return (kmh*1000000)/(60*DiamRoue*3.1415);
}

double kmhToErpmMotor (double kmh){
  return kmhToRpmWheel(kmh)*MagnetFactor/gearRatio; // Convert speed in RPM of the wheel and ajusted with the magnet factor of the motor and the gear ratio
}

// ------------------    BATTERY   -----------------------

float InitialBatteryLevelCalculation (float BatteryVoltage){ // Return the value in WattHours of the theoritical value left
  float percentageLeft = map(BatteryVoltage,MinBatteryVoltage,MaxBatteryVoltage,0,100);
  return (percentageLeft*BatteryCapacity/100);
}

int BatteryLeft (float wattHoursConsumed){ // return a value between 0 and 100
  float wattHoursLeft = InitialwattHoursLeft-(wattHoursConsumed);
  return map(wattHoursLeft,0,BatteryCapacity,0,100);
}

//********************************************************************
// ------------------    DISTANCE   -----------------------

void NewWhValueToSave(float WhConsumtionValue, float Array[], int ArraySize){ // Funtion that push all Wh value to the right and Add the last value
  for (int i=(ArraySize-1); i>=1 ; i--){  // Push all value 1 step to the right
    Array[i]=Array[i-1];
  }
  Array[0] = WhConsumtionValue;     // In the [0] cell, Put the most recent Wh hour value
}


float AverageWhPerKmConsumtion(float TabOfWhValues[], int ArraySize){        //Calculate the Average Wh/km consumtion 
  float WhattHours = TabOfWhValues[0] - TabOfWhValues[ArraySize-1];
  return (WhattHours/(float(DISTANCE_AVERAGE_CALCULATION)/1000));   // Convert the measure distance for meter to Km
}


float DistanceLeft (float wattHoursConsumed, float AverageWattHoursPerKm){ // Return a Value in Km, estimated on the Avergage consumtion and Battery Left
  float wattHoursLeft = InitialwattHoursLeft-(wattHoursConsumed);
  float local_distanceLeft;
  
  if ((float(DISTANCE_AVERAGE_CALCULATION)/1000)>Global_DistanceTraveled){  // If the miniumum calcutaion value is above the current traveled distance
    local_distanceLeft = wattHoursLeft/Global_AverageWhPerKmdefaultValue;
  }
  else{
      local_distanceLeft = wattHoursLeft/AverageWattHoursPerKm;
  }
  
  return (local_distanceLeft); // convert rpmin km/h
}

float TachymeterToMeters(float tachymeterValue){ // Return the value in m
  return(tachymeterValue*gearRatio*DiamRoue*3.14159/(TachometerConvertionRation*1000));
}

//********************************************************************
// ------------------   GLOBAL VARIABLES REFRESH   -----------------------
//********************************************************************

void ConsumtionStatusRefresh(){ //bldcMeasure& values

  int distanceSinceLastCheck = TachymeterToMeters(measuredVal.tachometerAbs-lastTachyValue); //Distance in Meter
  Global_DistanceTraveled = (TachymeterToMeters(measuredVal.tachometerAbs))/1000 ; // In KM
  
  if ((distanceSinceLastCheck/DISTANCE_STEP_CALCULATION)>0){  // If it is time to refresh the average consumtion value  
    NewWhValueToSave(measuredVal.wattHours,WhArrayValue,SizeOfWhArray);    // Add the current Watt hours value to the array
    Global_WhPerKm_Average=AverageWhPerKmConsumtion(WhArrayValue,SizeOfWhArray);  // Refresh the Watt hour value of the current ride
    lastTachyValue = measuredVal.tachometerAbs; //Save current distance for next check
  }
    
    Global_DistanceLeft = DistanceLeft(measuredVal.wattHours,Global_WhPerKm_Average) ;  //Refresh the distance left in Km
    Global_BatteryPercentageLeft = BatteryLeft (measuredVal.wattHours); // in Percentage between 0 and 100
      
}


