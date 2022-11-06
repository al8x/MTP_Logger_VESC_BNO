//Commandes availables with UART commande
//  VescUartSetCurrent
//  VescUartSetPosition
//  VescUartSetPosition
//  VescUartSetDuty
//  VescUartSetDuty
//  VescUartSetRPM
//  VescUartSetCurrentBrake

#include <PID_v1.h>

 //*************************************************************************** 
 // ---------------------------  Mode Scooter Assistance with Mechanical brake
 //***************************************************************************

void rideControlAssist(){

  // Press Right To exit the mode

  delay(100);
  ResetButton();
  
  float speedTarget = 0;
  float currentSpeedVesc; 

    // ----------------------------- ASSIST MODE LOOP and SAVING
    
    
  while (!(leftButtonPressed||middleButtonPressed)){ // Exit the assistance on left or middle pressed button the function does not reset the  buttons


    // - 0 Check mode selector
//    if (leftButtonPressed){
//      modeSelector=(modeSelector+1)%3;
//      ResetButton();
//      speedTarget=0;
//    }
        
    // - 1 Read current Speed 
    VescUartGetValue(measuredVal);
    currentSpeedVesc = erpmToKmH(measuredVal.rpm);
    
    VescScreenAssist();
    //SavingVescDataSd(measuredVal);

    // - 2 detect if acceleration  and update value if necessary
    if ((currentSpeedVesc)>(speedTarget+speedGapFactor)){  // analyse + safety coef
      VescUartGetValue(measuredVal);
      speedTarget=min(currentSpeedVesc,speedMaxMode[modeSelector]);
    }

    //3 - we check if user is braking
    if (digitalRead(pinBrake)==LOW){
      //if yes, we ramp a deceleration with current brake commande
      // We add a delay to debounce the issue on brake sensor
      delay(200);
      if (digitalRead(pinBrake)==LOW){
        timeStartBraking=millis();
      
        //  Loop when we maintain braking the wheel while the lever is triggered
        while (digitalRead(pinBrake)==LOW){
           currentDecelerat=min(currentDeceleratMax,((millis()-timeStartBraking)*currentDeceleratMax*10/(durationRampBraking*10))); // COurant de décélération est égale          
           VescUartSetCurrentBrake(currentDecelerat);
           VescScreenAssist();
           //SavingVescDataSd(measuredVal);
          }
                
        //  At the end of the braking session, we update the rpm value with the one measure after the braking session
        VescUartSetCurrentBrake(0);
        VescUartGetValue(measuredVal);
        
        //ScreenModeOne(measuredVal,modeSelector);
        //SavingVescDataSd(measuredVal);
        currentSpeedVesc = erpmToKmH(measuredVal.rpm);
        speedTarget=min(currentSpeedVesc,speedMaxMode[modeSelector]);
      }      
   }

    //4 - BOOST
//   if ((leftButtonPressed)&&(rpmTarget>rpmMiniAssist)){
//
//    if (modeSelector!=0){  // si pas mode 0
//      ScreeModeNotAvailable();
//      delay(300);
//    }
//    
//    else if ((millis()-lastBoost)<durationBetweenBoost){ // Si le drenier boost est trop recent on fait rien
//      int percentageRegeneration = ((millis()-lastBoost)*100)/durationBetweenBoost;
//      ScreenBoostAvailableIn(percentageRegeneration);
//      Serial.println(percentageRegeneration);
//      delay(300);
//    }
//
//    else{
//      
//      timeStartBoost = millis();
//      int BrakeDuringBoost = false;
//      
//      while ((!BrakeDuringBoost)&&((millis()-timeStartBoost<durationBoost))){ //Sinon on boost
//         
//         VescUartSetDuty(1);
//         VescUartGetValue(measuredVal);
//         ScreenBOOST(100*(millis()-timeStartBoost)/durationBoost); //Transmission d'une valeur entre 0 et 100 de l'avancement du boost
//         SavingVescDataSd(measuredVal);
//
//         if(analogRead(pinBrake)>1){ // We add a double check on brake to debounce the boost function
//          delay(50);
//          BrakeDuringBoost = (analogRead(pinBrake)>1); // If brake still on, we get out of the loop.
//         }
//         
////         Serial.println((millis()-timeStartBoost));
////         Serial.println((millis()-timeStartBoost)<durationBoost);
////         Serial.println((!(analogRead(pinBrake)>1))||((millis()-timeStartBoost<durationBoost)));
//       }
//
//       lastBoost = millis();
//    }
//      
//    VescUartGetValue(measuredVal);
//    ScreenModeOne(measuredVal,modeSelector);
//    SavingVescDataSd(measuredVal);
//    rpmTarget=measuredVal.rpm;
//    leftButtonPressed=false;
//    
//   }


    //4 - WE ASSIST THE MOTOR or WE ARE IN FREE WHEEL
    if (((currentSpeedVesc)<(speedTarget+speedGapIntervalAssit))&&(speedTarget>speedMiniAssist)&&(currentSpeedVesc<speedMaxMode[modeSelector])){ // only order the vesc if RPM above mini, min speed to start and current speed inferior to speed target
      // ------- MOTOR ON
      VescUartSetRPM(kmhToErpmMotor(speedTarget));
    }

      // We are in free wheel when under the rpm required to start
    else {
      VescUartSetCurrentBrake(0);
    }    
  }
   VescUartSetCurrentBrake(0); // Leave the programme realing the motor to avoid any issue
   rightButtonPressed=false;  // We 
      
}


