
//---------------------------------------
//------ MAIN PROGRAMME
//---------------------------------------


void wheelEfficiency() {
  // initialize stats variable
  
  wheelEffMesure = false;
  Fini =false;
  DataAlreadySaved = false;
  
  indX=0; //On met l'index dans stockage à l'origine
  NombrePassageAimant=0; // Valeur qui ne doit pas être changé, initialise le nombre de passage d'aiment observé
    
  //---------------------------------------------------------------------------------

  display.clearDisplay();
  ResetButton();

   //--------------------------------------------------------------------------------------------------
  if (speedKnown){                            // Step 2 - Screen speed, record did not start
    
    ResetButton();
    display.clearDisplay();
    
    wheelEffMesure=true; // The behavior of the magnet interrupt change now because Mesure is true. It save the time in the array
      
    //-------------------------------------
    while (!Fini){                                            // Step 3 - Recording
      AfficherAvancement();
      //Now, the function Passage aimant controle the end of this loop by changing the fini to true when the number
      //of point meet the requirement

      

       // On stop la mesure quand on a tous les points
      if (indX>NbrePtsMesure){
        wheelEffMesure=false;
        Fini=true;
      }  
    }
  
    ResetButton(); // We set each bouton false in case of an undesired action
    display.clearDisplay();

     //---------------------------------------------------------------------------------------------------
                                                         // Step 4 - Screen results and save if desired
    bool screenChoice = false;
    
    while (!leftButtonPressed){ 
      
      // SCREEN -----Show exponentiell results or the Speed Graph
      if (screenChoice){
        AfficheResultatExponentielle();
      }
  
      else{
        GraphVitesse();
      }
      
      // Behavior on button -----Show exponentiell results or the Speed Graph
      //Interface, on left button -> endo of loop and back to beggining
      //           on middle button -> Save data
      //           on right button -> switch what is on screen
      
      if (middleButtonPressed){ // 
        middleButtonPressed=false; // Button reinitilastion
        Sauvegarder();
        DataAlreadySaved=true;

      }
      
      else if (rightButtonPressed){
        rightButtonPressed=false; // Button reinitilastion
        screenChoice=(!screenChoice);
      }
    }
  }
  
   ResetButton(); // Reset the action of the left button
}
//----------------------------------------------------------------------- Fin programme


//------------------------------------------------------------
//-------------  InterruptProgramme
//------------------------------------------------------------


void WheelEfficiencyMagnet(){
    
      speedKnown=true; //We can screen speed with one magnet time (origine = whenlogger turned on)
      
     if (wheelEffMesure){
      NombrePassageAimant+=1; //On indique le passage d'un aiamnt en aquisition
      // on stock dans tableau que si il s'agit de l'aimant qui corrrepond a un tour de roue
      if ((NombrePassageAimant%nbPassageAimentParAquisition)==0){
        Stockage[indX]=currentMagnetTime/1000 ; //Converion of currentMagnetTime from microsec to millis
        indX+=1;
      }  
     }   
}



//------------------------------------------------------------
//-------------  SD SAVING Functions
//------------------------------------------------------------

void Sauvegarder(){
  delay(100);
   if (DataAlreadySaved){
    ScreenDataAlreadySaved();
    delay(1000);
  }
  else if (carteSdIn&&Fini){
    SaveData();
  }
}

