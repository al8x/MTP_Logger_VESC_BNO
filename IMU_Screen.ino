 // Possible vector values can be:
 
  // - VECTOR_ACCELEROMETER - m/s^2
  // - VECTOR_MAGNETOMETER  - uT
  // - VECTOR_GYROSCOPE     - rad/s
  // - VECTOR_EULER         - degrees
  // - VECTOR_LINEARACCEL   - m/s^2
  // - VECTOR_GRAVITY       - m/s^2

    //bno.getEvent(&IMU_data, Adafruit_BNO055::VECTOR_EULER);
    //bno.getEvent(&IMU_data, Adafruit_BNO055::VECTOR_LINEARACCEL);
    //bno.getEvent(&IMU_data, Adafruit_BNO055::VECTOR_GYROSCOPE);
    //bno.getEvent(&IMU_data, Adafruit_BNO055::VECTOR_ACCELEROMETER);
    //bno.getEvent(&IMU_data, Adafruit_BNO055::VECTOR_GRAVITY);
    //bno.getEvent(&IMU_data, Adafruit_BNO055::VECTOR_MAGNETOMETER);
    //  int8_t boardTemp = bno.getTemp();
    
//  Serial.print(F("temperature: "));
//  Serial.println(boardTemp);

void refreshImuDatas(){
  
  if (imuDetected==IMU_TYPE_BNO055){
      bno.getEvent(&IMU_data, Adafruit_BNO055::VECTOR_LINEARACCEL);    
      bno.getEvent(&IMU_data, Adafruit_BNO055::VECTOR_ACCELEROMETER); 
      bno.getEvent(&IMU_data, Adafruit_BNO055::VECTOR_GRAVITY);            
      bno.getEvent(&IMU_data, Adafruit_BNO055::VECTOR_EULER);          
      bno.getEvent(&IMU_data, Adafruit_BNO055::VECTOR_GYROSCOPE);      
      bno.getEvent(&IMU_data, Adafruit_BNO055::VECTOR_MAGNETOMETER);
      IMU_data.temperature = bno.getTemp();   
  }
  
  else if (imuDetected==IMU_TYPE_MPU925X){
      
      mpu.get_accel_g();
      mpu.get_gyro_d();
      mpu.get_mag_t();

      memset(&IMU_data, 0, sizeof(sensors_event_t));

      IMU_data.version = sizeof(sensors_event_t);
      IMU_data.timestamp = millis();
      
      mpu.get_accel();
      IMU_data.acceleration.x = mpu.x;
      IMU_data.acceleration.y = mpu.y;
      IMU_data.acceleration.z = mpu.z;

      IMU_data.gravity.x = mpu.x_g;
      IMU_data.gravity.y = mpu.y_g;
      IMU_data.gravity.z = mpu.z_g;

      IMU_data.orientation.x = 0;  // because the bn0 has no sensors
      IMU_data.orientation.y = 0;
      IMU_data.orientation.z = 0;
      
      IMU_data.magnetic.x = mpu.mx_t;
      IMU_data.magnetic.y = mpu.my_t;
      IMU_data.magnetic.z = mpu.mz_t;

      IMU_data.gyro.x = mpu.gx_d;
      IMU_data.gyro.y = mpu.gy_d;
      IMU_data.gyro.z = mpu.gz_d;

      IMU_data.temperature = bme.readTemperature();
      IMU_data.pressure = bme.readPressure();
      IMU_data.altitude = bme.readAltitude(1013.25);

  }
  else{
    DEBUGSERIAL.println("Error in the IMU ssignement");
  }
}

void IMU_RedirectingScreen(){

  refreshImuDatas();  
  
  switch (IMU_ScreenSelector) {
    case 0:
      ScreenOrientData();
      break;
    case 1:
      ScreenLinearAccelData();
      break; 
    case 2:
      ScreenAngleVelocityData();
      break;
    case 3:
      ScreenAccelerationData();
      break;
    case 4:
      ScreenGravityData();
      break;
    case 5:
     ScreenMagnetoData();
      break;
    case 6:
    ScreenTemperature();
      break; 
  }
}

void ScreenOrientData(){
   
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println("       - IMU -  ");
    display.setCursor(20,8);
    display.print("Orientation ");

    display.setCursor(0,20);
    display.print(" x : ");
    display.print(IMU_data.orientation.x);
    display.println();

    display.setCursor(0,30);
    display.print(" y : ");
    display.print(IMU_data.orientation.y);
    display.println();

    display.setCursor(0,40);
    display.print(" z : ");
    display.print(IMU_data.orientation.z);
    display.println();

    display.drawLine(0, 50, 128, 50, WHITE);
    display.setCursor(0,55); 

  if (imuDetected&&carteSdIn){
    display.print(" +1   ---o--   IMULog");
  }
  else{
    display.print(" +1   ---o--      *  ");
  }
      
    display.setCursor(0,0);
    display.display();
    
    display.clearDisplay();
    
}

void ScreenLinearAccelData(){
  
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println("       - IMU -  ");
    display.setCursor(20,8);
    display.println("Accelero Linear ");

    display.setCursor(0,20);
    display.print(" x : ");
    display.print(IMU_data.linearAcceleration.x);

    display.setCursor(0,30);
    display.print(" y : ");
    display.print(IMU_data.linearAcceleration.y);

    display.setCursor(0,40);
    display.print(" z : ");
    display.print(IMU_data.linearAcceleration.z);

    display.println(" ");

    display.drawLine(0, 50, 128, 50, WHITE);
    display.setCursor(0,55); 

  if (imuDetected&&carteSdIn){
    display.print(" +1   ---o--   IMULog");
  }
  else{
    display.print(" +1   ---o--      *  ");
  }     
    display.setCursor(0,0);
    display.display();
    
    display.clearDisplay();
    
}

