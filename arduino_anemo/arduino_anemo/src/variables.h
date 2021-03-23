#define nano_anemo 2 // adresse du nano anemo
// *** variables pour anemometre ***
float Capteurs_Anemo[2] = { 0, 0};
enum {Moyenne_vent, Vent_max};
char* Noms_Capteurs[2] = {"Moyenne vent: ", "Vent max: "};
//char* Symbols[2] = {"Km/h", "Km/h"};
/*
const int NbeMaxMesure = 250; // 300 mesures sur 5 min
uint8_t i = 0;  // permet de compter le nombre de prise de mesure
float TotalAnemometre = 0;
float ValeurAnemometre[NbeMaxMesure] = {0};
float MoyVent = 0.0;
float MaxVent = 0.0;
float voltageMin = 0.4; // Mininum output voltage from anemometer in mV.
float voltageMax = 5.0; // Maximum output voltage from anemometer in mV.
const float VitesseventMax = 32.5; // Wind speed in meters/sec corresponding to maximum voltage   116km/h
float Vanemocalc = 0.0;
float VitesseVent = 0.0;
float sensorVoltage = 0.0; //Variable that stores the voltage (in Volts) from the anemometer being sent to the analog pin
float ValeurMaxTampon = 0.0; // permet de comparer les valeurs du tableau pour trouver la valeur max
*/
#define pinAnemometre 0   // defini a0 pour anemo

float ValeurAnemometre = 0;  // valeur lue sur la pin A0