void SaveData()
{
  // 0 -- Put sample number in base Name
  
  
  // 1 -- Find an unused file name.
  slowFindName();
  SdFile::dateTimeCallback(dateTime);
  
  // 2 --- Open the file and write parameters
  
   if (!file.open(fileName, O_CREAT | O_WRITE | O_EXCL)) { // We attemp to open the file
    error("file.open");
    DEBUGSERIAL.println(" Slow file.open error");
    delay(2000);
  }
  
  else{   // -Do the JOB
  
    file.write(" --- Parameters --- ");
    
    file.write("File created on : ");file.print(String(hour(),DEC)); file.write("h");file.print(String(minute(),DEC));file.write("-");
    file.print(String(day(),DEC));file.write("/");file.print(String(month(),DEC));file.write("/");file.print(String(year(),DEC));
    file.println();
     
    file.write("WheelDiameter : ");file.print(ListeDeDiametrePossible[RangDiametre]); file.write("mm ; Nb passage aimants par aquisition : ");file.println(nbPassageAimentParAquisition);
    file.write(" ; Numero d'échantillion : ");file.print(NumeroDeSAmple);file.write(" ; Nombre d'aimants sur la roue ");file.print(nbAimentSurRoue);
    file.println();
  
  
    // 3 - Generate datas Linear and save
    //--------------------------------------
    float LesTempsEnX[NbrePtsMesure];
    float LesVitesseEnY[NbrePtsMesure];
    TempsAPartirStockage(Stockage,LesTempsEnX);
    VitesseAPartirStockage(Stockage,LesVitesseEnY);
  
    float MoyenneXi=moyenne(somme(LesTempsEnX));
    float MoyenneYi=moyenne(somme(LesVitesseEnY));  
    float Lapente=pente(SommeProduitTab(LesTempsEnX,LesVitesseEnY),MoyenneXi,MoyenneYi,SommeCarreEcartAMoyenne(LesTempsEnX,MoyenneXi));
    float OrdonneAlOrigine = MoyenneYi-Lapente*MoyenneXi;
    float RCarre = pow(sqrt((Lapente*Lapente*SommeCarreEcartAMoyenne(LesTempsEnX,MoyenneXi))/(SommeCarreEcartAMoyenne(LesVitesseEnY,MoyenneYi))),2);  
  
    file.println();
    file.write("--- LINEAIRE---");
    file.println();
    file.write("A= ");
    file.println(Lapente,4); 
    file.write("B= ");
    file.println(OrdonneAlOrigine,3);
    file.write("R2= ");
    file.println(RCarre,5);
    
    //--------------------------------------
    // 4 - Generate datas exponentials and save
    VitesseToLNVitesse(LesVitesseEnY);
    
    MoyenneXi=moyenne(somme(LesTempsEnX));
    MoyenneYi=moyenne(somme(LesVitesseEnY));
        
    Lapente=pente(SommeProduitTab(LesTempsEnX,LesVitesseEnY),MoyenneXi,MoyenneYi,SommeCarreEcartAMoyenne(LesTempsEnX,MoyenneXi));
    OrdonneAlOrigine = MoyenneYi-Lapente*MoyenneXi;
    RCarre = pow(sqrt((Lapente*Lapente*SommeCarreEcartAMoyenne(LesTempsEnX,MoyenneXi))/(SommeCarreEcartAMoyenne(LesVitesseEnY,MoyenneYi))),2);  
    
    file.println();
    file.write("---Exponentielle---");
    file.println();
    file.write("A= ");
    file.println(Lapente,4); 
    file.write("B= ");
    file.println(exp(OrdonneAlOrigine),3);
    file.write("R2= ");
    file.println(RCarre,5);
  
    //Save the datas
  
    file.println();
    file.write(" --- Datas --- ");
    file.println();
    
    
    for (int i=0; i<NbrePtsMesure ; i++){
      file.print(LesTempsEnX[i],3);
      file.write(";");
      file.println(exp(LesVitesseEnY[i]),3); //On remet les vitesse en exponentielle car à ce stade le ln des vitesse est dans ce tableau
      
    }
    file.println();
    file.write("------------------------------------");
    
    // 5 Close the file
    
    file.close();
  
    // 6 We say it's saved
    ScreenDataSaved(fileName);
    delay(2000);
  }
}

//------------------------------------------------------------
//-------------  SETTINGS
//------------------------------------------------------------