void ScreenAngleVelocityData(){
   
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println("       - IMU -  ");
    display.setCursor(20,8);
    display.println("Gyroscope ");

    display.setCursor(0,20);
    display.print(" x : ");
    display.print(IMU_data.gyro.x);
    display.println();

    display.setCursor(0,30);
    display.print(" y : ");
    display.print(IMU_data.gyro.y);
    display.println();

    display.setCursor(0,40);
    display.print(" z : ");
    display.print(IMU_data.gyro.z);
    display.println();

    display.println(" ");

    display.drawLine(0, 50, 128, 50, WHITE);
    display.setCursor(0,55); 

  if (imuDetected&&carteSdIn){
    display.print(" +1   ---o--   IMULog");
  }
  else{
    display.print(" +1   ---o--      *  ");
  }
      
    display.setCursor(0,0);
    display.display();
    
    display.clearDisplay();
    
}

void ScreenAccelerationData(){
   
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println("       - IMU -  ");
    display.setCursor(20,8);
    display.println("Accelerometer ");

    display.setCursor(0,20);
    display.print(" x : ");
    display.print(IMU_data.acceleration.x);
    display.println();

    display.setCursor(0,30);
    display.print(" y : ");
    display.print(IMU_data.acceleration.y);
    display.println();

    display.setCursor(0,40);
    display.print(" z : ");
    display.print(IMU_data.acceleration.z);
    display.println();

    display.println(" ");

    display.drawLine(0, 50, 128, 50, WHITE);
    display.setCursor(0,55); 

  if (imuDetected&&carteSdIn){
    display.print(" +1   ---o--   IMULog");
  }
  else{
    display.print(" +1   ---o--      *  ");
  }
      
    display.setCursor(0,0);
    display.display();
    
    display.clearDisplay();
    
}


void ScreenGravityData(){
      
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println("       - IMU -  ");
    display.setCursor(20,8);
    display.println("Gravitometer ");

    display.setCursor(0,20);
    display.print(" x : ");
    display.print(IMU_data.gravity.x);
    display.println();

    display.setCursor(0,30);
    display.print(" y : ");
    display.print(IMU_data.gravity.y);
    display.println();

    display.setCursor(0,40);
    display.print(" z : ");
    display.print(IMU_data.gravity.z);
    display.println();

    display.println(" ");

    display.drawLine(0, 50, 128, 50, WHITE);
    display.setCursor(0,55); 

  if (imuDetected&&carteSdIn){
    display.print(" +1   ---o--   IMULog");
  }
  else{
    display.print(" +1   ---o--      *  ");
  }
      
    display.setCursor(0,0);
    display.display();
    
    display.clearDisplay();
    
}

void ScreenMagnetoData(){
      
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println("       - IMU -  ");
    display.setCursor(20,8);
    display.println("Magnetometer ");
    
    display.setCursor(0,20);
    display.print(" x : ");
    display.print(IMU_data.magnetic.x);
    display.println();

    display.setCursor(0,30);
    display.print(" y : ");
    display.print(IMU_data.magnetic.y);
    display.println();

    display.setCursor(0,40);
    display.print(" z : ");
    display.print(IMU_data.magnetic.z);
    display.println();

    display.println(" ");

    display.drawLine(0, 50, 128, 50, WHITE);
    display.setCursor(0,55); 

  if (imuDetected&&carteSdIn){
    display.print(" +1   ---o--   IMULog");
  }
  else{
    display.print(" +1   ---o--      *  ");
  }
      
    display.setCursor(0,0);
    display.display();
    
    display.clearDisplay();
    
}

void ScreenTemperature(){
      
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println("       - IMU -  ");
    display.setCursor(20,8);
    display.println("External");

    display.setCursor(0,20);
    display.print("Temperature ");
    display.print(IMU_data.temperature); 
    display.print(" C");

    display.setCursor(0,30);
    display.print("Pressure ");
    display.print(IMU_data.pressure);
    display.print(" hPa");

    display.setCursor(0,40);
    display.print("Altitude ");
    display.print(IMU_data.altitude);
    display.print(" m");

    display.setTextSize(1);
    display.drawLine(0, 50, 128, 50, WHITE);
    display.setCursor(0,55); 

  if (imuDetected&&carteSdIn){
    display.print(" +1   ---o--   IMULog");
  }
  else{
    display.print(" +1   ---o--      *  ");
  }
      
    display.setCursor(0,0);
    display.display();
    
    display.clearDisplay();
    
}

//------------------------------------------



int appendValueReturnMinMax(int newValue, int arrayOfValues[], int sizeOfArray){
    
    int averageValueResult;
    int minValue = arrayOfValues[0];
    int maxValue = arrayOfValues[0];

    for (int i = (sizeOfArray-1); i >=1; i--){ // We go through all the array exept the first cell

      if (arrayOfValues[i]>maxValue){
        maxValue = arrayOfValues[i];
      }
      else if (arrayOfValues[i]<minValue){
        minValue = arrayOfValues[i];
      }
      
      arrayOfValues[i]=arrayOfValues[i-1]; // We switch the value 
      
    }
    
    arrayOfValues[0] = newValue;
    if (newValue>maxValue){
        maxValue = newValue;
      }
    else if (newValue<minValue){
      minValue = newValue;
    }
    
    return (minValue,maxValue);
  
}
