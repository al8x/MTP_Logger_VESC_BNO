
void fastLog_programme(){
    
    // redirect the different case of the Saving programmes availables
    
    Aquisition("Fast logger","Name Search");     //Show the aquisition screen sying that we are logging
    createBinFile();                   // Find a for the File name
    Aquisition("Fast logger",binName); //Show the aquisition screen sying that we are logging
    ResetButton();
    fastLogData();                     //Fast log loop, interrupted with a right button pressed
    createCsvFile();
    binaryToCsv();
    //ShowSavingStats(binName,millis(),m,totalOverrun);
    ResetButton(); 

}


//==============================================================================
// Replace logRecord(), printRecord(), and ExFatLogger.h for your sensors.
void logRecord(data_t* data, uint16_t overrun) {
  if (overrun) {
    // Add one since this record has no adc data. Could add overrun field.
    overrun++;
    data->overrun_in = 0X8000 | overrun;
  } else {

      // Fisrt we save the time for every cases, and uses MicroSecs if frequency above 1000 Hz
      
      if (LOG_INTERVAL_USEC_TABLE[LOG_INTERVAL_VAL]<1000){
        data->time_s = float((float(micros())-float(microS_logStart))/1000000);
      }
      else{
        data->time_s = float((float(millis())-float(millisS_logStart))/1000);
      }
    
    
      switch (logging_application){   
                
        case EXTERNAL_MCP_SPEED:
          
          data->mcp_0 = readADC(0);
          data->mcp_1 = readADC(1);
          data->mcp_2 = readADC(2);
          data->Speed = currentSpeed;
          data->hallSensorOne = digitalRead(HALL_SENSOR_ONE_PIN);
          data->hallSensorTwo = digitalRead(HALL_SENSOR_TWO_PIN);
          break;
  
        case ANALOG_ACCELERO:
          
          data->adc_1 = analogRead(ACCELERO_X_PIN);
          data->adc_2 = analogRead(ACCELERO_Y_PIN);
          data->adc_3 = analogRead(ACCELERO_Z_PIN);
          data->Speed = currentSpeed;
          data->hallSensorOne = digitalRead(HALL_SENSOR_ONE_PIN);
          data->hallSensorTwo = digitalRead(HALL_SENSOR_TWO_PIN);
          break;
          
        case THERMOCOUPLE:
          
          int status = thermocouple.read();                
          data->temperature = thermocouple.getTemperature();
          data->Speed = currentSpeed;
          data->hallSensorOne = digitalRead(HALL_SENSOR_ONE_PIN);
          data->hallSensorTwo = digitalRead(HALL_SENSOR_TWO_PIN);
          break;
      }
  }
}

//------------------------------------------------------------------------------
void printRecord(Print* pr, data_t* data) {
  static uint32_t nr = 0;
  if (!data) { //  ----  Print Header
     switch (logging_application){       
        case EXTERNAL_MCP_SPEED:
          pr->print(F("File created on : "));
          pr->println(String(hour(),DEC)+ "h"+String(minute(),DEC)+ " - "+String(day(),DEC) + "/"+String(month(),DEC) + "/"+String(year(),DEC));
          pr->println("Wheel Diameter : "+ String(ListeDeDiametrePossible[RangDiametre]) + "mm ; Number of magnet : "+ String(nbAimentSurRoue)+ " ; Frequency : "+String(1000000/LOG_INTERVAL_USEC)+ " Hz");
          pr->print(F("time(Sec);Input 1;Input 2;Voltage bat;Speed(km/h);Digital 1;Digital 2"));
          pr->println();    
          break;
  
        case ANALOG_ACCELERO:
          pr->print(F("File created on : "));
          pr->println(String(hour(),DEC)+ "h"+String(minute(),DEC)+ " - "+String(day(),DEC) + "/"+String(month(),DEC) + "/"+String(year(),DEC));
          pr->println("Wheel Diameter : "+ String(ListeDeDiametrePossible[RangDiametre]) + "mm ; Number of magnet : "+ String(nbAimentSurRoue)+ " ; Frequency : "+String(1000000/LOG_INTERVAL_USEC)+ " Hz");
          pr->print(F("time(Sec);X;Y;Z;Speed(km/h);Digital 1;Digital 2"));
          pr->println();    
          break;
          
        case THERMOCOUPLE:
          pr->print(F("File created on : "));
          pr->println(String(hour(),DEC)+ "h"+String(minute(),DEC)+ " - "+String(day(),DEC) + "/"+String(month(),DEC) + "/"+String(year(),DEC));
          pr->println("Wheel Diameter : "+ String(ListeDeDiametrePossible[RangDiametre]) + "mm ; Number of magnet : "+ String(nbAimentSurRoue)+ " ; Frequency : "+String(1000000/LOG_INTERVAL_USEC)+ " Hz");
          pr->print(F("time(Sec);Temperature;Speed(km/h);Digital 1;Digital 2"));
          pr->println();    
          break;
      }

      nr = 0;
      return;
  }
  
  if (data->overrun_in & 0X8000) {
    uint16_t n = data->overrun_in & 0X7FFF;
    nr += n;
//    pr->print(F("-1,"));
//    pr->print(n);
//    pr->println(F(",overuns"));
  } 
  else {
      switch (logging_application){       
        case EXTERNAL_MCP_SPEED:
          pr->print(data->time_s,4);
          pr->write(';');
          pr->print(data->mcp_0);
          pr->write(';');
          pr->print(data->mcp_1);
          pr->write(';');
          pr->print(data->mcp_2);
          pr->write(';');
          pr->print(data->Speed);
          pr->write(';');
          pr->print(data->hallSensorOne);
          pr->write(';');
          pr->print(data->hallSensorTwo);
          pr->println();
          break;
  
        case ANALOG_ACCELERO:
          pr->print(data->time_s,4);
          pr->write(';');
          pr->print(data->adc_1);
          pr->write(';');
          pr->print(data->adc_2);
          pr->write(';');
          pr->print(data->adc_3);
          pr->write(';');
          pr->print(data->Speed);
          pr->write(';');
          pr->print(data->hallSensorOne);
          pr->write(';');
          pr->print(data->hallSensorTwo);
          pr->println();
          break;
          
        case THERMOCOUPLE:
          pr->print(data->time_s,4);
          pr->write(';');
          pr->print(data->temperature);
          pr->write(';');
          pr->print(data->Speed);
          pr->write(';');
          pr->print(data->hallSensorOne);
          pr->write(';');
          pr->print(data->hallSensorTwo);
          pr->println();
          break;
      }
  }
}

