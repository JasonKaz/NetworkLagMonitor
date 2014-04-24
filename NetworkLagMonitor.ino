#include <SPI.h>
#include <Ethernet.h>
#include <ICMPPing.h>

//Config Stuff
int goodThresh = 150;
int warningThresh = 350;
int pingDelay = 2000;

byte mac[] = {0x90, 0xA2, 0xDA, 0x0E, 0x04, 0x7B};
IPAddress ip;
byte pingAddress[] = {74, 125, 226, 38}; //Google

SOCKET pingSocket = 0;

char buffer[256];
ICMPPing ping(pingSocket, (uint16_t)random(0, 255));

int time;

void setup(){
  Serial.begin(9600);
  if (Ethernet.begin(mac) == 1){
    ip = Ethernet.localIP();
    
    Serial.print("Connection established: ");
    Serial.println(ip);
    
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
  }else{
    Serial.println("Could not establish connection");
    for (;;) ; 
  }
}

void loop(){
  ICMPEchoReply echoReply = ping(pingAddress, 4);
  
  if (echoReply.status == SUCCESS){
    time = getPingTime(echoReply);
    
    updateLEDs(time);
    
    Serial.print("Ping: ");
    Serial.println(time);
  }else{
     sprintf(buffer, "Echo request failed; %d", echoReply.status);
     Serial.println(buffer);
  }
  
  delay(pingDelay);
}

int getPingTime(ICMPEchoReply reply){
  return millis() - reply.data.time;
}

void updateLEDs(int time){
  if (time < goodThresh){
   digitalWrite(3, HIGH); 
   digitalWrite(4, LOW);
   digitalWrite(5, LOW);
  }else if (time < warningThresh){
   digitalWrite(3, LOW); 
   digitalWrite(4, HIGH);
   digitalWrite(5, LOW);
  }else{
   digitalWrite(3, LOW); 
   digitalWrite(4, LOW);
   digitalWrite(5, HIGH);
  }
}
