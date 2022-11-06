//  VESC LOGGING FUNCTION
// ---- Use VescLogData() in the programme

void VescLogData(){
  
  slowFindName();
  SdFile::dateTimeCallback(dateTime);

  
  if (!file.open(fileName, O_CREAT | O_WRITE | O_EXCL)) { // We attemp to open the file
    error("file.open");
    DEBUGSERIAL.println(" Slow file.open error");
  }
  
  else{ 
    
     Aquisition("VESC LOGGING",fileName); // Diplsay the screen of aquisition data
     VescsdHeaderPrint();
     
     while (!rightButtonPressed){
        VescUartGetValue(measuredVal);
        SavingVescDataSd(measuredVal);
        
        if (!file.sync() || file.getWriteError()) {
          error("write error");
          DEBUGSERIAL.println(" Slow write error error");
        }
     }
     file.close();
     ResetButton();
     SlowSavingStats(fileName);
     
     while (!rightButtonPressed){
      delay(10);
     }
     ResetButton();
  }
}

void VescsdHeaderPrint(){
  // Print header in text file
    file.print("Time");
    
    file.print(";");
    file.print("tempFetFiltered");

    file.print(";");
    file.print("tempMotorFiltered");

    file.print(";");
    file.print("avgMotorCurrent");

    file.print(";");
    file.print("avgInputCurrent");
    
    file.print(";");
    file.print("avgId");
    
    file.print(";");
    file.print("avgIq");
    
    file.print(";");
    file.print("dutyNow");
    
    file.print(";");
    file.print("rpm");
    
    file.print(";");
    file.print("inpVoltage");
    
    file.print(";");
    file.print("ampHours");
    
    file.print(";");
    file.print("ampHoursCharged");
    
    file.print(";");
    file.print("tachometer");
    
    file.print(";");
    file.print("tachometerAbs"); 

    file.print(";");
    file.print("Analog Input 1");

    file.print(";");
    file.print("Analog Input 2"); 

    file.print(";");
    file.println("faultCode");   
}

void SavingVescDataSd(const struct bldcMeasure& data){
  
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

   // if the file is available, write to it:
    file.print(millis());
    
    file.print(";");
    file.print(data.tempFetFiltered);

    file.print(";");
    file.print(data.tempMotorFiltered);

    file.print(";");
    file.print(data.avgMotorCurrent);

    file.print(";");
    file.print(data.avgInputCurrent);
    
    file.print(";");
    file.print(data.avgId);
    
    file.print(";");
    file.print(data.avgIq);
    
    file.print(";");
    file.print(data.dutyNow);
    
    file.print(";");
    file.print(data.rpm);
    
    file.print(";");
    file.print(data.inpVoltage);
    
    file.print(";");
    file.print(data.ampHours);
    
    file.print(";");
    file.print(data.ampHoursCharged);
    
    file.print(";");
    file.print(data.tachometer);
    
    file.print(";");
    file.print(data.tachometerAbs);

    file.print(";");
    file.print(analogRead(ANALOG_ONE_PIN));

    file.print(";");
    file.print(analogRead(ANALOG_TWO_PIN));

    file.print(";");
    file.println(data.faultCode);
    
   }
