#include <SPI.h>
#include <RH_NRF24.h>
//#include <Servo.h>
RH_NRF24 radio;
int mot1s=0;
int mot2s=0;
//Servo mot1;
//Servo mot2;
int trash;
bool state=1;
void setup() {
  Serial.begin(9600);
  //mot1.attach(5,1000,2000);
  //mot2.attach(6,1000,2000);
  //mot1.write(2000);
  //mot2.write(2000);
  if (radio.init())
    Serial.println("Initiation succesfull.Proceeding to set up channel");
  else ("intiation failed");
  delay(1000);
  if (radio.setChannel(1))
    Serial.println("Channel set up sucessfully. Setting communication rate @250kbps");
  delay(1000);
  //mot1.write(1000);
  //mot2.write(1000);
  if (radio.setRF(RH_NRF24::DataRate250kbps, RH_NRF24::TransmitPowerm18dBm))
    Serial.println("Rate set. Proceeding to establish connection");
  delay(1000);

}

void loop() {
  if (radio.waitAvailableTimeout(500)) {
    uint8_t message[RH_NRF24_MAX_MESSAGE_LEN];
    uint8_t s;
    radio.recv(message, &s);
    if (message[0] == 2) {
      trash=map(message[1],0,256,1000,2000);
      mot1s=trash;mot2s=trash;
      state=0;
    }
    if (message[0] == 1) {
      trash=message[1];
      if(state==0){
        if(trash<100){
          mot1s=mot1s+map(trash,128,0,0,500);

        }
        if(trash>156){
          mot2s=mot2s+map(trash,128,256,0,500);
        }
        state=1;
      }
    }
    if (message[0] == 3) {}
    if (message[0] == 4) {}
    if(mot1s>2000)
      mot1s=2000;
    if(mot2s>2000)
      mot2s=2000;
    Serial.print("mot1: ");Serial.print(mot1s);Serial.print("  mot2: ");Serial.println(mot2s);
  } 
  else
    Serial.println("Radio not detected. Please resolve.");
}
