 // Possible vector values can be:
 
  // - VECTOR_ACCELEROMETER - m/s^2
  // - VECTOR_MAGNETOMETER  - uT
  // - VECTOR_GYROSCOPE     - rad/s
  // - VECTOR_EULER         - degrees
  // - VECTOR_LINEARACCEL   - m/s^2
  // - VECTOR_GRAVITY       - m/s^2

    //bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
    //bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
    //bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
    //bno.getEvent(&accelerationData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
    //bno.getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY);
    //bno.getEvent(&magnetoData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
    //  int8_t boardTemp = bno.getTemp();
    
//  Serial.print(F("temperature: "));
//  Serial.println(boardTemp);

void IMU_RedirectingScreen(){
  
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
  
    bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
 
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println("       - IMU -  ");
    display.println("  Orientation ");

    display.println(" ");

    display.setCursor(0,20);
    display.print(" x : ");
    display.print(orientationData.orientation.x);
    display.println();

    display.setCursor(0,30);
    display.print(" y : ");
    display.print(orientationData.orientation.y);
    display.println();

    display.setCursor(0,40);
    display.print(" z : ");
    display.print(orientationData.orientation.z);
    display.println();

    display.drawLine(0, 50, 128, 50, WHITE);
    display.setCursor(0,55); 

  if (carteSdIn){
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
  
    bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);    

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println("       - IMU -  ");
    display.println("   Accelero Linear ");

    display.println(" ");

    display.setCursor(0,20);
    display.print(" x : ");
    display.print(linearAccelData.acceleration.x);

    display.setCursor(0,30);
    display.print(" y : ");
    display.print(linearAccelData.acceleration.y);

    display.setCursor(0,40);
    display.print(" z : ");
    display.print(linearAccelData.acceleration.z);

    display.println(" ");

    display.drawLine(0, 50, 128, 50, WHITE);
    display.setCursor(0,55); 

  if (carteSdIn){
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
  
    bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
 
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println("       - IMU -  ");
    display.println("        Gyroscope ");

    display.println(" ");

    display.setCursor(0,20);
    display.print(" x : ");
    display.print(angVelocityData.gyro.x);
    display.println();

    display.setCursor(0,30);
    display.print(" y : ");
    display.print(angVelocityData.gyro.y);
    display.println();

    display.setCursor(0,40);
    display.print(" z : ");
    display.print(angVelocityData.gyro.z);
    display.println();

    display.println(" ");

    display.drawLine(0, 50, 128, 50, WHITE);
    display.setCursor(0,55); 

  if (carteSdIn){
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
  
    bno.getEvent(&accelerationData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
 
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println("       - IMU -  ");
    display.println("     Accelerometer ");

    display.println(" ");

    display.setCursor(0,20);
    display.print(" x : ");
    display.print(accelerationData.acceleration.x);
    display.println();

    display.setCursor(0,30);
    display.print(" y : ");
    display.print(accelerationData.acceleration.y);
    display.println();

    display.setCursor(0,40);
    display.print(" z : ");
    display.print(accelerationData.acceleration.z);
    display.println();

    display.println(" ");

    display.drawLine(0, 50, 128, 50, WHITE);
    display.setCursor(0,55); 

  if (carteSdIn){
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
  
    bno.getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY); 
    
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println("       - IMU -  ");
    display.println("   Gravitometer ");

    display.println(" ");

    display.setCursor(0,20);
    display.print(" x : ");
    display.print(gravityData.acceleration.x);
    display.println();

    display.setCursor(0,30);
    display.print(" y : ");
    display.print(gravityData.acceleration.y);
    display.println();

    display.setCursor(0,40);
    display.print(" z : ");
    display.print(gravityData.acceleration.z);
    display.println();

    display.println(" ");

    display.drawLine(0, 50, 128, 50, WHITE);
    display.setCursor(0,55); 

  if (carteSdIn){
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
  
    bno.getEvent(&magnetoData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
    
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println("       - IMU -  ");
    display.println("   Magnetometer ");

    display.println(" ");

    display.setCursor(0,20);
    display.print(" x : ");
    display.print(magnetoData.magnetic.x);
    display.println();

    display.setCursor(0,30);
    display.print(" y : ");
    display.print(magnetoData.magnetic.y);
    display.println();

    display.setCursor(0,40);
    display.print(" z : ");
    display.print(magnetoData.magnetic.z);
    display.println();

    display.println(" ");

    display.drawLine(0, 50, 128, 50, WHITE);
    display.setCursor(0,55); 

  if (carteSdIn){
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
  
    int8_t boardTemp = bno.getTemp();
    
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println("       - IMU -  ");
    display.println("   Temperature ");
    display.println(" ");

    display.setCursor(20,20);
    display.setTextSize(3);
    display.print(boardTemp);

    display.setCursor(65,20);
    display.setTextSize(1);
    display.print("o");
    display.setCursor(70,20);
    display.setTextSize(2);
    display.print(" C ");

    display.setTextSize(1);
    display.drawLine(0, 50, 128, 50, WHITE);
    display.setCursor(0,55); 

  if (carteSdIn){
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

void ScreenIMUEvent(sensors_event_t* event) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  double x = -1000000, y = -1000000 , z = -1000000; //dumb values, easy to spot problem
  
  if (event->type == SENSOR_TYPE_ACCELEROMETER) {
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
    display.println(" ACCELEROMETER m/s^2");
  }

  else if (event->type == SENSOR_TYPE_LINEAR_ACCELERATION) {
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
    display.println(" LINEAR_ACCEL m/s^2");

  }

   else if (event->type == SENSOR_TYPE_GRAVITY ) {
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
    display.println("  GRAVITY m/s^2");
  }
  
  else if (event->type == SENSOR_TYPE_ORIENTATION) {
    x = event->orientation.x;
    y = event->orientation.y;
    z = event->orientation.z;
    display.println("  ORIENTATION deg");
  }
  else if (event->type == SENSOR_TYPE_MAGNETIC_FIELD) {
    x = event->magnetic.x;
    y = event->magnetic.y;
    z = event->magnetic.z;
    display.println("MAGNETIC_FIELD uT");
  }
  else if ((event->type == SENSOR_TYPE_GYROSCOPE) || (event->type == SENSOR_TYPE_ROTATION_VECTOR)) {
    x = event->gyro.x;
    y = event->gyro.y;
    z = event->gyro.z;
    display.println("  GYROSCOPE rad/s");
  }
  
   display.println("");

  display.print("  x = ");
  display.println(x);
  display.print("  y = ");
  display.println(y);
  display.print("  z = ");
  display.println(z);

  display.print("  Abs = ");
  display.println(sqrt((x*x)+(y*y)+(z*z)));

  display.println("");

  display.drawLine(0, 53, 128, 53, WHITE);
    
  display.setTextSize(1);
    
  display.print(" Home    ///     Log ");
    
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


