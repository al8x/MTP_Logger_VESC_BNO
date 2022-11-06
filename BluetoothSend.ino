// WOEK BEST With BLUETOOTH GRAPH APPLICATION


void Sensors_BluetoothSend(){

  SERIAL_BLUETOOTH.print("E");

  SERIAL_BLUETOOTH.print(readADC(1));
  SERIAL_BLUETOOTH.print(",");
  SERIAL_BLUETOOTH.print(readADC(0));
  SERIAL_BLUETOOTH.print(",");
  
  SERIAL_BLUETOOTH.print((currentSpeed)); 
  SERIAL_BLUETOOTH.print(",");

  SERIAL_BLUETOOTH.print(analogRead(ANALOG_ONE_PIN));
  SERIAL_BLUETOOTH.print(",");

  SERIAL_BLUETOOTH.print(analogRead(ANALOG_TWO_PIN));
  SERIAL_BLUETOOTH.print("\n");
  
}


void AnalogAccelero_BluetoothSend(){

  int coordX = analogRead(ACCELERO_X_PIN) ;
  int coordY = analogRead(ACCELERO_Y_PIN) ;
  int coordZ = analogRead(ACCELERO_Z_PIN) ;

  SERIAL_BLUETOOTH.print("E");

  SERIAL_BLUETOOTH.print(coordX);

  SERIAL_BLUETOOTH.print(",");
  
  SERIAL_BLUETOOTH.print(coordY);

  SERIAL_BLUETOOTH.print(",");
  
  SERIAL_BLUETOOTH.print(coordZ);

  SERIAL_BLUETOOTH.print("\n");
  
}
  
  
 
  
  
