/*
 * Prog pour nano dédié a l'anemo
 */

#define nano_anemo 2                // adresse du nano anemo
#define pinAnemometre 0             // defini a0 pour anemo

// *** variables pour anemometre ***
float Capteurs_Anemo[2] = { 0, 0};  // trame pour le mega
enum {Moyenne_vent, Vent_max};      // 
float ValeurAnemometre = 0.0;      // valeur lue sur la pin A0
float VoltageConversionConstant = 0.0; // valeur converti en tension
const float voltageMin = 0.4; // Mininum output voltage from anemometer in mV.
float VitesseVent = 0.0;
const float voltageMax = 5.0; // Maximum output voltage from anemometer in mV.
const float VitesseventMax = 32.5; // Wind speed in meters/sec corresponding to maximum voltage   116km/h
float DernierVentMax = 0.0;
unsigned int i = 1;
double Cumul_moy = 0.0;
  
// **** includes ***********
#include <Arduino.h>
#include <Wire.h>
//#include <variables.h>

void MesuresAnemometre();
void Calculs_vitesse();
void Calcul_moyenne();
void Affiche();
//void requestEvent();


void setup() 
{
  Serial.begin(115200);   // demarre la liaison série affichage PC
  Wire.begin(nano_anemo); // Rejoindre le bus I2C (Pas besoin d adresse pour le maitre)
  pinMode(pinAnemometre, INPUT);  // sera brancher l'anemometre
 // Wire.onRequest(requestEvent);
}

void loop() 
{
  MesuresAnemometre();  // prise mesures 
  delay(1000);
  Calculs_vitesse();
  Calcul_moyenne();
  Affiche();
}
void MesuresAnemometre()       
{
  Serial.println("************* Debut du programme **********************");
  
  ValeurAnemometre = analogRead(pinAnemometre);
  Serial.print("sensorValue: "); Serial.println(ValeurAnemometre); 
  ValeurAnemometre = (ValeurAnemometre * 5.0)/1024;  // ne pas utiliser la fonction map() car elle renvoie un entier
  Serial.print("valeur: "); Serial.println(ValeurAnemometre); 
}
void Calculs_vitesse()
{
  if (ValeurAnemometre <= voltageMin) // si tension convertie < ou = a 0.4
  {
    VitesseVent = 0;  // pas de vent  
  }
  else
  {
    VitesseVent = (((ValeurAnemometre - voltageMin)*VitesseventMax) /(voltageMax - voltageMin))*3.6;  // il y a du vent
    
    // permet de sauvegarder le vent max
    DernierVentMax = max(DernierVentMax, VitesseVent); 
    Capteurs_Anemo[Vent_max] = DernierVentMax;             
  }
  Serial.print("VitesseVent: "); Serial.println(VitesseVent); 
  Serial.print("DernierVentMax: "); Serial.println(DernierVentMax);
  delay(1000);
}
void Calcul_moyenne()
{
  Cumul_moy = VitesseVent;
  if (i == 1)
  {
  Cumul_moy = VitesseVent;
  }
  else
  {
  Cumul_moy = Cumul_moy / 2;
  Serial.print("i: "); Serial.println(i);
  Serial.print("Cumul_moy: "); Serial.println(Cumul_moy);
  
  }
  i++;
  Capteurs_Anemo[Moyenne_vent] = Cumul_moy;
}
void Affiche()
{
  Serial.print("Capteurs_Anemo[Vent_max]: "); Serial.println(Capteurs_Anemo[Vent_max]);
  Serial.print("Capteurs_Anemo[Moyenne_vent]: "); Serial.println(Capteurs_Anemo[Moyenne_vent]);
}