//------------------------------------------------------------------------------
#define error(s) sd.errorHalt(&Serial, F(s))
#define dbgAssert(e) ((e) ? (void)0 : error("assert " #e))
//-----------------------------------------------------------------------------

// Convert binary file to csv file.
void binaryToCsv() {

  uint8_t lastPct = 0;
  uint32_t t0 = millis();
  data_t binData[FIFO_DIM];

  if (!binFile.seekSet(512)) {
    error("binFile.seek failed");
  }
  uint32_t tPct = millis();
  printRecord(&csvFile, nullptr);
  while (!Serial.available() && binFile.available()) {
    int nb = binFile.read(binData, sizeof(binData));
    if (nb <= 0 ) {
      error("read binFile failed");
    }
    size_t nr = nb/sizeof(data_t);
    for (size_t i = 0; i < nr; i++) {
      printRecord(&csvFile, &binData[i]);
    }

    if ((millis() - tPct) > 500) { 
      uint8_t pct = binFile.curPosition()/(binFile.fileSize()/100);
      if (pct != lastPct) {
        tPct = millis();
        lastPct = pct;
        Serial.print(pct, DEC);
        Serial.println('%');
        csvFile.sync();
        DisplayProgressBinToCsv(global_CsvName, pct);
      }
    }
    if (Serial.available()) {
      break;
    }
  }
  csvFile.close();
  Serial.print(F("Done: "));
  Serial.print(0.001*(millis() - t0));
  Serial.println(F(" Seconds"));
}
//------------------------------------------------------------------------------
void clearSerialInput() {
  uint32_t m = micros();
  do {
    if (Serial.read() >= 0) {
      m = micros();
    }
  } while (micros() - m < 10000);
}



