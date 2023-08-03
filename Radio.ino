#include<SPI.h>
#include<RH_NRF24.h>
RH_NRF24 plane;
int vx1=A0;
int vy1=A3;
int vx2=A2;
int vy2=A1;
int S1=A4;
int S2=A5;
int p1=4;
int p2=5;
int l1=2;
int l2=3;
int vval;
bool arm=false;
uint8_t message[2];

void setup() {
  Serial.begin(9600);
  pinMode(p1,INPUT);
  pinMode(p2,INPUT);
  pinMode(l1,OUTPUT);
  pinMode(l2,OUTPUT);
  if(plane.init())
  Serial.println("Initiation succesfull.Proceeding to set up channel");
  delay(1000);
  if(plane.setChannel(1))
  Serial.println("Channel set up sucessfully. Setting communication rate @250kbps");
  delay(1000);
  if(plane.setRF(RH_NRF24::DataRate250kbps , RH_NRF24::TransmitPowerm18dBm ));
  Serial.println("Rate set. Proceeding to establish connection");
  delay(1000);
  digitalWrite(l1,LOW);
  digitalWrite(l2,HIGH);
  while(!arm){
    if(analogRead(vy1)==0){
      if(digitalRead(S1)==1){
        arm=1;
        digitalWrite(l1,HIGH);
        digitalWrite(l2,LOW);
      }
    }
  }
}

void loop() {
  if(digitalRead(S1)==0){
      digitalWrite(l1,LOW);
      digitalWrite(l2,HIGH);
      arm=0;
      while(!arm){
        if(analogRead(vy1)==0){
          if(digitalRead(S1)==1){
            arm=1;
            digitalWrite(l1,HIGH);
            digitalWrite(l2,LOW);
          }
        }
      }
  }
  else{
  vval=map(analogRead(vx1),0,1024,0,255);
  message[0]=1;
  message[1]=vval;
  plane.send(message,sizeof(message));
  delay(5);
  vval=map(analogRead(vy1),0,1024,0,255);
  message[0]=2;
  message[1]=vval;
  plane.send(message,sizeof(message));
  delay(5);
  vval=map(analogRead(vx2),0,1024,255,0);
  message[0]=3;
  message[1]=vval;
  plane.send(message,sizeof(message));
  delay(5);
  vval=map(analogRead(vy2),0,1024,255,0);
  message[0]=4;
  message[1]=vval;
  plane.send(message,sizeof(message));
  delay(5);
  vval=digitalRead(S2);
  message[0]=6;
  message[1]=vval;
  plane.send(message,sizeof(message));
  delay(5);
  /*
  vval=digitalRead(S2);
  message[0]=6;
  message[1]=vval;
  plane.send(message,sizeof(message));
  vval=digitalRead(p1);
  message[0]=7;
  message[1]=vval;
  plane.send(message,sizeof(message));
  vval=digitalRead(p2);
  message[0]=8;
  message[1]=vval;
  plane.send(message,sizeof(message));
*/}
}
