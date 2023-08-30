#include "WiFi.h"
#include "esp_now.h"
#include <TinyGPS++.h>
#include <SoftwareSerial.h>


int RXPin = 13;
int TXPin = 12;
 
int GPSBaud = 9600;

TinyGPSPlus gps;  


SoftwareSerial gpsSerial(RXPin, TXPin);

// REPLACE WITH YOUR RECEIVER MAC Address
//ENDERECO DO ESP COM O DISPLAY FUNCIONANDO
uint8_t broadcastAddress[] = {0xB4, 0xE6, 0x2D, 0x8D, 0xE9,0x61};

// Structure example to send data
// Must match the receiver structure
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
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(9600);
  gpsSerial.begin(GPSBaud);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
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
 
void loop() {
   while (gpsSerial.available() > 0){
     if (gps.encode(gpsSerial.read())){
       if (gps.location.isValid()){
         myData.a = gps.location.lat();
         myData.b = gps.location.lng();

       }
       else{
         Serial.println("Não detectamos a localização");
       }  
     }
   }
   Serial.print("Latitude: ");
   Serial.println(gps.location.lat());
    Serial.print("Longitude: ");
   Serial.println(gps.location.lng());
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  if (result == ESP_OK) {
    Serial.println("Sent with success");
    Serial.println("");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(2000);
}