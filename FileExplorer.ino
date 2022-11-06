// This functions help you to dump datas from SD file to Serial port in order to access them much faster
// At the end of a recorder sequence, the screen propose you a dump data function that open the file and stream datas 
// directly to the serial Port

void USB_dumpFile(char FileAccessName[]) {
  
  if (!file.open(FileAccessName, O_READ)) { // We attemp to open the file
    error("file.open");
    DEBUGSERIAL.print(FileAccessName);
    DEBUGSERIAL.println(" open error");
  }
  
  else{ 
    DEBUGSERIAL.println("----------");
    DEBUGSERIAL.println(FileAccessName);

    // read from the file until there's nothing else in it:
    while (file.available()) {
      DEBUGSERIAL.write(file.read());
    }
    
    DEBUGSERIAL.println("----------");
    // close the file:
    file.close();
  } 
}


void scrollAndDumpFiles(){

  const int MAX_NAME_LIST = 200;
  #define MAX_LENGHT_NAME  30
  
  char fname[MAX_LENGHT_NAME];
  char CSV_EXT[]=".csv";
  char csvFileList [MAX_NAME_LIST][MAX_LENGHT_NAME];

  int fileIndex = 0;

  //-----------------------------------------------------
  // 0 - Create a list of all CSV file in the Directory
  //-----------------------------------------------------

  // Go To Root
    // Change current directory to root.
  if (!sd.chdir()) {
    error("chdir to root failed.\n");
  }
  
  if (!dir.open("/")){
    error("dir.open failed");
  }

  while (file.openNext(&dir, O_RDONLY)) {
    // This loop create an array of all the CSV file available to be USB dumped in the root directory

    if (file.isFile()){ // Exclude automatically directories and work at the root
      file.getName(fname,MAX_LENGHT_NAME); // Store the name for file manipulation
      if ((strcmp(CSV_EXT, &fname[strlen(fname)-strlen(CSV_EXT)]) == 0)&&(fileIndex<MAX_NAME_LIST)){  // ==> CSV File found

        for (int i=0;i<MAX_LENGHT_NAME;i++){// Store the file Name
          csvFileList[fileIndex][i]=fname[i]; 
        }
        
        fileIndex++;
      } 
    }
    file.close();  // Need to be closed for the USB dump to always work
  }
  
  dir.close();
  
  const int csvFileFound = fileIndex;
  fileIndex=0;

  //-----------------------------------------------------
  // 1 - Debug Print the list
  //-----------------------------------------------------
  
  for (int i=csvFileFound-1;i>=0;i--){
    Serial.println(csvFileList[i]);
  }

  //-----------------------------------------------------
  // 2 - Interface for user choice
  //-----------------------------------------------------

    fileIndex = csvFileFound-1;
    ResetButton();

    if (csvFileFound<1){ // If no files found, we exit right away
      Serial.println("No files found");
      displayPopUpMessage("No files found",1000);
    }
    
    else{
            
      while ((!leftButtonPressed)){

          displayFilesNames(csvFileList,fileIndex,csvFileFound); // Display the interface on the screen
          
          if (rightButtonPressed){ // We go to Next Files
            fileIndex--;
            if (fileIndex<0){  // End of the list ? Go back to begining
              fileIndex = csvFileFound-1;
            }
            fileIndex = constrain(fileIndex,0,csvFileFound);
            rightButtonPressed = false;
          }
          
          if (middleButtonPressed){
            USB_dumpFile(csvFileList[fileIndex]);
            middleButtonPressed = false;
          }
          
          delay(2);
      }
    }      
  ResetButton();
 
}



void displayFilesNames(char binName[][MAX_LENGHT_NAME],int index, int csvFileFound){
    
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  display.setCursor(10,5);
  if ((index+1)<=(csvFileFound-1)){
    display.println(binName[index+1]);
  }
  
  display.fillRect(0, 18, 115, 12, WHITE);
  display.setTextColor(BLACK);
  display.setCursor(10,20);
  display.println(binName[index]);

  display.setTextColor(WHITE);
  display.setCursor(10,35);
  if ((index-1)>=0){
    display.println(binName[index-1]);
  }

  // Side Scroll Bar for 
  int heightScrollBar = 46/csvFileFound;
  heightScrollBar = constrain(heightScrollBar,1,46);
  int Y_Position = ((csvFileFound-index)*(46-heightScrollBar))/csvFileFound; 
  display.fillRect(120, Y_Position+1 , 5, heightScrollBar, WHITE);  
  
  display.setCursor(0,55);
  display.print("Exit  USB-Dump  Next");
 
  display.drawLine(0, 50, 128, 50, WHITE);
  display.display();
  display.clearDisplay();
  
}
