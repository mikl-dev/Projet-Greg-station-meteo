/*
 * Prog pour nano dédié a l'anemo
 */

// **** includes ***********
#include <Arduino.h>
#include <Wire.h>
# define nano_anemo 2 // adresse du nano anemo

float Capteurs_Anemo[2] = { 0, 0};
enum {Moyenne_vent, Vent_max};

float ValeurAnemometre = 0.0;  // valeur lue sur la pin A0
float VoltageConversionConstant = 0.0; // valeur convertie en tension
const float voltageMin = 0.4; // Mininum output voltage from anemometer in mV.
float VitesseVent = 0.0;
const float voltageMax = 5.0; // Maximum output voltage from anemometer in mV.
const float VitesseventMax = 32.5; // Wind speed in meters/sec corresponding to maximum voltage   116km/h
float DernierVentMax = 0.0;
unsigned int i = 1;
float somme = 0;  
int nbr_iteration = 1;
float moy = 0;

void MesuresAnemometre();
void Calculs_vitesse();
void Calcul_moyenne();
void Affiche();
void requestEvent();

void setup() 
{
  Serial.begin(115200);   // demarre la liaison série affichage PC
  pinMode(A0, INPUT);  // sera branché l'anemometre
  Wire.begin(nano_anemo);
  Wire.onRequest(requestEvent);
}

void loop() 
{
  MesuresAnemometre();  // prise mesures 
  Calculs_vitesse();
  Calcul_moyenne();
  Affiche();
}
void MesuresAnemometre()       
{
  Serial.println("************* Debut du programme **********************");
  
  ValeurAnemometre = analogRead(A0);
  //Serial.print("sensorValue: "); Serial.println(ValeurAnemometre); 
  VoltageConversionConstant = (ValeurAnemometre * 5.0)/1024;  // ne pas utiliser la fonction map() car elle renvoie un entier
  //Serial.print("valeur: "); Serial.print(ValeurAnemometre); Serial.println("   --> sur 5.00V max");
}
void Calculs_vitesse()
{
  if (VoltageConversionConstant <= voltageMin) // si tension convertie < ou = à 0.4V
  {
    VitesseVent = 0;  // vent nul 
  }
  else
  {
    VitesseVent = (((VoltageConversionConstant - voltageMin)*VitesseventMax) /(voltageMax - voltageMin))*3.6;  // il y a du vent
   
    // permet de sauvegarder le vent max
    DernierVentMax = max(DernierVentMax, VitesseVent); 
    Capteurs_Anemo[Vent_max] = DernierVentMax;             
  }
  //Serial.print("VitesseVent: "); Serial.println(VitesseVent); 
  
  delay(1000);
}
void Calcul_moyenne()
{
  somme = somme + VitesseVent;
  moy = somme / nbr_iteration;
  //Serial.print("nbr_iteration: "); Serial.println(nbr_iteration);
  nbr_iteration++;
  
 
  //Serial.print("somme: "); Serial.println(somme); 
  
  //Serial.print("moy: "); Serial.println(moy); 
  
  //Serial.print("VentMax: "); Serial.println(DernierVentMax);
  
  i++;
  Capteurs_Anemo[Moyenne_vent] = moy;
}
void Affiche()
{
  Serial.print("Capteurs_Anemo[Vent_max]: "); Serial.println(Capteurs_Anemo[Vent_max]);
  Serial.print("Capteurs_Anemo[Moyenne_vent]: "); Serial.println(Capteurs_Anemo[Moyenne_vent]);
}
void requestEvent()  
{
  //Serial.println("J'envoi...");
  //Serial.print("Wire.available: "); Serial.println(Wire.available());
   
  for (char i = 0; i < 4; i++) 
  {
    Wire.write(((char*)&Capteurs_Anemo[Moyenne_vent])[i]);
    //Serial.println("MoyVent: "); Serial.println(Capteurs_Anemo[Moyenne_vent]);
  }
  for (i = 0; i < 4; i++) 
  {
   Wire.write(((char*)&Capteurs_Anemo[Vent_max])[i]);
   //Serial.println("MaxVent: "); Serial.println(Capteurs_Anemo[Vent_max]);
  }
}











