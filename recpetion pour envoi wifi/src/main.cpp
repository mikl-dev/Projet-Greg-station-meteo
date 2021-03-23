#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Wire.h>

#define NbrCapteur 9 
#define Nbr_octets_a_recevoir 4

// REPLACE WITH RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xBC, 0xDD, 0xC2, 0x24, 0x64, 0x9C};  //8C:CE:4E:D5:55:02 d1 mini     bc:dd:c2:24:64:9c  uno d1


float Capteurs[NbrCapteur] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

void reception();
void printInfo();
void Envoi_Wifi();
void receiveEvent();
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

void setup() 
{
  Serial.begin(115200);         // Initilaise une liaison série
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() 
{
  reception();
  printInfo();
  Envoi_Wifi();

  digitalWrite(LED_BUILTIN, LOW);
}
void reception()
{
    if (Serial.available())
    {
      /*for (int j = 0; j<NbrCapteur ; j++)  // compte le nombre de valeurs a recevoir
      {
          Serial.readBytes((byte *)&Capteurs[j], sizeof Capteurs[j]);       
      }*/
      
      Serial.readBytes((byte *)&Capteurs[0], sizeof Capteurs[0]); 
      Serial.readBytes((byte *)&Capteurs[1], sizeof Capteurs[1]); 
      Serial.readBytes((byte *)&Capteurs[2], sizeof Capteurs[2]); 
      Serial.readBytes((byte *)&Capteurs[3], sizeof Capteurs[3]); 
      Serial.readBytes((byte *)&Capteurs[4], sizeof Capteurs[4]); 
      Serial.readBytes((byte *)&Capteurs[5], sizeof Capteurs[5]); 
      Serial.readBytes((byte *)&Capteurs[6], sizeof Capteurs[6]); 
      Serial.readBytes((byte *)&Capteurs[7], sizeof Capteurs[7]); 
      Serial.readBytes((byte *)&Capteurs[8], sizeof Capteurs[8]); 
    }
}
void printInfo()
{
    Serial.println("  ");
    for (int i=0; i<NbrCapteur; i++)
    {
      Serial.print("Capteurs["); Serial.print(i);Serial.print("]: "); Serial.println(Capteurs[i]);
    }
  delay(1000);
}  
void Envoi_Wifi()
{
  digitalWrite(LED_BUILTIN, HIGH);
  // Send message via ESP-NOW
  esp_now_send(broadcastAddress, (uint8_t *) &Capteurs, sizeof(Capteurs));
}

  
