#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Wire.h>
//#include <Adafruit_SSD1306.h>
//#include <Adafruit_GFX.h>

// MAC envoi : 2c:f4:32:66:65:3d
//MAC reception : 8C:CE:4E:D5:55:02

// OLED display TWI address
#define OLED_ADDR   0x3C
//Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t mac[] = {0x8C, 0xCE, 0x4E, 0xD5, 0x55, 0x02};

#define NbrCapteur 9
float Capteurs[NbrCapteur] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&Capteurs, incomingData, sizeof(Capteurs));
  for (int i=0; i<NbrCapteur; i++)
    {
      Serial.print("Capteurs["); Serial.print(i);Serial.print("]: "); Serial.println(Capteurs[i]);
    }
  Serial.println();
}
void RAZCapteurs();
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);

  //display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  //display.clearDisplay();
  //display.display();
}

void loop() 
{
  RAZCapteurs();

/*
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();

  // affiche les pixel aux 4 coins   permet de voir quel est la taille de l'ecran 32 ou 64
  display.drawPixel(0, 0, WHITE);   // haut a gauche
  display.drawPixel(127, 0, WHITE); // haut droite
  display.drawPixel(0, 31, WHITE);  // bas gauche
  display.drawPixel(127, 31, WHITE);// bas droite

  // display a line of text
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  Serial.println("  ");
  for (int i=0; i<NbrCapteur; i++)
  {
    display.setCursor(0, 10);
    display.print("Capteurs["); display.print(i);display.print("]: "); display.print(Capteurs[i]);
    Serial.print("Capteurs["); Serial.print(i);Serial.print("]: "); Serial.println(Capteurs[i]);
    display.display();
    delay(1000);
    display.clearDisplay();
    display.display();
  }
}*/
void RAZCapteurs()    // remise a 0 des valeurs Capteurs
{
  for (byte j=0; j<NbrCapteur; j++)  // reinitialise toutes les valeurs des capteurs à 0
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