void actionVescMode (int modeSelected){
  switch (modeSelected) {
    case 0:
      //VescScreen();    //VESC Mode, can controle the Ride in certain Mode
      VescScreenDebug();
      break;
    case 1:
        VescScreenDebug2();
        //rideControlAssistPID();
        //rideControlAssistAnalogBrake();
      //rideControlAssist();
      break;
    case 2:
       VescScreenDebug3();
      //rideControlReomte();
      break;
    case 3:
       VescScreenDebug4();
      //rideControlReomte();
      break;
  }
}


void modeSelectorVesc(){
  //display the settings screen and allowed the user to change between VESC MODE
  // on 0 display only
  // on 1 assistance
  // on 2 ?

  // Make use of those variables
  // vescMode = 0;
  // nbTotalVescMode = 2;

  int timeOutFunction = 5000 ; //ms
  int avancementLigne =0;
  int animationStart = 3000;
  ResetButton();
  
  while (!((leftButtonPressed)||((millis() - lastButtonAction)>(timeOutFunction)))){  
    
    if (rightButtonPressed){
      vescMode=vescMode+1;
      vescMode=vescMode%nbTotalVescMode;
      ResetButton();
    }

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println("");
    display.setTextSize(1);

    if (vescMode==0){
      display.setTextColor(BLACK);
      display.fillRect(0, 4, 128, 16, WHITE);
    }
    
    display.println("  Mode Display Only ");
    display.println("");
    display.setTextColor(WHITE);
    
    if (vescMode==1){
      display.setTextColor(BLACK);
      display.fillRect(0, 20, 128, 16, WHITE);
    }
 
    display.println("  Mode ASSISTANCE ");
    display.println("");
    display.setTextColor(WHITE);
    
    if (vescMode==2){
      display.setTextColor(BLACK);
      display.fillRect(0, 36, 128, 16, WHITE);
    }
    display.println("  Mode REMOTE ");
    display.println("");

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,55);
    display.print(" Select    *    Next ");
   
    display.setCursor(0,0);
    display.drawLine(0, 50, 128, 50, WHITE);
    display.display();
    display.clearDisplay();
   
  }
}

void VescScreen(){
  // Screen data and help to calibrate the sensors
  VescUartGetValue(measuredVal);
  display.setTextColor(WHITE);
  display.setTextSize(4);
  display.setCursor(5,5);
  
  if (vescConnection){
    display.print(round(erpmToKmH(measuredVal.rpm)));
  }
  else{
    display.print("X");
  }

  //Serial.println(measuredVal.tachometer);
  
  display.setTextSize(1);
  display.setCursor(10,40);
  display.print(" Km/h");

  display.setTextSize(2); 
  display.setCursor(60,5);
  display.print(measuredVal.avgInputCurrent);
  //display.print("22.22");
  display.setTextSize(1);
  display.setCursor(120,12);
  display.print("A");

  display.setTextSize(2); 
  display.setCursor(60,30);
  display.print(round(BatteryLeft(measuredVal.wattHours)));
  display.print("%");
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,55);
  if (vescConnection){
    display.print("Mode  ----o-  VESCLog");
  }
  else{
    display.print("Mode  ----o-     *   ");
  }
  
 
  display.setCursor(0,0);
  display.drawLine(0, 50, 128, 50, WHITE);
  display.display();
  display.clearDisplay();
}

void VescScreenAssist(){
  // Screen data and help to calibrate the sensors
  VescUartGetValue(measuredVal);
  display.setTextColor(WHITE);
  display.setTextSize(4);
  display.setCursor(5,5);
  display.print(round(erpmToKmH(measuredVal.rpm)));
  
  //Serial.println(measuredVal.tachometer);
  
  display.setTextSize(1);
  display.setCursor(10,40);
  display.print(" Km/h");

  display.setTextSize(2); 
  display.setCursor(60,5);
  display.print(measuredVal.avgInputCurrent);
  //display.print("22.22");
  display.setTextSize(1);
  display.setCursor(120,12);
  display.print("A");

  display.setTextSize(2); 
  display.setCursor(60,30);
  display.print(round(BatteryLeft(measuredVal.wattHours)));
  display.print("%");
  
  if ((millis()%1000)>500){ // Animate the text and colors every half second
    display.fillRect(110, 28, 17, 18, WHITE);
    display.setTextColor(BLACK); 
  }
  
  display.setCursor(113,30);
  display.setTextSize(2);
   display.print("A");
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,55);
  display.print("Mode  ----o-      *  ");
 
  display.setCursor(0,0);
  display.drawLine(0, 50, 128, 50, WHITE);
  display.display();
  display.clearDisplay();
  
}

void VescScreenRemote(){
  // Screen data and help to calibrate the sensors
  VescUartGetValue(measuredVal);
  display.setTextColor(WHITE);
  display.setTextSize(4);
  display.setCursor(5,5);
  display.print(round(erpmToKmH(measuredVal.rpm)));
  
  display.setTextSize(1);
  display.setCursor(10,40);
  display.print("Km/h");

  // Remote graph
  int sizeRemote = map(pwm_value,1080,2170,0,45);
  display.fillRect(52, 47-sizeRemote, 5, sizeRemote, WHITE);

  display.setTextSize(2); 
  display.setCursor(60,5);
  display.print(measuredVal.avgInputCurrent);
  //display.print("22.22");
  display.setTextSize(1);
  display.setCursor(120,12);
  display.print("A");

  display.setTextSize(2); 
  display.setCursor(60,30);
  display.print(round(BatteryLeft(measuredVal.wattHours)));
  display.print("%");
  
  if ((millis()%1000)>500){ // Animate the text and colors every half second
    display.fillRect(110, 28, 17, 18, WHITE);
    display.setTextColor(BLACK); 
  }
  
  display.setCursor(113,30);
  display.setTextSize(2);
   display.print("R");
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,55);
  display.print("Mode  ----o-      *  ");
 
  display.setCursor(0,0);
  display.drawLine(0, 50, 128, 50, WHITE);
  display.display();
  display.clearDisplay();
  
}

