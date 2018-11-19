#include <SoftwareSerial.h>
#include <Servo.h>
#define max_jar 9 //Maksimal Jarak
#define psin A0  //Pin servo
#define psout A1
SoftwareSerial wifi(3,2);
Servo svin;
Servo svout;
int pin=10,pout=11;   //pin Push  Button
int tP[3]={4,6,8},eP[3]={5,7,9}; //Ubah Trigger dan echo 
typedef struct{
  int sts;
  int temp;
}sensor;
sensor jr[3];
void init(sensor x){
  x.sts=0;
  x.temp=0;
}
long dur;
void printResponse() {
  while (wifi.available()) {
    Serial.println(wifi.readStringUntil('\n')); 
  }
}
void kirim(int idx,int val){
    Serial.println("Kirim");
    wifi.println("AT+CIPSTART=\"TCP\",\"ear-piercing-spring.000webhostapp.com\",80");
    if(wifi.find("Error")){
      delay(100);
      kirim(idx,val);
    }else{
      String cmd = "GET /update.php?no="+(String)(idx+1)+"&val="+(String)val+" HTTP/1.1\r\nHost:ear-piercing-spring.000webhostapp.com\r\nConnection: close\r\n\r\n";
      wifi.println("AT+CIPSEND="+String(cmd.length()));
      if(wifi.find(">")){
        wifi.print(cmd);
      }
      delay(700);
      wifi.println("AT+CIPCLOSE");
      printResponse();
    }
    
}
int jarak(int x){
  digitalWrite(tP[x], LOW);
  delayMicroseconds(2);
  digitalWrite(tP[x], HIGH);
  delayMicroseconds(10);
  digitalWrite(tP[x], LOW);
  x=pulseIn(eP[x], HIGH) / 58.2;
  return x;   
}
void sv(int in,int bk){
  int st,stp;
    if(bk==1){
      st=90;
      stp=0;
    }else{
      st=0;
      stp=90;
    }  
  do{
    (in==1) ? svin.write(st):svout.write(st);                                 
    (bk==1) ? st--:st++;
    delay(15);
  }while((bk==1) ? (st>=stp):(st<=stp) );
}
void setup(){
  Serial.begin(9600);
  delay(1000);
  pinMode(pin,INPUT_PULLUP);
  pinMode(pout,INPUT_PULLUP);
  svin.attach(psin);
  svout.attach(psout);
  wifi.begin(9600);
  wifi.println("AT+RST");
  delay(5000);
  for(int x=0;x<3;x++){
    init(jr[x]);
    pinMode(tP[x], OUTPUT);
    pinMode(eP[x], INPUT); 
  }
  Serial.println("Start");
}
int lop=0,jml=0,z;
void loop(){  
//  z=jarak(lop);
//  if(z < max_jar){
//      jr[lop].temp++;
//      if(jr[lop].temp==5 && jr[lop].sts==0){
//        jr[lop].sts=1;
//        kirim(lop,1);
//        delay(200);
//      }
//  }else if(jr[lop].sts==1 && jr[lop].temp==0){
//        jr[lop].sts=0;
//        Serial.println("Kluar");
//        kirim(lop,0);
//        delay(200);
//  }else{
//      jr[lop].temp=0;
//  }
  if(digitalRead(pin)==LOW && jml<2){
    jml++;
    Serial.println("Buka");
    sv(1,1); //in dan buka
    delay(1500);
    sv(1,0);
  }
  if(digitalRead(pout)==LOW){
    jml--;
    Serial.println("Tutup");
    sv(0,0);
    delay(1500);
    sv(0,1); 
  }
  if(jr[lop].temp>=10){
    jr[lop].temp-=4;
  }
  if(lop==0){
    lop=0;    
  }else{
    lop++;
  }
}
