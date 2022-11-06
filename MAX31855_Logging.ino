void thermocoupleLogData(){
  
  slowFindName();
  SdFile::dateTimeCallback(dateTime);

  if (!file.open(fileName, O_CREAT | O_WRITE | O_EXCL)) { // We attemp to open the file
    error("file.open");
    DEBUGSERIAL.println(" Slow file.open error");
  }
  
  else{ 

    long  startTime = millis();
    long  lastSavingTime = millis();
    float delayBetweenSave = 200 ; // in ms
    
//     Aquisition("Thermo Logging",fileName); // Diplsay the screen of aquisition data
     thermocoupleHeader();
     
     while (!rightButtonPressed){
          
        if ((millis()-lastSavingTime)>delayBetweenSave){

          int status = thermocouple.read();          
          float tempTemp = thermocouple.getTemperature();
          
          file.print(millis()-startTime);
          file.print(";");
          file.print(tempTemp);
          file.println("");
                 
          if (!file.sync() || file.getWriteError()) {
            error("write error");
            DEBUGSERIAL.println(" Slow write error error");
          }
          lastSavingTime = millis();

          ScreenThermocouple(true);
          
          }
        }         
         
     file.close();
     ResetButton();
     SlowSavingStats(fileName);
     }
     ResetButton();
  }


void thermocoupleHeader() {
  //Linear accel
    file.print("File created on : ");
    file.println(String(hour(),DEC)+ "h"+String(minute(),DEC)+ " - "+String(day(),DEC) + "/"+String(month(),DEC) + "/"+String(year(),DEC));
    file.print("Time t (ms)");
    file.print(";");   
    file.print("Temperature");
    file.println("");

}
