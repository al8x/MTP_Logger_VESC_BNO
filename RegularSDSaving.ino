
// The function must execute in this order :
// -- slowFindName()
// -- Open file
// -- Write header()
// -- While button not pressed
// ---- slowWriteData()

void slowLogData(){
  
  slowFindName();
  SdFile::dateTimeCallback(dateTime);

  
  if (!file.open(fileName, O_CREAT | O_WRITE | O_EXCL)) { // We attemp to open the file
    error("file.open");
    DEBUGSERIAL.println(" Slow file.open error");
  }
  
  else{ 
     Aquisition("Slow Log",fileName); // Diplsay the screen of aquisition data
     slowWriteHeader();
     
     while (!rightButtonPressed){
        slowWriteData();
        if (!file.sync() || file.getWriteError()) {
          error("write error");
          DEBUGSERIAL.println(" Slow write error error");
        }
     }
     file.close();
     ResetButton();
     SlowSavingStats(fileName);
     ResetButton();
  }
}

//-----------------------------------------------------


void slowFindName(){
    if (BASE_NASS_SIZE > 6) {
      error("FILE_BASE_NAME too long");
      DEBUGSERIAL.println(" Slow FILE_BASE_NAME too long");
  }
  while (sd.exists(fileName)) {
    if (fileName[BASE_NASS_SIZE + 1] != '9') {
      fileName[BASE_NASS_SIZE + 1]++;
    } else if (fileName[BASE_NASS_SIZE] != '9') {
      fileName[BASE_NASS_SIZE + 1] = '0';
      fileName[BASE_NASS_SIZE]++;
    } else {
      error("Can't create file name");
      DEBUGSERIAL.println(" Slow Can't create file name");
    }
  }  
}

void slowWriteData() {
  int theSize =3;
  uint16_t data[theSize];
  
  // Read all channels to avoid SD write latency between readings.
  for (uint8_t i = 0; i < theSize; i++) {
    data[i] = analogRead(i);
  }
  // Write data to file.  Start with log time in micros.
  file.print(millis());

  // Write ADC data to CSV record.
  for (uint8_t i = 0; i < theSize; i++) {
    file.write(',');
    file.print(data[i]);
  }
  file.println();
}

void slowWriteHeader() {
  file.print(F("File created on : "));
  file.println(String(hour(),DEC)+ "h"+String(minute(),DEC)+ " - "+String(day(),DEC) + "/"+String(month(),DEC) + "/"+String(year(),DEC));
  file.println("WheelDiameter : "+ String(wheel_diameter) + "mm ; Number of magnet : "+ String(nbAimentSurRoue)+ " ; Frequency : "+String(1000000/LOG_INTERVAL_USEC_TABLE[log_interval_selector])+ " Hz");
  file.print(F("time"));
  
  
  file.print(F(";Speed(km/h)"));
  file.print(F(";Sensor 1"));
  file.print(F(";Sensor 2"));
  file.println();
  file.print(F("micros"));
  
  file.println();
}
