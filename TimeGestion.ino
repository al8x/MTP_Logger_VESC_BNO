// RTC Functions
// Call back for file timestamps.  Only called for file create and sync().
void dateTime(uint16_t* date, uint16_t* time, uint8_t* ms10) {

  // Return date using FS_DATE macro to format fields.
  *date = FS_DATE(year(), month(), day());

  // Return time using FS_TIME macro to format fields.
  *time = FS_TIME(hour(), minute(), second());

  // Return low time bits in units of 10 ms.
  *ms10 = second() & 1 ? 100 : 0;
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  display.print(":");
  if(digits < 10)
    display.print('0');
  display.print(digits);
}

void digitalClockDisplay() {
  // digital clock display of the time
  display.print(hour());
  printDigits(minute());
  printDigits(second());
  display.print(" ");
  display.print(day());
  display.print(" ");
  display.print(month());
  display.print(" ");
  display.print(year());  
}

void digitalTimeDisplay() {
  // digital clock display of the time
  display.print(hour());
  printDigits(minute());
  printDigits(second()); 
}

time_t getTeensy3Time()
{
  return Teensy3Clock.get();
}
