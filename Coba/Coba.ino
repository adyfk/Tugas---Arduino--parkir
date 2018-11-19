//#include <SoftwareSerial.h>
//const byte rxPin = 3;
//const byte txPin = 2;
//
//SoftwareSerial wifi (rxPin, txPin);
//
//void setup() {
//  Serial.begin(9600);   
//  wifi.begin(9600);
//  delay(2000);
//}
//
//void printResponse() {
//  while (wifi.available()) {
//    Serial.println(wifi.readStringUntil('\n')); 
//  }
//}
//void loop() {
//
//
//  delay(1000);
//  printResponse();
//
//  wifi.println("AT+CIPSTART=\"TCP\",\"ear-piercing-spring.000webhostapp.com\",80");
//  delay(1000);
//  printResponse();
//  String cmd = "GET /update.php?no=2&val=1 HTTP/1.1";
//  wifi.println("AT+CIPSEND="+ String(cmd.length()));
//  delay(1000);
//  wifi.println(cmd);
//  delay(1000);
//  wifi.println();
//  delay(1000);
//  printResponse();
//  delay(5000);
//}

#include <SoftwareSerial.h>

const byte rxPin = 3;
const byte txPin = 2;

SoftwareSerial wifi(rxPin, txPin);

unsigned long lastTimeMillis = 0;

void setup() {
  Serial.begin(9600);   
  wifi.begin(9600);
  delay(2000);
  Serial.println("Udah");
}

void loop() {
  if (millis() - lastTimeMillis > 5000) {
      Serial.println("Udah");
    lastTimeMillis = millis();
    wifi.println("AT+CIPMUX=1");
    while(!wifi.available());
    delay(100);
    wifi.println("AT+CIPSTART=4,\"TCP\",\"ear-piercing-spring.000webhostapp.com\",80");
    while(!wifi.available());
    delay(100);
    int idx=0,val=1;
    String cmd = "GET /update.php?no="+(String)(idx+1)+"&val="+(String)val+" HTTP/1.1\r\nHost:ear-piercing-spring.000webhostapp.com\r\nConnection: close";
    wifi.println("AT+CIPSEND=4," + String(cmd.length() + 4));
    while(!wifi.available());
    delay(100);
    wifi.println(cmd);
    while(!wifi.available());
    wifi.println("");
  }

  if (wifi.available()) {
    Serial.write(wifi.read());
  }

}
