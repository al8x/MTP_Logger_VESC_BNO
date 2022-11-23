void longPress_detect(){
  // This function modify the buttonsPressed status and should be putted in a screen setting loop
  
      // CHECK FOR A CHANGE OF STATE _ Not pressed to pressed state and start the time counter   
    if ((!digitalRead(RIGHT_BUTTON_PIN))&&(!rightButtonIsHold)){ // Button pressed and was not pressed on previous loop
        falling_edge_right=millis(); // We store the beginning of the long press
      }
      
     if ((!digitalRead(LEFT_BUTTON_PIN))&&(!leftButtonIsHold)){ // Button pressed and was not pressed on previous loop
      falling_edge_left=millis(); // We store the beginning of the long press
    }
      
    rightButtonIsHold = (!digitalRead(RIGHT_BUTTON_PIN));
    leftButtonIsHold = (!digitalRead(LEFT_BUTTON_PIN));
      
    if ((rightButtonIsHold)&&((millis()-falling_edge_right)>DEBOUNCE_LONG_PRESS)){
        rightButtonPressed = true;
    }
    if ((leftButtonIsHold)&&((millis()-falling_edge_left)>DEBOUNCE_LONG_PRESS)){
        leftButtonPressed = true;
    }
}

void generalSettings(){
  //display the settings screen and allowed the user to change
  // on 0 : the frequency
  // on 1 the diameter of the wheel
  // on 2 the number of magnet on the wheel
  int numberOfchoicesAvailable = 3;
  int highlightedChoice = 0;
  
  while (!leftButtonPressed){
    delay(1);
    screenGeneralSetting(highlightedChoice); //Interface on the screen
    
    if (middleButtonPressed){ // execute the choice
      middleButtonPressed=false;
      switch (highlightedChoice) {
        case 0:{
          setFrequency();
        }
        break;
        case 1:{
          setDiameter();
      }
        break;
        case 2:{
          setNbMagnetOnWheel();
      }
         break;
      }
    }
    
    else if (rightButtonPressed){
      highlightedChoice+=1;
      highlightedChoice=highlightedChoice%numberOfchoicesAvailable; // We keep the range
      rightButtonPressed=false;
      }
   
  }
  // at the end, we set the final frequency by changing the value
  leftButtonPressed=false;
}

void setFrequency(){
  
  while (!middleButtonPressed){
    delay(10);
      ScreenChooseFrequency(); //Interface on the screen   
    
    
    if (leftButtonPressed){
      
      log_interval_selector += 1; 
      leftButtonPressed=false;
      //We set the minimum 
      log_interval_selector=log_interval_selector%LOG_INTERVAL_VAL;


    }
    
    else if (rightButtonPressed){
      log_interval_selector -= 1; 
      rightButtonPressed=false;
      
      if (log_interval_selector<0){
        log_interval_selector=0;
      }
    }
  }
  // at the end, we set the final frequency by changing the value
  middleButtonPressed=false;
  EEPROM.update(ADRESS_FREQUENCY, log_interval_selector); // We store the datat for a futur use
}

void setDiameter(){

  rightButtonIsHold= false; // reset long press state
  leftButtonIsHold = false; // reset long press state
  
  while (!middleButtonPressed){
    ScreenChooseDiameter(); //Interface on the screen

    longPress_detect(); // function that modify the state of right and left buttonPressed in accordance to the button read

    if (leftButtonPressed){
      leftButtonPressed=false; // Button reinitilastion
      
      //We set the minimum 
      if (wheel_diameter>1){ // If the minimum is not reached
        wheel_diameter=wheel_diameter-1;
      }
    }
    
    else if (rightButtonPressed){
      rightButtonPressed=false; // Button reinitilastion
      
      if (wheel_diameter<1000){ // If the maximum is not reached
        wheel_diameter=wheel_diameter+1;
      }
    }
  }
  // at the end, we can leave with the modified rank
  middleButtonPressed=false;
  EEPROM.update(ADRESS_WHEEL_DIAM, wheel_diameter); // We store the datat for a futur use
  NbrePtsMesure = (DistanceDeMesure*nbAimentSurRoue)/(nbPassageAimentParAquisition*(wheel_diameter/(float)1000)*3.14159); 

}

void setNbMagnetOnWheel(){

  rightButtonIsHold= false; // reset long press state
  leftButtonIsHold = false; // reset long press state
  
  while (!middleButtonPressed){
    
    ScreenChooseNbMagnet(); //Interface on the screen

    longPress_detect(); // function that modify the state of right and left buttonPressed in accordance to the button read
    
    if (leftButtonPressed){  // Short press LEFT
      leftButtonPressed=false; // Button reinitilastion
      
      //We set the minimum 
      if (nbAimentSurRoue>1){ // If the minimum is not reached
        nbAimentSurRoue=nbAimentSurRoue-1;
      }
    }
    
    else if (rightButtonPressed){ // SHORT PRESS RIGHT
      rightButtonPressed=false; // Button reinitilastion
      nbAimentSurRoue=nbAimentSurRoue+1;
   
    }
  }
  
  // at the end, we can leave with the modified rank
  middleButtonPressed=false;
  EEPROM.update(ADRESS_MAGNET_NB, nbAimentSurRoue); // We store the datat for a futur use
}