void rideControlAssistAnalogBrake(){

  // Press Right To exit the mode

  delay(100);
  ResetButton();
  
  float speedTarget = 0;
  float currentSpeedVesc; 

    // ----------------------------- ASSIST MODE LOOP and SAVING
    
    
  while (!(leftButtonPressed||middleButtonPressed)){ // Exit the assistance on left or middle pressed button the function does not reset the  buttons


    // - 0 Check mode selector
//    if (leftButtonPressed){
//      modeSelector=(modeSelector+1)%3;
//      ResetButton();
//      speedTarget=0;
//    }
        
    // - 1 Read current Speed 
    VescUartGetValue(measuredVal);
    currentSpeedVesc = erpmToKmH(measuredVal.rpm);
    
    VescScreenAssist();
    //SavingVescDataSd(measuredVal);

    Serial.println(analogRead(analogPinBrake));

    // - 2 detect if acceleration  and update value if necessary
    if ((currentSpeedVesc)>(speedTarget+speedGapFactor)){  // analyse + safety coef
      VescUartGetValue(measuredVal);
      speedTarget=min(currentSpeedVesc,speedMaxMode[modeSelector]);
    }

    //3 - we check if user is braking
    
    if (analogRead(analogPinBrake)>minValueBrakeStart){
      //if yes, we ramp a deceleration with current brake commande
      // We add a delay to debounce the issue on brake sensor
      delay(100);
      
      if (analogRead(analogPinBrake)>minValueBrakeStart){
        timeStartBraking=millis();
        
        int AnalogbrakingValue = analogRead(analogPinBrake);
        //  Loop when we maintain braking the wheel while the lever is triggered
        while (AnalogbrakingValue>minValueBrakeStart){

          VescUartGetValue(measuredVal);
          currentSpeedVesc = erpmToKmH(measuredVal.rpm);

          if (currentSpeedVesc>3){
            currentDecelerat=map(AnalogbrakingValue,minValueBrakeStart,maxValueBrake,0,100); // COurant de décélération est égale
            currentDecelerat=currentDecelerat/100;  
            VescUartSetDuty(-currentDecelerat);  
          }

          else { // To avoid the wheel to go backward

            currentDecelerat=map(AnalogbrakingValue,minValueBrakeStart,maxValueBrake,0,(currentDeceleratMax*100)); // COurant de décélération est égale
            currentDecelerat=currentDecelerat/100;  
            VescUartSetCurrentBrake(currentDecelerat);
            
          }
                    
          
           
           VescScreenAssist();
           AnalogbrakingValue = analogRead(analogPinBrake);
           
           //SavingVescDataSd(measuredVal);
          }
                
        //  At the end of the braking session, we update the rpm value with the one measure after the braking session
        VescUartSetCurrentBrake(0);
        VescUartGetValue(measuredVal);
        
        //SavingVescDataSd(measuredVal);
        currentSpeedVesc = erpmToKmH(measuredVal.rpm);
        speedTarget=min(currentSpeedVesc,speedMaxMode[modeSelector]);
      }      
   }


    //4 - WE ASSIST THE MOTOR or WE ARE IN FREE WHEEL
    if (((currentSpeedVesc)<(speedTarget+speedGapIntervalAssit))&&(speedTarget>speedMiniAssist)&&(currentSpeedVesc<speedMaxMode[modeSelector])){ // only order the vesc if RPM above mini, min speed to start and current speed inferior to speed target
      // ------- MOTOR ON
      VescUartSetRPM(kmhToErpmMotor(speedTarget));
    }

      // We are in free wheel when under the rpm required to start
    else {
      VescUartSetCurrentBrake(0);
    }    
  }
   VescUartSetCurrentBrake(0); // Leave the programme realing the motor to avoid any issue
   rightButtonPressed=false;  // We 
      
}


