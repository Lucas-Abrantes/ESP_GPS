#include <TinyGPS++.h>
#include <SoftwareSerial.h>
 

int RXPin = 13;
int TXPin = 12;
 
int GPSBaud = 9600;
 
//CRIANDO UM O BJETO PARA COMUNICAR COM A BIBLIOTECA
TinyGPSPlus gps;
 
// CRIANDO UMA PORTA SERIAL gpsSerial PARA CONVERSAR COM MÓDULO
SoftwareSerial gpsSerial(RXPin, TXPin);
 
void setup()
{
Serial.begin(9600);
 
// INICIA A PORTA SERIAL DO SOFTWARE NO BAUD PADRÃO DO GPS, COMO DETERMINAMOS ACIMA:9600
gpsSerial.begin(GPSBaud);
}
 
void loop()
{
// TODA VEZ QUE FOR LIDA UMA NOVA SENTENÇA NMEA, CHAMAREMOS A FUNÇÃO displayInfo() PARA MOSTRAR OS DADOS NA TELA
while (gpsSerial.available() > 0)
if (gps.encode(gpsSerial.read()))
displayInfo();
 
//SE EM 5 SEGUNDOS NÃO FOR DETECTADA NENHUMA NOVA LEITURA PELO MÓDULO,SERÁ MOSTRADO ESTA MENSGEM DE ERRO.
if (millis() > 5000 && gps.charsProcessed() < 10)
{
Serial.println("Sinal GPS não detectado");
while(true);
}
}
 
void displayInfo()//FUNÇÃO RESPONSAVEL PELA LEITURA DOS DADOS
{
if (gps.location.isValid())//SE A LOCALIZAÇÃO DO SINAL ENCONTRADO É VÁLIDA, ENTÃO
{
Serial.print("Latitude: ");
Serial.println(gps.location.lat(), 6);//IMPRIME NA SERIAL O VALOR DA LATIDUE LIDA
Serial.print("Longitude: ");
Serial.println(gps.location.lng(), 6);//IMPRIME NA SERIAL O VALOR DA LONGITUDE LIDA
 
}
else
{
Serial.println("Não detectamos a localização");//SE NÃO HOUVER NENHUMA LEITURA, IMPRIME A MENSAGEM DE ERRO NA SERIAL
}
 

 
Serial.println();
Serial.println();
delay(1000);
}