void SettingsWheelEfficiency(){
  //display the settings screen and allowed the user to change
  // on 0 : the diameter of the wheel
  // on 1 The number of points to aquire
  // on 2 the number of wheel roatation per aquistion
  
  int numberOfchoicesAvailable = 3;
  int highlightedChoice = 0;
  
  while (!leftButtonPressed){
    delay(10);
    screenGeneralSettingWheelEfficiency(highlightedChoice); //Interface on the screen
    
    if (middleButtonPressed){ // execute the choice
      middleButtonPressed=false;
      switch (highlightedChoice) {
        case 0:{
          setDiameter();
          
        }
        break;
        case 1:{
          SetDistanceDeMesure();
      }
        break;
        case 2:{
          SetSampleNumber();
      }
         break;
      }
    }
    
    else if (rightButtonPressed){ // Change the highlighted choice
      highlightedChoice+=1;
      highlightedChoice=highlightedChoice%numberOfchoicesAvailable; // We keep the range
      rightButtonPressed=false;
      }
   
  }
  // at the end, we set the final frequency by changing the value
  leftButtonPressed=false;
}

void SetDistanceDeMesure(){

  while (!middleButtonPressed){
    delay(10);
    ScreenChooseDistanceDeMesure(); //Interface on the screen
    
    if (leftButtonPressed){
      leftButtonPressed=false; // Button reinitilastion
      
      //We set the minimum 
      if (DistanceDeMesure>2){ // If the minimum is not reached
        DistanceDeMesure=DistanceDeMesure-1;
      }
    }
    
    else if (rightButtonPressed){
      rightButtonPressed=false; // Button reinitilastion
      if (DistanceDeMesure<999){ // If the minimum is not reached
        DistanceDeMesure=DistanceDeMesure+1;
      }
    }
  }
  // at the end, we can leave with the modified rank
  middleButtonPressed=false;
  NbrePtsMesure = (DistanceDeMesure*nbAimentSurRoue)/(nbPassageAimentParAquisition*(ListeDeDiametrePossible[RangDiametre]/(float)1000)*3.14159); 
  // We update the value of the number of points
}


void SetSampleNumber(){

  while (!middleButtonPressed){
    delay(10);
    ScreenChooseSampleNumber(); //Interface on the screen
    
    if (leftButtonPressed){
      leftButtonPressed=false; // Button reinitilastion
      
      //We set the minimum 
      if (NumeroDeSAmple>1){ // If the minimum is not reached
        NumeroDeSAmple=NumeroDeSAmple-1;
      }
    }
    
    else if (rightButtonPressed){
      rightButtonPressed=false; // Button reinitilastion
      if (NumeroDeSAmple<99){ // On limite la valeur du numéro de sample à 99 max
        NumeroDeSAmple +=1; // On incrémente la valeur du nruméro de sample
      }
      
    }
  }
  // at the end, we can leave with the modified rank
  middleButtonPressed=false;
}


//------------------------------------------------------------
//-------------TOUTE LA PARTIE REGRESSION LINEAIRE
//------------------------------------------------------------

float somme(float tab[])
{
  float somme=0;
  for(int i = 0; i < NbrePtsMesure; i++)
  {
   somme=((tab[i])+(somme));
   }
  return(somme);
}


/*Fonction de calcul de moyenne d'element d'un tableau*/
float moyenne(float somme)
{
  float moyenne;
  moyenne=((somme)/(float)NbrePtsMesure);
  return (moyenne);
}


/*Fonction de calcul du produit d'element de deux tableau ligne par ligne*/
float SommeProduitTab(float tab1[],float tab2[])
{
  float SommeProduit=0;
  for(int i = 0; i < NbrePtsMesure; i++)
  {
    SommeProduit+=(tab1[i]*tab2[i]);
  }
  return SommeProduit;
}

float SommeCarreEcartAMoyenne(float tab[],float Moyenne){
  //Calcul la somme des (Xi-Moyenne)XiCarré
  float SommeEcartAmoyenne=0;
  
  for(int i = 0; i < NbrePtsMesure; i++){
    SommeEcartAmoyenne+=((tab[i]-Moyenne)*(tab[i]-Moyenne));
  }
  return SommeEcartAmoyenne;
}

