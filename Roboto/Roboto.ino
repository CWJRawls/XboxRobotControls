

/* All of the lovely Yun libraries */
#include <BridgeUdp.h>
#include <Console.h>
#include <Bridge.h>
#include <BridgeServer.h>
#include <Mailbox.h>
#include <BridgeSSLClient.h>
#include <HttpClient.h>
#include <FileIO.h>
#include <Process.h>
#include <BridgeClient.h>
#include <Servo.h>

#define ARMPIN 13
#define SPEEDPIN 9
#define TURNPIN 11

#define CLEARBUFFER //clears excess data from buffer, comment out if undesired

BridgeUDP udp;
int speedVal = 0;
int turnVal = 0;
bool buttonVal = false;
uint8_t loopsSinceLast = 0;
Servo dir;
Servo turns;


void setup() {
  
  pinMode(8, OUTPUT);
  pinMode(SPEEDPIN, OUTPUT);
  pinMode(TURNPIN, OUTPUT);
  pinMode(10, OUTPUT);
  dir.attach(SPEEDPIN);
  turns.attach(TURNPIN);

  //turn off built-in LED while establishing the bridge
  digitalWrite(13, LOW);

  Bridge.begin();

  udp.begin(8000);

  pinMode(ARMPIN, OUTPUT);

  //turn the LED back on when done
  digitalWrite(13, HIGH);

}

void loop() {
  int packetSize = udp.parsePacket();

  if(packetSize) {
    
    digitalWrite(13, HIGH); //turn on LED for network input
    
    //create a container
    char* buf = (char*)malloc(sizeof(char) * 255);

    //fill it
    int len = udp.read(buf, 255);

    if(len > 0) {
      buf[len] = 0;
    }

    if(len > 19) {
      char* tmp = buf + 19; //grab the correct byte for the value in packet
      speedVal = *tmp;
    }

    if(len > 23) { 
      char* tmp = buf + 23; //grab the correct byte for the value in packet
      turnVal = *tmp;
    }

    //if there is data for the button press, update the value
    buttonVal = (len > 27) ? ((buf[27]) ? true : false) : buttonVal; 

    //get rid of the buffer when done
    free(buf);

    //check if there is more data in the network buffer and read it off
    #ifdef CLEARBUFFER
      if(udp.available() > 0) {
        char* btemp = (char*) malloc(sizeof(char) * udp.available());
        udp.read(btemp, udp.available();
        free(btemp);
      }
    #endif
    
    
    digitalWrite(13, LOW); //turn LED off when done
  }

  int l1 = map(speedVal, -127, 127, 1000, 2000);
  int r = map(turnVal, -127, 127, 1000, 2000);
  dir.writeMicroseconds(l1);
  turns.writeMicroseconds(r);
  digitalWrite(ARMPIN, (buttonVal) ? HIGH : LOW);
  delay(1);

}
