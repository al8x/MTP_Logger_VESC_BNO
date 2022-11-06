
float rawDataArray[200]; // This global variable is used to store the data input 
int index_rawData=0;  // Global variable to go through the raw data array
long lastTimeGraph=millis();

void WideGraph(float newValue, int ScrollSpeed ,int XOrigine, int YOrigine, int graphHeight){
      
      const int InnerOffset = 4; // 4 pixels for graph offset
      const int LeftOffset = 30;
      const int graphedWidth = 128 -LeftOffset;
      const int graphed_Height = graphHeight - (2*InnerOffset);
  
      int scaledArray[graphedWidth];
      float maxValueInArray;
      float minValueInArray;

      const int gapRefresh = ((ScrollSpeed*1000)/graphedWidth);

    if (millis()-lastTimeGraph>gapRefresh){ //If long enough, We refresj=h the data
        lastTimeGraph=millis();
        
        // Update tab list with new Value
        rawDataArray[index_rawData] = newValue;  // Store the new value
        index_rawData+=1;  
        index_rawData = (index_rawData%graphedWidth);  // Limit the value to the max point to be display
    
    }
      
      
      // Update max & min value
      maxValueInArray = rawDataArray[0];
      minValueInArray = rawDataArray[0];
      for (int i=0; i<graphedWidth; i++){
        if (rawDataArray[i]>maxValueInArray) maxValueInArray=rawDataArray[i];
        if (rawDataArray[i]<minValueInArray) minValueInArray=rawDataArray[i];
      }
       
      // Remap the array  and display 
      for (int i=0; i<graphedWidth; i++){
        scaledArray[i] = mapfloat(rawDataArray[i],minValueInArray,maxValueInArray,0,graphed_Height);
        scaledArray[i] = (scaledArray[i]-graphed_Height)*(-1); // Reverse the value since the coordinate of the display are opposite as a graph
      }
    
      // Draw the graph
      int j = index_rawData-1;
      for (int i=0; i<(graphedWidth-1); i++){
        
        if (j<graphedWidth-1){ // If conditions to avoid the j+1 out of range value
          display.drawLine(LeftOffset+i, YOrigine+ InnerOffset + scaledArray[j], LeftOffset+i+1, YOrigine+ InnerOffset + scaledArray[j+1], WHITE);          
        }
               
        j+=1;
        j=(j%graphedWidth); // To have smooth animation, J starts with the last index saved point
      }

      //Display left graph scale
      display.drawLine(XOrigine+LeftOffset, YOrigine, XOrigine+LeftOffset, YOrigine+graphHeight, WHITE); //Vertical bar
      display.drawLine(XOrigine+LeftOffset-2, YOrigine+InnerOffset, XOrigine+LeftOffset+2, YOrigine+InnerOffset, WHITE); // Max value horizontal bar
      display.drawLine(XOrigine+LeftOffset-2, YOrigine+ graphHeight - InnerOffset, XOrigine+LeftOffset+2, YOrigine+ graphHeight - InnerOffset, WHITE); // Min value horizontal bar
  
      //Display the instant measured value
      display.setTextSize(1);
      display.setTextColor(BLACK);
      display.fillRect(XOrigine, YOrigine+(graphHeight/2)-5, LeftOffset, 10, WHITE);
      
      
      if ((maxValueInArray-minValueInArray>10)||(minValueInArray>100)){ // No float display
        display.setCursor(XOrigine+1,YOrigine+(graphHeight/2)-4);
        display.print(int(newValue));
        display.setTextColor(WHITE);
        display.setCursor(XOrigine+1,YOrigine);
        display.print(int(maxValueInArray));
        display.setCursor(XOrigine+1,YOrigine+ graphHeight - 10);
        display.print(int(minValueInArray));
      }
      else { // float display
        display.setCursor(XOrigine+1,YOrigine+(graphHeight/2)-4);
        display.print((newValue));
        display.setTextColor(WHITE);
        display.setCursor(XOrigine+1,YOrigine);
        display.print((maxValueInArray));
        display.setCursor(XOrigine+1,YOrigine+ graphHeight - 10);
        display.print((minValueInArray));
      }
}


///////////////////////////////////////////////////////
//---------------------FFT functions-------------------
///////////////////////////////////////////////////////

arduinoFFT FFT = arduinoFFT(); /* Create FFT object */
/*
These values can be changed in order to evaluate the functions
*/

const uint16_t samples = 512; //This value MUST ALWAYS be a power of 2
const double samplingFrequency = 10000; //Hz, must be less than 10000 due to ADC
const int bar_number = 128 ; // must be a power of 2
double amplitudeMax = 1000;

unsigned int sampling_period_us; 
unsigned long microseconds;

double vReal[samples];
double vImag[samples];


#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03

void FFT_Graph (int analogInput){
  const int graphHeight = 50;
  
  sampling_period_us = round(1000000*(1.0/samplingFrequency));

  /* -- 1 -- SAMPLING*/
  microseconds = micros();
  for(int i=0; i<samples; i++){
      vReal[i] = analogRead(analogInput);
      vImag[i] = 0;
      while(micros() - microseconds < sampling_period_us){
        //empty loop
      }
      microseconds += sampling_period_us;
   }
 
 /* -- 1 -- COMPUTE*/
  FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Weigh data */
  FFT.Compute(vReal, vImag, samples, FFT_FORWARD); /* Compute FFT */
  FFT.ComplexToMagnitude(vReal, vImag, samples); /* Compute magnitudes */
  //PrintVector(vReal, (samples >> 1), SCL_FREQUENCY);
  double x = FFT.MajorPeak(vReal, samples, samplingFrequency);
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(90,0);
  display.print(round(x)); 
  display.setCursor(115,0);
  display.print("Hz"); 
  display.setCursor(0,0);
  
  int j=0; // J is used to seek vReal values 
  int groupFactor = (samples>>1)/bar_number; // We group the bars togethers to have somethin clear to display
  
  for (int i=1;i<bar_number;i++){ 
  
    j=i*groupFactor;
    double temporarySum=0;
    for (int k=0;k<groupFactor;k++){
      temporarySum+=vReal[j+k];
    }
   
    temporarySum = map(temporarySum,0,amplitudeMax,0,graphHeight);
    temporarySum = constrain(temporarySum,0,graphHeight);
    display.fillRect(i, (graphHeight-temporarySum) , 1, temporarySum, WHITE);
  }
  
  
}


