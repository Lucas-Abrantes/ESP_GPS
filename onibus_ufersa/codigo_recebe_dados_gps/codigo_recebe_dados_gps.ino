  #include "WiFi.h"
#include "esp_now.h"
#include "heltec.h"

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  float a;
  float b;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println("");

  Serial.print("LAT: ");
  Serial.println(myData.a);
  Serial.print("LONG: ");
  Serial.println(myData.b);
  
}
 


void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  Heltec.begin(true /DisplayEnable Enable/, false /LoRa Enable/, true /Serial Enable/);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);

}
 
void loop() {

  Heltec.display -> clear();
  String latitude = String(myData.a, 6);
  String longitude = String(myData.b, 6);

  Heltec.display -> drawString(10, 0, "COORDENADAS:");
  
	Heltec.display -> drawString(15, 25, "LAT: " + latitude);
	Heltec.display -> display();

  Heltec.display -> drawString(15, 45, "LONG: " + longitude);
	Heltec.display -> display();

}