/*Fonction de calcul de la pente "a" de la droite*/
float pente(float SommeProduitXiYi,float MoyenneXi,float MoyenneYi,float SommeCarreXiMoinMoyenneXi)
{
  return (((SommeProduitXiYi)-(NbrePtsMesure*MoyenneXi*MoyenneYi))/SommeCarreXiMoinMoyenneXi);
}

//----------------------------------------------------------------------------------------

//----------------------------------------------------------
// -----------------------CONVERSION  DES POINTS DAQUISITION
//----------------------------------------------------------

float Vitesse(int TmillisMoinsUn,int Tmillis){
  // Prend 2 vitesse en argument (en millis sec) et retourne une vitesse en km/h
  return ((ListeDeDiametrePossible[RangDiametre])*3.14159*3.6)/((float)(Tmillis-TmillisMoinsUn)*nbAimentSurRoue); //Convertion en Km/h
}

float TempsAPartirStockage(int Tab[],float AxeX[]){
  //Soustrait la première valeur a toute les valeurs pour avoir une courbe qui commence a 0 en x
  for(int i = 0; i < NbrePtsMesure; i++)
  {
    AxeX[i]=(Tab[i]-Tab[0])/(float)(1000); //On passe le temps en seconde
  }
}

float VitesseAPartirStockage(int Tab[],float AxesVitesse[]){
  //On multiplie les vitesse par 1000 pour retser en int
  for(int i = 0; i < NbrePtsMesure; i++)
  {
    AxesVitesse[i]=Vitesse(Tab[i],Tab[i+1])*nbPassageAimentParAquisition; //  On remultiplie les vitesse par le nombre de mesure par passage pour avoir la vraie valeurs
  }
}

float VitesseToLNVitesse (float Tab[]){
  // Transforme les vitesse en log de vitesse pour une regression exponenielle
   for(int i = 0; i < NbrePtsMesure; i++)
  {
    Tab[i]=(float)(log(Tab[i]));
  }
}

//-----------------------------------------------------------
//----------------------------------FONCTION D'AFFICHAGE
//----------------------------------------------------------

void screenWheelEfficiencyLanding(){

    display.setTextSize(1);
    display.println("  WHEEL EFFICIENCY");
    display.setTextColor(WHITE);
    display.println("");
    display.setCursor(10,15);
    display.setTextSize(4);
    if (speedKnown){
      //display.println(Vitesse(tMoin1,t));
      display.print(int(currentSpeed));
      display.setCursor(70,15);
      display.setTextSize(2);
      display.print(".");
      display.print(int((currentSpeed-int(currentSpeed))*100));
    }
    else{
      display.println(0);
    }
    display.setTextSize(1);
    display.setCursor(70,35);
    display.print(" Km/h");
    display.println();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,55);
    
  if (speedKnown){
      display.print(" Set  --o---   Start ");
    }
  else{
      display.print(" Set  --o---      *  ");
    }
    
  display.setCursor(0,0);
  display.drawLine(0, 50, 128, 50, WHITE);
  display.display();
  display.clearDisplay();
}


void screenGeneralSettingWheelEfficiency(int choice){
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println("");
    display.setTextSize(1);

    if (choice==0){
      display.setTextColor(BLACK);
      display.fillRect(0, 4, 128, 16, WHITE);
    }
    
    display.print(" Wheel diameter ");
    display.print(ListeDeDiametrePossible[RangDiametre]);
    display.println("mm");
    display.println("");
    display.setTextColor(WHITE);
    
    
    if (choice==1){
      display.setTextColor(BLACK);
      display.fillRect(0, 20, 128, 16, WHITE);
    }
    
    display.print(" Distance - ");
    display.print(DistanceDeMesure);
     display.print(" m");
    display.println("");
    display.println("");
    display.setTextColor(WHITE);
    
    if (choice==2){
      display.setTextColor(BLACK);
      display.fillRect(0, 36, 128, 16, WHITE);
    }

    display.print(" Sample Number - ");
    display.print(NumeroDeSAmple);
    display.println("");
    display.println("");
    display.setTextColor(WHITE);
    
    display.drawLine(0, 53, 128, 53, WHITE);
    
    display.setTextSize(1);
    
    display.print(" Exit    Set     Next");
    
    display.setCursor(0,0);
    display.display();
    display.clearDisplay();
}