void VescScreenDebug(){
  // Screen data and help to calibrate the sensors
  // Screen Speed, power (watts) , distance left , Batterie % and distance traveled
  
  VescUartGetValue(measuredVal);
  ConsumtionStatusRefresh();

  display.setTextColor(WHITE);  
  display.setTextSize(4);
  display.setCursor(5,10);
  display.print(round(erpmToKmH(measuredVal.rpm)));
  display.setTextSize(1);
//  display.setCursor(10,40);
//  display.print(" Km/h");

   //POWER
  display.setTextSize(2); 
  display.setCursor(60,2);
  display.print(int(measuredVal.avgInputCurrent*measuredVal.inpVoltage));
  display.setCursor(60,18);
  display.print((measuredVal.avgInputCurrent));
  display.setCursor(60,32);
  display.print((measuredVal.avgMotorCurrent));
  display.setTextSize(1);
  display.setCursor(110,5);
  display.print("W");
  display.setCursor(110,22);
  display.print("Ab");
  display.setCursor(110,35);
  display.print("Am");

  display.setTextColor(WHITE);

//  display.print(Global_BatteryPercentageLeft);
//  display.print("%");

  display.setTextColor(WHITE);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,55);
  if (carteSdIn&&vescConnection){
    display.print("/     ----o-    Log ");
  }
  else{
    display.print("/     ----o-      *  ");
  }
  
 
  display.setCursor(0,0);
  display.drawLine(0, 50, 128, 50, WHITE);
  display.display();
  display.clearDisplay();
  
}

void VescScreenDebug2(){
  // Screen data and help to calibrate the sensors
  // Screen Speed, power (watts) , distance left , Batterie % and distance traveled
  
  VescUartGetValue(measuredVal);
  ConsumtionStatusRefresh();

   //Distance traveled, Avg consumtion , Voltage
  display.setTextSize(2); 
  display.setCursor(10,2);
  display.print(Global_DistanceTraveled);
  
  display.setCursor(10,18);
  display.print(Global_WhPerKm_Average);

  display.setCursor(10,33);
  display.print((measuredVal.inpVoltage));
  display.setTextSize(1);
  display.setCursor(90,5);
  display.print(" Km ");
  display.setCursor(90,22);
  display.print(" Wh/Km ");
  display.setCursor(90,36);
  display.print(" V");
 
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,55);
  if (carteSdIn&&vescConnection){
    display.print("//    ----o-    Log ");
  }
  else{
    display.print("//    ----o-      *  ");
  }
  
 
  display.setCursor(0,0);
  display.drawLine(0, 50, 128, 50, WHITE);
  display.display();
  display.clearDisplay();
  
}

void VescScreenDebug3(){
  // Screen data and help to calibrate the sensors
  // Screen Speed, power (watts) , distance left , Batterie % and distance traveled
  
  VescUartGetValue(measuredVal);
  ConsumtionStatusRefresh();

   //Tachometer, Rpm , duty
  display.setTextSize(2); 
  display.setCursor(10,2);
  display.print(measuredVal.tachometer);
  
  display.setCursor(10,18);
  display.print(measuredVal.rpm);

  display.setCursor(10,33);
  display.print((measuredVal.dutyNow));
  display.setTextSize(1);
  display.setCursor(90,5);
  display.print(" Tach ");
  display.setCursor(90,22);
  display.print(" RPM ");
  display.setCursor(90,36);
  display.print(" /1");
 
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,55);
  if (carteSdIn&&vescConnection){
    display.print("///   ----o-    Log ");
  }
  else{
    display.print("///   ----o-      *  ");
  }
  
 
  display.setCursor(0,0);
  display.drawLine(0, 50, 128, 50, WHITE);
  display.display();
  display.clearDisplay();
  
}

void VescScreenDebug4(){
  // Screen data and help to calibrate the sensors
  // GRAPH SCREEN
  
  VescUartGetValue(measuredVal);
  ConsumtionStatusRefresh();
 
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,55);

  display.print("////  ----o-   ");

  switch (Vesc_GraphSelector) {
    case 0:
      display.print(" RPM");
      WideGraph(measuredVal.rpm,10,0,0,48);
      break;
    case 1:
      display.print("Bat-A");
      WideGraph(measuredVal.avgInputCurrent,10,0,0,48);
      break;
    case 2:
      display.print("Mot-A");
      WideGraph(measuredVal.avgMotorCurrent,10,0,0,48);
      break;
    case 3:
      display.print(" ADC");
      WideGraph(readADC(3),10,0,0,48);
      break;
  }

  display.setCursor(0,0);
  display.drawLine(0, 50, 128, 50, WHITE);
  display.display();
  display.clearDisplay();
  
}
