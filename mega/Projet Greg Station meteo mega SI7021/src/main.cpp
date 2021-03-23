//*******************************************************************************
//***                                                                         ***
//***                                                                         ***
//***                        STATION METEO                                    ***
//***                                                                         ***
//***                                                                         ***
//*******************************************************************************
//========================================================================================================================╗  
//║  1 - RAZCapteurs()   --> mise a 0 des valeurs Capteurs                                                                 ║
//║  2 - Pluvio()        --> toutes les 5 min : interruption (D3 du mega)sur nano (D2) pluvio pour aller chercher la valeur║ 
//║  3 - Test_Capteurs() --> tester si les capteurs I2C sont connectés sinon reboot                                        ║
//║  4 - Prise_Mesures() --> prise de mesure et remplissage de la variable tableau                                         ║ alt186
//║              └> envoi etat haut (D2 mega) vers nano anemo
//║              └> remplissage dans Capteurs
//║              └> demande a tous les capteurs I2C leurs infos et remplis Capteurs
//║              └> TestDefautCapteurI2C() si les capteurs I2C ne repondent pas alors 9999
//║              └> affiche tous les Capteurs sur port serie
//║  5 - EnvoiCapteurs() --> envoi de la trame Capteurs sans fils
//║  6 - Affiche_Mesures() --> affiche tous les capteurs sur Oled 
//========================================================================================================================╝

// **** includes ***********
#include <Arduino.h>
#include <avr/wdt.h>           // necessaire pour reset logiciel
#include <Wire.h>              // I2C
#include <SparkFun_Si7021_Breakout_Library.h>  // librairie pour le capteur SI7021   temps abris

// **** Define ****
#define NbrCapteur 9    // definis le  nombre de capteurs
#define nano_pluie 1   // adresse du nano pluie
#define nano_anemo 2   // adresse du nano pluie
#define wemos_envoi 3  // pour envoie WIFI
#define Nbr_octets_a_envoyer 4

// Creation des instances
Weather sensor;   //creer une instance du SI7021

// **** Definition des variables
                           // *** tableau a envoyer ****
                           // [0][1][2][3][4][5][6][7][8]
float Capteurs[NbrCapteur] = { 0, 0, 0, 0, 0, 0, 0, 0, 0};  // initialize tableau a X valeurs defini à 0 
enum { Temp_Abris, Hygro_Abris, P_Atmospherique, UV, Moyenne_vent, Vent_max, Temp_Pyro, Luxmetre, Pluviometre};  // donne un nom de "variable" a chaques elements du tableau Capteurs 
char* Noms_Capteurs[NbrCapteur] = { "Temp_Abris :", "Hygro_Abris :", "P_Atmospherique :", "UV :", "Moyenne vent :", "Vent max :", "Temp Pyro :", "Luxmetre :" ,"Pluviometre :"};  // tableau texte qui permet d'afficher quel capteur est affiché
char* Symbols[NbrCapteur] = { " C", " %", " hPa", " ", " km/h", " km/h", " C", " Lux", " mm/m3"}; // permet d'afficher "C" pour une temp, etc...
unsigned int ValeurErreur = 9999;    // contient 9999 pour dire qu'il y a une erreur de transmission

int i = 0;
int j = 0;  // pour renitialiser le tableau
float humidity = 0;
float temp = 0;

float compteurPluie = 0;
const float precision = 0.03;

// Declaration des fonctions
void RAZCapteurs();
void Pluvio();
void Anemo();
void EnvoiI2C();
void getWeather();
void printInfo();


void setup() 
{
  Serial.begin(115200);   // demarre la liaison série affichage PC
  Wire.begin();          // initialise la liaison I2C 
  sensor.begin();         // initialise le SI7021
}

void loop() 
{
  Serial.println("");
  Serial.println(" ........ DEMARRAGE DU PROGRAMME .......");

  RAZCapteurs();       // 1
  getWeather(); 
  Pluvio();            // 2
  Anemo();
  EnvoiI2C();
  printInfo();

  delay(1000);
}