void ScreenChooseSampleNumber(){
  // Ecran ou on le nombre de tours par aquisition
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println("Sample number");
    display.setTextSize(2);
    display.println("");
    display.print("     ");
    display.print(NumeroDeSAmple);
    display.println("");

    display.drawLine(0, 50, 120, 50, WHITE);
    
    display.setTextSize(1);
    display.println("");
    display.println("");
    
    display.print("Less     OK     More");
    
    display.setCursor(0,0);
    display.display();
    display.clearDisplay();
}


void ScreenChooseDistanceDeMesure(){
  // Ecran ou on le nombre de tours par aquisition
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println("   Distance ");
    display.setTextSize(2);
    display.println("");
    display.print("  ");
    display.print(DistanceDeMesure);
    display.println(" m");

    display.drawLine(0, 50, 120, 50, WHITE);
    
    display.setTextSize(1);
    display.println("");
    display.println("");
    
    display.print("Less     OK     More");
    
    display.setCursor(0,0);
    display.display();
    display.clearDisplay();
}


void AfficheResultatLineaire(){
  display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println("   -   LINEAIRE   -");
    display.setCursor(0,9);
    
    display.setTextSize(2);
    display.print("A: ");
    
    
    float LesTempsEnX[NbrePtsMesure];
    float LesVitesseEnY[NbrePtsMesure];
    TempsAPartirStockage(Stockage,LesTempsEnX);
    VitesseAPartirStockage(Stockage,LesVitesseEnY);

    float MoyenneXi=moyenne(somme(LesTempsEnX));
    float MoyenneYi=moyenne(somme(LesVitesseEnY));
        
    float Lapente=pente(SommeProduitTab(LesTempsEnX,LesVitesseEnY),MoyenneXi,MoyenneYi,SommeCarreEcartAMoyenne(LesTempsEnX,MoyenneXi));
    display.println(Lapente,3);
    
    float OrdonneAlOrigine = MoyenneYi-Lapente*MoyenneXi;
    display.print("B: ");
    display.println(OrdonneAlOrigine,3);

    display.setTextSize(1);
    display.println("");
    display.print("R2 : ");
    float RCarre = pow(sqrt((Lapente*Lapente*SommeCarreEcartAMoyenne(LesTempsEnX,MoyenneXi))/(SommeCarreEcartAMoyenne(LesVitesseEnY,MoyenneYi))),2);  
    display.println(RCarre,5); // On affiche *100 pour avoir plus de chifffre significatif à l'affichage sur ecran (car que 2 chiffre significatis)
    
    display.drawLine(0, 50, 128, 50, WHITE);
    display.setCursor(0,55);
    display.print(" Exit  SaveFile   R2 ");
    display.setCursor(0,0);
    display.display();
    display.clearDisplay();
}