void rideControlAssistPID(){

  // Press Right To exit the mode

  delay(100);
  ResetButton();

  //------------------------------------------------------------------------------
//---------------------------------------------------------- PIDDDD
//------------------------------------------------------------------------------


//Define Variables we'll be connecting to
  double speedTargetPID, currentSpeedVescPID, DutyPID;
  
  //Specify the links and initial tuning parameters
  double Kp=2, Kd=0.01, Ki=0.01;
  
  //PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
  PID myPID(&currentSpeedVescPID, &DutyPID ,&speedTargetPID , Kp, Ki, Kd, DIRECT);
  
  myPID.SetOutputLimits(0, 100); // We multiply the have the value in mah
  
  //turn the PID on
  myPID.SetMode(AUTOMATIC);

  
    
  speedTargetPID = 0;

    // ----------------------------- ASSIST MODE LOOP and SAVING
    
    
  while (!(leftButtonPressed||middleButtonPressed)){ // Exit the assistance on left or middle pressed button the function does not reset the  buttons


    // - 0 Check mode selector
//    if (leftButtonPressed){
//      modeSelector=(modeSelector+1)%3;
//      ResetButton();
//      speedTarget=0;
//    }
        
    // - 1 Read current Speed 
    VescUartGetValue(measuredVal);
    currentSpeedVescPID = erpmToKmH(measuredVal.rpm);
    myPID.Compute();
    
    Serial.println(DutyPID);
    Serial.println(currentSpeedVescPID);
    VescScreenAssist();
    
    //SavingVescDataSd(measuredVal);

    // - 2 detect if acceleration  and update value if necessary
    if ((currentSpeedVescPID)>(speedTargetPID+speedGapFactor)){  // analyse + safety coef
      VescUartGetValue(measuredVal);
      speedTargetPID=min(currentSpeedVescPID,speedMaxMode[modeSelector]);
    }

    //3 - we check if user is braking
    if (digitalRead(pinBrake)==LOW){
      //if yes, we ramp a deceleration with current brake commande
      // We add a delay to debounce the issue on brake sensor
      delay(200);
      if (digitalRead(pinBrake)==LOW){
        timeStartBraking=millis();
      
        //  Loop when we maintain braking the wheel while the lever is triggered
        while (digitalRead(pinBrake)==LOW){
           currentDecelerat=min(currentDeceleratMax,((millis()-timeStartBraking)*currentDeceleratMax*10/(durationRampBraking*10))); // COurant de décélération est égale          
           VescUartSetCurrentBrake(currentDecelerat);
           VescScreenAssist();
           //SavingVescDataSd(measuredVal);
          }
                
        //  At the end of the braking session, we update the rpm value with the one measure after the braking session
        VescUartSetCurrentBrake(0);
        VescUartGetValue(measuredVal);
        
        //ScreenModeOne(measuredVal,modeSelector);
        //SavingVescDataSd(measuredVal);
        currentSpeedVescPID = erpmToKmH(measuredVal.rpm);
        speedTargetPID=min(currentSpeedVescPID,speedMaxMode[modeSelector]);
      }      
   }


    //4 - WE ASSIST THE MOTOR or WE ARE IN FREE WHEEL
    if (((currentSpeedVescPID)<(speedTargetPID+speedGapFactor))&&(speedTargetPID>speedMiniAssist)&&(currentSpeedVescPID<speedMaxMode[modeSelector])){ // only order the vesc if RPM above mini, min speed to start and current speed inferior to speed target
      // ------- MOTOR ON
      //VescUartSetRPM(kmhToErpmMotor(speedTarget));
      
      myPID.Compute();
      DutyPID=DutyPID/100; //Conversion back to Amps
      Serial.println(DutyPID);
       //VescUartSetRPM(kmhToErpmMotor(speedTargetPID));
      VescUartSetDuty(DutyPID);
    }

      // We are in free wheel when under the rpm required to start
    else {
      VescUartSetCurrentBrake(0);
      Serial.println("FreeWheel");
    }
        
  }
   VescUartSetCurrentBrake(0); // Leave the programme realing the motor to avoid any issue
   rightButtonPressed=false;  // We 
      
}
  

  //*******************************************************
  // ----------------------------- END OF     ASSIST MODE LOOP
  //*******************************************************


void rideControlReomte(){
  // Press Right To exit the mode

  delay(100);
  ResetButton();

   int currentPwm;
   int safetyGap =40;
   float dutyCycle=0;
   float brakingCurrent=0;

   //use currentDeceleratMax

    // ----------------------------- ASSIST MODE LOOP and SAVING
    
    
  while (!(leftButtonPressed||middleButtonPressed)){ // Exit the assistance on left or middle pressed button the function does not reset the  buttons
    currentPwm=pwm_value;
    VescScreenRemote();

    if ((currentPwm>(middle_pwm_value+safetyGap))&&(currentPwm<(high_pwm_value+safetyGap))){ // Acceleration control 
      
      dutyCycle = map(currentPwm,middle_pwm_value+safetyGap,high_pwm_value,0,100);
      dutyCycle=dutyCycle/100;
      VescUartSetDuty(dutyCycle);
    }
    
    else if ((currentPwm<(middle_pwm_value-safetyGap))&&(currentPwm>(low_pwm_value-safetyGap))){ // Between
       
      brakingCurrent = map(middle_pwm_value-currentPwm,0,middle_pwm_value-safetyGap-low_pwm_value,0,(currentDeceleratMax*10)); //Remape the current brake
      brakingCurrent=brakingCurrent/10;
      VescUartSetCurrentBrake(brakingCurrent);
    }
    
    else{
      VescUartSetCurrentBrake(0);
    }
    
  }
  
  VescUartSetCurrentBrake(0); // Leave the programme realing the motor to avoid any issue
  rightButtonPressed=false;  // We 
  
}

