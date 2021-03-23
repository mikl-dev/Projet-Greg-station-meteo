#include <Wire.h>
# define nano_pluie 1 // adresse du nano_pluie

float compteur = 0.1;
void requestEvent();


void setup() {
  Serial.begin(115200);
  Wire.begin(nano_pluie);
  Wire.onRequest(requestEvent);

}

void loop() 
{
  compteur += 0.1;
  Serial.println(compteur);  
  delay(100);

}
void requestEvent()  
{
   Serial.println("J'envoi...");
   //Serial.print("Wire.available: "); Serial.println(Wire.available());
   
   //if (Wire.available())
   //{
       
     for (char i = 0; i < 4; i++) 
     {
       Wire.write(((char*)&compteur)[i]);
       Serial.println(compteur);
     }
   //}
}
