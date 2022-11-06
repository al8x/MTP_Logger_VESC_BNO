void IMULogData(){
  
  slowFindName();
  SdFile::dateTimeCallback(dateTime);

  
  if (!file.open(fileName, O_CREAT | O_WRITE | O_EXCL)) { // We attemp to open the file
    error("file.open");
    DEBUGSERIAL.println(" Slow file.open error");
  }
  
  else{ 
    
     Aquisition("IMU Logging",fileName); // Diplsay the screen of aquisition data
     IMUWriteHeader();
     DEBUGSERIAL.println(" A");
     
     while (!rightButtonPressed){
      
        bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
        bno.getEvent(&accelerationData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
        bno.getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY);
        bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
        bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
        bno.getEvent(&magnetoData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
        
        IMUsdWriteData(&orientationData,&linearAccelData,&angVelocityData,&accelerationData,&gravityData,&magnetoData);

        
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
  //Linear accel
    file.print("linaAccel t");
    file.print(";");   
    file.print("linaAccel x");
    file.print(";");
    file.print("linaAccel y");
    file.print(";");
    file.print("linaAccel z");
    file.print(";");

    //Accel Data
    file.print("Accel t");
    file.print(";"); 
    file.print("accel x");
    file.print(";");
    file.print("accel y");
    file.print(";");
    file.print("accel z");
    file.print(";");

    //Gravity
    file.print("gravit t");
    file.print(";"); 
    file.print("gravit x");
    file.print(";");
    file.print("gravit y");
    file.print(";");
    file.print("gravit z");
    file.print(";");
    
    //Orientation
    file.print("orient t");
    file.print(";"); 
    file.print("orient x");
    file.print(";");
    file.print("orient y");
    file.print(";");
    file.print("orient z");
    file.print(";");

    //Angle velocity
    file.print("angleVelo t");
    file.print(";"); 
    file.print("angleVelo x");
    file.print(";");
    file.print("angleVelo y");
    file.print(";");
    file.print("angleVelo z");
    file.print(";");

    //Magneto data
    file.print("magnet t");
    file.print(";"); 
    file.print("magnet x");
    file.print(";");
    file.print("magnet y");
    file.print(";");
    file.print("magnet z");
    file.print(";");

    file.println();
}

void IMUsdWriteData(sensors_event_t* orient,sensors_event_t* linaAccel, sensors_event_t* angleVelo, sensors_event_t* accel, sensors_event_t* gravit,sensors_event_t* magnet){
    
    //Linear accel
    file.print(linaAccel->timestamp);
    file.print(";");
    file.print(linaAccel->acceleration.x);
    file.print(";");
    file.print(linaAccel->acceleration.y);
    file.print(";");
    file.print(linaAccel->acceleration.z);
    file.print(";");

    //Accel Data
    file.print(accel->timestamp);
    file.print(";");
    file.print(accel->acceleration.x);
    file.print(";");
    file.print(accel->acceleration.y);
    file.print(";");
    file.print(accel->acceleration.z);
    file.print(";");

    //Gravity
    file.print(gravit->timestamp);
    file.print(";");
    file.print(gravit->acceleration.x);
    file.print(";");
    file.print(gravit->acceleration.y);
    file.print(";");
    file.print(gravit->acceleration.z);
    file.print(";");
    
    //Orientation
    file.print(orient->timestamp);
    file.print(";");
    file.print(orient->orientation.x);
    file.print(";");
    file.print(orient->orientation.y);
    file.print(";");
    file.print(orient->orientation.z);
    file.print(";");

    //Angle velocity
    file.print(angleVelo->timestamp);
    file.print(";");
    file.print(angleVelo->gyro.x);
    file.print(";");
    file.print(angleVelo->gyro.y);
    file.print(";");
    file.print(angleVelo->gyro.z);
    file.print(";");

    //Magneto data
    file.print(magnet->timestamp);
    file.print(";");
    file.print(magnet->magnetic.x);
    file.print(";");
    file.print(magnet->magnetic.y);
    file.print(";");
    file.print(magnet->magnetic.z);
    file.print(";");

    file.println();
        
}
