#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include "WiFi.h"
#include "esp_now.h"


int RXPin = 13;
int TXPin = 12;
 
int GPSBaud = 9600;

TinyGPSPlus gps;

SoftwareSerial gpsSerial(RXPin, TXPin);

uint8_t broadcastAddress[] = {0xB4, 0xE6, 0x2D, 0x8D, 0xE9,0x61};

typedef struct struct_message {
  float a;
  float b;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  delay(1000);
}


void setup(){
  Serial.begin(9600);
  gpsSerial.begin(GPSBaud);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}


void loop(){


while (gpsSerial.available() > 0)
if (gps.encode(gpsSerial.read()))
displayInfo();
 
  //SE EM 5 SEGUNDOS NÃO FOR DETECTADA NENHUMA NOVA LEITURA PELO MÓDULO,SERÁ MOSTRADO ESTA MENSGEM DE ERRO.
  if (millis() > 5000 && gps.charsProcessed() < 10){
    Serial.println("Sinal GPS não detectado");
    while(true);
}

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  if (result == ESP_OK) {
    Serial.println("Sent with success");
    Serial.println("");
    delay(2000);
  }
}

void displayInfo(){//FUNÇÃO RESPONSAVEL PELA LEITURA DOS DADOS

  if (gps.location.isValid()){//SE A LOCALIZAÇÃO DO SINAL ENCONTRADO É VÁLIDA, ENTÃO
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);//IMPRIME NA SERIAL O VALOR DA LATIDUE LIDA
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);//IMPRIME NA SERIAL O VALOR DA LONGITUDE LIDA
  
  }
  else{
    Serial.println("Não detectamos a localização");//SE NÃO HOUVER NENHUMA LEITURA, IMPRIME A MENSAGEM DE ERRO NA SERIAL
  }
  Serial.println();
  Serial.println();
  delay(1000);
}