//-------------------------------------------------------------------------------
void createBinFile() {
  binFile.close();

  switch (logging_application){   
                
        case EXTERNAL_MCP_SPEED:
          memcpy(binName,"FastLog00.bin",sizeof(binName));
          break;
  
        case ANALOG_ACCELERO:
          memcpy(binName,"FastACC00.bin",sizeof(binName));
          break;
          
        case THERMOCOUPLE:
          memcpy(binName,"FastTem00.bin",sizeof(binName));
          break;   
  }
  
  while (sd.exists(binName)) {
    char* p = strchr(binName, '.');
    if (!p) {
      error("no dot in filename");
    }
    while (true) {
      p--;
      if (p < binName || *p < '0' || *p > '9') {
        error("Can't create file name");
      }
      if (p[0] != '9') {
        p[0]++;
        break;
      }
      p[0] = '0';
    }
  }
  if (!binFile.open(binName, O_RDWR | O_CREAT)) {
    error("open binName failed");
  }
  if (!binFile.preAllocate(PREALLOCATE_SIZE)) {
    error("preAllocate failed");
  }
}
//-------------------------------------------------------------------------------
bool createCsvFile() {
  char csvName[FILE_NAME_DIM];
  if (!binFile.isOpen()) {
    Serial.println(F("No current binary file"));
    return false;
  }

  // Create a new csvFile.
  binFile.getName(csvName, sizeof(csvName));
  char* dot = strchr(csvName, '.');
  if (!dot) {
    error("no dot in filename");
  }
  strcpy(dot + 1, "csv");
  if (!csvFile.open(csvName, O_WRONLY | O_CREAT | O_TRUNC)) {
    error("open csvFile failed");
  }
  clearSerialInput();
  global_CsvName=String(csvName);
  Serial.print(F("Writing: "));
  Serial.print(csvName);
  Serial.println(F(" - type any character to stop"));
  return true;
}
//-------------------------------------------------------------------------------
void fastLogData() {
  int32_t delta;  // Jitter in log time.
  int32_t maxDelta = 0;
  uint32_t maxLogMicros = 0;
  uint32_t maxWriteMicros = 0;
  size_t maxFifoUse = 0;
  size_t fifoCount = 0;
  size_t fifoHead = 0;
  size_t fifoTail = 0;
  uint16_t overrun = 0;
  uint16_t maxOverrun = 0;
  uint32_t totalOverrun = 0;
  uint32_t fifoBuf[128*FIFO_SIZE_SECTORS];
  data_t* fifoData = (data_t*)fifoBuf;

  // Write dummy sector to start multi-block write.
  dbgAssert(sizeof(fifoBuf) >= 512);
  memset(fifoBuf, 0, sizeof(fifoBuf));
  if (binFile.write(fifoBuf, 512) != 512) {
    error("write first sector failed");
  }
  clearSerialInput();

  // Wait until SD is not busy.
  while (sd.card()->isBusy()) {}

  // Start time for log file.
  uint32_t m = millis();

  // Time to log next record.
  uint32_t logTime = micros();
  millisS_logStart=m;
  microS_logStart=logTime;
  while (true) {
    // Time for next data record.
    logTime += LOG_INTERVAL_USEC;

    // Wait until time to log data.
    delta = micros() - logTime;
    if (delta > 0) {
      Serial.print(F("delta: "));
      Serial.println(delta);
      error("Rate too fast");
    }
    while (delta < 0) {
      delta = micros() - logTime;
    }

    if (fifoCount < FIFO_DIM) {
      uint32_t m = micros();
      logRecord(fifoData + fifoHead, overrun);
      m = micros() - m;
      if (m > maxLogMicros) {
        maxLogMicros = m;
      }
      fifoHead = fifoHead < (FIFO_DIM - 1) ? fifoHead + 1 : 0;
      fifoCount++;
      if (overrun) {
        if (overrun > maxOverrun) {
          maxOverrun = overrun;
        }
        overrun = 0;
      }
    } else {
      totalOverrun++;
      overrun++;
      if (overrun > 0XFFF) {
        error("too many overruns");
      }
    }
    // Save max jitter.
    if (delta > maxDelta) {
      maxDelta = delta;
    }
    // Write data if SD is not busy.
    if (!sd.card()->isBusy()) {
      size_t nw = fifoHead > fifoTail ? fifoCount : FIFO_DIM - fifoTail;
      // Limit write time by not writing more than 512 bytes.
      const size_t MAX_WRITE = 512/sizeof(data_t);
      if (nw > MAX_WRITE) nw = MAX_WRITE;
      size_t nb = nw*sizeof(data_t);
      uint32_t usec = micros();
      if (nb != binFile.write(fifoData + fifoTail, nb)) {
        error("write binFile failed");
      }
      usec = micros() - usec;
      if (usec > maxWriteMicros) {
        maxWriteMicros = usec;
      }
      fifoTail = (fifoTail + nw) < FIFO_DIM ? fifoTail + nw : 0;
      if (fifoCount > maxFifoUse) {
        maxFifoUse = fifoCount;
      }
      fifoCount -= nw;
      
      if (rightButtonPressed) { // Stoping the file record After the button Pressed
        break;
      }
    }
  }

  DisplayProgressBinToCsv(binName, 0);
  
  Serial.print(F("\nLog time: "));
  Serial.print(0.001*(millis() - m));
  Serial.println(F(" Seconds"));
  binFile.truncate();
  binFile.sync();
//  Serial.print(("File size: "));
  // Warning cast used for print since fileSize is uint64_t.
  Serial.print((uint32_t)binFile.fileSize());
  Serial.println(F(" bytes"));
  Serial.print(F("totalOverrun: "));
  Serial.println(totalOverrun);
  Serial.print(F("FIFO_DIM: "));
  Serial.println(FIFO_DIM);
  Serial.print(F("maxFifoUse: "));
  Serial.println(maxFifoUse);
  Serial.print(F("maxLogMicros: "));
  Serial.println(maxLogMicros);
  Serial.print(F("maxWriteMicros: "));
  Serial.println(maxWriteMicros);
  Serial.print(F("Log interval: "));
  Serial.print(LOG_INTERVAL_USEC);
  Serial.print(F(" micros\nmaxDelta: "));
  Serial.print(maxDelta);
  Serial.println(F(" micros"));
}
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