void RAZCapteurs()    // remise a 0 des valeurs Capteurs
{
  for (j=0; j<NbrCapteur; j++)  // reinitialise toutes les valeurs des capteurs à 0
  {
    Capteurs[j]=0;
  }
  /*Serial.print("Capteurs complets avant remplissage: ");
    for (int i=0; i<9; i++)
    {
      Serial.print(Capteurs[i]);
      Serial.print(" ,");
    }
    Serial.println("");
  */  
}
void Pluvio()         // interroger nano pluvio 
{
  // le mega demande au pluvio de lui envoyer 4 octets
  Wire.requestFrom(nano_pluie, 4);

  // si la liaison I2C existe, on recois 4 octets et la sotcke dans la trame
  while (Wire.available() > 0)
  {
    //Serial.print("Wire.available: "); Serial.println(Wire.available());
    for (char i = 0; i < 4; i++)
    {
      ((char*)&compteurPluie)[i] = Wire.read();
    }
    //Serial.print("Compteur de pluie brut reçu: ");Serial.println(compteurPluie);  // affiche la variable reçue           
    Capteurs[Pluviometre] = compteurPluie * precision; // calcule pour le mettre sous en mm/m3
  }
  Wire.flush();
  Wire.endTransmission(nano_pluie);  
    Serial.print("Wire.available apres pluie: "); Serial.println(Wire.available());
}
void Anemo()
{
  //Serial.println("Je suis dans anemo.");
  // le mega demande au pluvio de lui envoyer 4 octets
  Wire.requestFrom(nano_anemo, 8);

  //Serial.print("Wire.available: "); Serial.println(Wire.available());
  // si la liaison I2C existe, on recois 4 octets et la sotcke dans la trame
  while (Wire.available() > 0)
  {
    for (char i = 0; i < 4; i++)
    {
      ((char*)&Capteurs[Moyenne_vent])[i] = Wire.read();
    }
    //Serial.print("Moyenne_vent reçu: ");Serial.println(Capteurs[Moyenne_vent]);  // affiche la variable reçue           
    for (char i = 0; i < 4; i++)
    {
      ((char*)&Capteurs[Vent_max])[i] = Wire.read();
    }
    //Serial.print("Vent_max reçu: ");Serial.println(Capteurs[Vent_max]);
  }  
   Wire.flush();
   Wire.endTransmission(nano_anemo);
     Serial.print("Wire.available apres anemo: "); Serial.println(Wire.available());
}
void getWeather()
{
  // Prise de mesure de l'humidité et stockage
  humidity = sensor.getRH();
  Capteurs[Hygro_Abris] = humidity; // stocke l'humidité dans le tableau

  // Prise de mesure de la temperature et stockage
  temp = sensor.getTemp();
  Capteurs[Temp_Abris] = temp; // stocke la temprature dans le tableau 
}
void EnvoiI2C()
{
  //float tampon=0;
  //Serial.print("Wire.available vers envoi avant begintransmission: "); Serial.println(Wire.available());
  //Serial.print("lecture bit en plus: "); Serial.println(((char*)&tampon)[1] = Wire.read());
  Serial.print("Wire.available vers envoi avant begintransmission: "); Serial.println(Wire.available());
  Wire.beginTransmission(wemos_envoi); 
  Serial.print("Wire.available vers envoi apres begintransmission: "); Serial.println(Wire.available());
  for (int j = 0; j<NbrCapteur ; j++)  // compte le nombre de valeurs a recevoir
  {
    for (int i = 0; i<Nbr_octets_a_envoyer ; i++)  // compte pour recevoir 4o
    {
       Wire.write( ((char*)&Capteurs)[i] );  // envoi de 32 octets composants le float Capteurs
    }
  }

  Wire.endTransmission(nano_anemo); 

}
void printInfo()
{
  Serial.print("Capteurs complets: ");
    for (int i=0; i<9; i++)
    {
      Serial.print(Capteurs[i]);
      Serial.print(" ,");
    }
    Serial.println("");
}

