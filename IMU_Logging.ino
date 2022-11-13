void IMULogData(){
  
  slowFindName();
  SdFile::dateTimeCallback(dateTime);

  
  if (!file.open(fileName, O_CREAT | O_WRITE | O_EXCL)) { // We attemp to open the file
    error("file.open");
    DEBUGSERIAL.println(" Slow file.open error");
  }
  
  else{ 

     startTimeLogImu=millis();           // We save the start time to start at 0
     Aquisition("IMU Logging",fileName); // Diplsay the screen of aquisition data
     IMUWriteHeader();
     DEBUGSERIAL.println(" A");
     
     while (!rightButtonPressed){
      
        refreshImuDatas();  
        IMUsdWriteData();
        
        if (!file.sync() || file.getWriteError()) {
          error("write error");
          DEBUGSERIAL.println(" Slow write error error");
           }
        }
     file.close();
     ResetButton();
     SlowSavingStats(fileName);
          
    }
     
   ResetButton();
  }



void IMUWriteHeader() {
  
    file.print("T (ms)");
    file.print(";");

    //Linear accel
    file.print("linaAccel x");
    file.print(";");
    file.print("linaAccel y");
    file.print(";");
    file.print("linaAccel z");
    file.print(";");

    //Accel Data
    file.print("accel x");
    file.print(";");
    file.print("accel y");
    file.print(";");
    file.print("accel z");
    file.print(";");

    //Gravity
    file.print("gravit x");
    file.print(";");
    file.print("gravit y");
    file.print(";");
    file.print("gravit z");
    file.print(";");
    
    //Orientation 
    file.print("orient x");
    file.print(";");
    file.print("orient y");
    file.print(";");
    file.print("orient z");
    file.print(";");

    //Angle velocity
    file.print("angleVelo x");
    file.print(";");
    file.print("angleVelo y");
    file.print(";");
    file.print("angleVelo z");
    file.print(";");

    //Magneto data 
    file.print("magnet x");
    file.print(";");
    file.print("magnet y");
    file.print(";");
    file.print("magnet z");
    file.print(";");

    // Put the temperature
    file.print("temperature(C)");
    file.print(";");

    // Put the pressure
    file.print("Pressure(hPa)");
    file.print(";");

    // Put the altitude
    file.print("Altitude(m)");
    file.print(";");

    file.println();
}

void IMUsdWriteData(){
    
    // Put the time
    file.print((IMU_data.timestamp)-startTimeLogImu);
    file.print(";");
    
    //Linear accel
    file.print(IMU_data.linearAcceleration.x);
    file.print(";");
    file.print(IMU_data.linearAcceleration.y);
    file.print(";");
    file.print(IMU_data.linearAcceleration.z);
    file.print(";");

    //Accel Data
    file.print(IMU_data.acceleration.x);
    file.print(";");
    file.print(IMU_data.acceleration.y);
    file.print(";");
    file.print(IMU_data.acceleration.z);
    file.print(";");

    //Gravity
    file.print(IMU_data.gravity.x);
    file.print(";");
    file.print(IMU_data.gravity.y);
    file.print(";");
    file.print(IMU_data.gravity.z);
    file.print(";");
    
    //Orientation
    file.print(IMU_data.orientation.x);
    file.print(";");
    file.print(IMU_data.orientation.y);
    file.print(";");
    file.print(IMU_data.orientation.z);
    file.print(";");

    //Angle velocity
    file.print(IMU_data.gyro.x);
    file.print(";");
    file.print(IMU_data.gyro.y);
    file.print(";");
    file.print(IMU_data.gyro.z);
    file.print(";");

    //Magneto data
    file.print(IMU_data.magnetic.x);
    file.print(";");
    file.print(IMU_data.magnetic.y);
    file.print(";");
    file.print(IMU_data.magnetic.z);
    file.print(";");

    // Put the temperature
    file.print(IMU_data.temperature);
    file.print(";");

    // Put the pressure
    file.print(IMU_data.pressure);
    file.print(";");

    // Put the altitude
    file.print(IMU_data.altitude);
    file.print(";");

    file.println();
        
}