/*
// *** variables pour anemometre ***
float Capteurs_Anemo[2] = { 0, 0};
enum {Moyenne_vent, Vent_max};
char* Noms_Capteurs[2] = {"Moyenne vent: ", "Vent max: "};
char* Symbols[2] = {"Km/h", "Km/h"};

const int NbeMaxMesure = 300; // 300 mesures sur 5 min
uint8_t i = 0;  // permet de compter le nombre de prise de mesure
float TotalAnemometre = 0;
float ValeurAnemometre[NbeMaxMesure] = {0};
float MoyVent = 0.0;
float MaxVent = 0.0;
float voltageMin = .4; // Mininum output voltage from anemometer in mV.
float VitesseVentMin = 0.0; // Wind speed in meters/sec corresponding to minimum voltage
float voltageMax = 5.0; // Maximum output voltage from anemometer in mV.
float VitesseventMax = 32.5; // Wind speed in meters/sec corresponding to maximum voltage   116km/h
float Vanemocalc = 0.0;
float VitesseVent = 0.0;
float sensorVoltage = 0.0; //Variable that stores the voltage (in Volts) from the anemometer being sent to the analog pin
float ValeurMaxTampon = 0.0; // permet de comparer les valeurs du tableau pour trouver la valeur max

#define pinAnemometre 0   // defini a0 pour anemo

void MesuresAnemometre();
void CalculsAnemo();
void Save();
void requestEvent();
void CalculsAnemo();

void setup() 
{
  Serial.begin(115200);   // demarre la liaison série affichage PC
  Wire.begin(nano_anemo); // Rejoindre le bus I2C (Pas besoin d adresse pour le maitre)
  pinMode(pinAnemometre, INPUT);  // sera bracher l'anemometre
  Wire.onRequest(requestEvent);
}

void loop() 
{
  MesuresAnemometre();  // prise de 300 mesures sur 5 min
  CalculsAnemo();
  Save();
}
  
void MesuresAnemometre()       
{
  // integrer i  a ValeurAnemometre pour les 300 mesures
  Serial.println("******************************************************");
  // Stock 300 valeurs a 1s d'interval
  for (i=0;i<NbeMaxMesure; i++)   
  {
    ValeurAnemometre[i] = analogRead(pinAnemometre);   // 1 mesure toutes les secondes                                
    Serial.print(i); Serial.print("     "); Serial.println(ValeurAnemometre[i]);
    //delay (1000);  // 1 prise se de mesure par seconde
  }
}

void CalculsAnemo()
{
  Serial.println("je suis dans calcul");
  for (i=0; i<NbeMaxMesure; i++)
  {
   Vanemocalc = (ValeurAnemometre[i] * 5)/1024; // convertie la valeur en tension 
   Serial.print("Vanemocalc: "); Serial.println(Vanemocalc);
  } 
  if (Vanemocalc <= voltageMin) // si tension convertie < ou = a 0.4
  {
    VitesseVent = 0;            // pas de vent
    i += 1;    // comptabilise la prise de mesure              
  }
  else
  {
    VitesseVent = (Vanemocalc - voltageMin)/((voltageMax - voltageMin)*3.6);
    i += 1;    // comptabilise la prise de mesure
  }
  Serial.print("VitesseVent: "); Serial.println(VitesseVent); 
  //******* permet de definir la valeur max du tableau ***********        
  if (ValeurMaxTampon != VitesseVent)
  {
   ValeurMaxTampon = max(ValeurMaxTampon, VitesseVent);
   MaxVent = ValeurMaxTampon;
  }
  else
  {
    ValeurMaxTampon = 0;
    MaxVent = ValeurMaxTampon;
  }      
  Serial.print("Vent max: "); Serial.println(MaxVent);
     
  // a revoir
  TotalAnemometre = TotalAnemometre + VitesseVent;  // somme de toutes les valeurs du tableau
  if (VitesseVent != 0)    // il y a du vent
  {
    MoyVent = TotalAnemometre / (i); // moyenne du tableau    
  }
  else
  {
    MoyVent = 0;       
  }     
  Serial.print("moyenne Vent :");
  Serial.println(MoyVent); 
  Serial.println("");
}

void Save()
{
  Capteurs_Anemo[Moyenne_vent] = MoyVent;   
  Capteurs_Anemo[Vent_max] = MaxVent;
}

void requestEvent()  
{
  Serial.println("J'envoi...");
  Serial.print("Wire.available: "); Serial.println(Wire.available());
   
  for (char i = 0; i < 4; i++) 
  {
    Wire.write(((char*)&Capteurs_Anemo[Moyenne_vent])[i]);
    Serial.println("MoyVent: "); Serial.println(Capteurs_Anemo[Moyenne_vent]);
  }
  for (i = 0; i < 4; i++) 
  {
   Wire.write(((char*)&Capteurs_Anemo[Vent_max])[i]);
   Serial.println("MaxVent: "); Serial.println(Capteurs_Anemo[Vent_max]);
  }
}
*/