void AfficheResultatExponentielle(){
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println("   -EXPONENTIELLE -");
    display.setCursor(0,9);
    display.setTextSize(2);
    display.print("A: ");
    
    
    float LesTempsEnX[NbrePtsMesure];
    float LesVitesseEnY[NbrePtsMesure];
    
    TempsAPartirStockage(Stockage,LesTempsEnX);
    VitesseAPartirStockage(Stockage,LesVitesseEnY);
    VitesseToLNVitesse(LesVitesseEnY);
    
    float MoyenneXi=moyenne(somme(LesTempsEnX));
    float MoyenneYi=moyenne(somme(LesVitesseEnY));
        
    float Lapente=pente(SommeProduitTab(LesTempsEnX,LesVitesseEnY),MoyenneXi,MoyenneYi,SommeCarreEcartAMoyenne(LesTempsEnX,MoyenneXi));
    display.println(Lapente,4);
    
    float OrdonneAlOrigine = MoyenneYi-Lapente*MoyenneXi;
    display.print("B: ");
    display.println(exp(OrdonneAlOrigine),3);
    
    display.setTextSize(1);
    display.print("R2 : ");
    float RCarre = pow(sqrt((Lapente*Lapente*SommeCarreEcartAMoyenne(LesTempsEnX,MoyenneXi))/(SommeCarreEcartAMoyenne(LesVitesseEnY,MoyenneYi))),2);
    display.println(RCarre,5); 
    
    display.drawLine(0, 50, 128, 50, WHITE);
    display.setCursor(0,55);
    display.print(" New   SaveFile   R2 ");
    display.setCursor(0,0);
    display.display();
    display.clearDisplay();
}

void AfficherAvancement(){
    display.setTextSize(1);
    display.println("    - AQUISITION - ");
    display.setTextColor(WHITE);
    display.setTextSize(2);

    int PercentageAvancement = (NombrePassageAimant*100)/((NbrePtsMesure+1)*nbPassageAimentParAquisition);//(NbrePtsMesure*nbPassageAimentParAquisition);
    display.println("");
    display.print(" ");
    display.print(PercentageAvancement);
    display.print("% ");
    
    //display.print(Vitesse(Stockage[indX-2],Stockage[indX-1])*nbPassageAimentParAquisition); // On remultiplie les vitesse par le nombre de mesure par passage 
    //display.print(" / ");
    display.println(currentSpeed);//  On remultiplie les vitesse par le nombre de mesure par passage 
    display.setCursor(0,0);

    display.fillRect(0, 52, ((PercentageAvancement*128)/100), 8, WHITE);
    display.display();
    display.clearDisplay();
}

void GraphVitesse(){
  
  float LesTempsEnX[NbrePtsMesure];
  float LesVitesseEnY[NbrePtsMesure];
  TempsAPartirStockage(Stockage,LesTempsEnX);
  VitesseAPartirStockage(Stockage,LesVitesseEnY);

  float TempsMax =  LesTempsEnX[NbrePtsMesure-1];
  float VitesseMax = LesVitesseEnY[0];
  float VitesseMin = LesVitesseEnY[NbrePtsMesure-1];
  
  display.setCursor(0,0);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.print("            0;");
  display.println(VitesseMax,3);
 
  for (int i=0;i<(NbrePtsMesure);i++) {
      display.drawPixel(LesTempsEnX[i]*120/TempsMax, (50-(LesVitesseEnY[i]-VitesseMin)*50/(float)(VitesseMax-VitesseMin)), WHITE);

    }    
    display.setCursor(0,40);
    display.print(TempsMax);
    display.print(";");
    display.print(VitesseMin,3);
    
    display.drawLine(0, 50, 128, 50, WHITE);
    display.setCursor(0,55);
    display.print(" New   SaveFile   R2 ");
    display.setCursor(0,0);
    display.display();
    display.clearDisplay();
    
}


void ScreenDataSaved(char fileName[]){
    display.fillRect(0, 10, 128, 40, WHITE);
    display.setTextSize(2);
    display.setTextColor(BLACK);
    display.println("");
    display.println("   Saved");
    display.setTextSize(1);
    display.print("   Name : ");
    display.println(fileName);
    display.setCursor(0,0);
    display.display();
    display.clearDisplay();  
}

void ScreenDataAlreadySaved(){
  display.fillRect(0, 10, 128, 40, WHITE);
    display.setTextSize(2);
    display.setTextColor(BLACK);
    display.println("");
    display.println(" Already ");
    display.println(" Saved ");
    display.setCursor(0,0);
    display.display();
    display.clearDisplay(); 
}
