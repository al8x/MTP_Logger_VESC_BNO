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
    LOG_INTERVAL_USEC = LOG_INTERVAL_USEC_TABLE[log_interval_selector];
    
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
}

void setDiameter(){
  
  while (!middleButtonPressed){
    delay(10);
    ScreenChooseDiameter(); //Interface on the screen
    
    if (leftButtonPressed){
      leftButtonPressed=false; // Button reinitilastion
      
      //We set the minimum 
      if (RangDiametre>0){ // If the minimum is not reached
        RangDiametre=RangDiametre-1;
      }
    }
    
    else if (rightButtonPressed){
      rightButtonPressed=false; // Button reinitilastion
      
      if (RangDiametre<(NbDeDiametre-1)){ // If the maximum is not reached
        RangDiametre=RangDiametre+1;
      }
    }
  }
  // at the end, we can leave with the modified rank
  middleButtonPressed=false;
  NbrePtsMesure = (DistanceDeMesure*nbAimentSurRoue)/(nbPassageAimentParAquisition*(ListeDeDiametrePossible[RangDiametre]/(float)1000)*3.14159); 

}

void setNbMagnetOnWheel(){

  while (!middleButtonPressed){
    delay(10);
    ScreenChooseNbMagnet(); //Interface on the screen
    
    if (leftButtonPressed){
      leftButtonPressed=false; // Button reinitilastion
      
      //We set the minimum 
      if (nbAimentSurRoue>1){ // If the minimum is not reached
        nbAimentSurRoue=nbAimentSurRoue-1;
      }
    }
    
    else if (rightButtonPressed){
      rightButtonPressed=false; // Button reinitilastion
       nbAimentSurRoue=nbAimentSurRoue+1;
      
    }
  }
  // at the end, we can leave with the modified rank
  middleButtonPressed=false;
}
