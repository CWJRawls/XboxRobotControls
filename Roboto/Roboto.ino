/* All of the lovely Yun libraries */
//#include <YunClient.h>
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
//#include <YunServer.h>

#define PACKETTRACK
#define THROTTLE 10
#define MAXPASSES 100

BridgeUDP udp;
int8_t leftVal = 0;
int8_t rightVal = 0;
bool buttonVal = false;
uint8_t loopsSinceLast = 0;


void setup() {

  Serial.begin(9600);
  pinMode(13, OUTPUT);

  //turn off built-in LED while establishing the bridge
  digitalWrite(13, LOW);

  Bridge.begin();

  udp.begin(8000);

  //turn the LED back on when done
  digitalWrite(13, HIGH);

}

void loop() {
  // put your main code here, to run repeatedly:
  int packetSize = udp.parsePacket();

  if(packetSize) {
    //Serial.print("Incoming Packet\t|\t");
    //create a container
    char* buf = (char*)malloc(sizeof(char) * 255);

    //fill it
    int len = udp.read(buf, 255);

    if(len > 0) {
      buf[len] = 0;
    }

    /* Uncomment to see contents of incoming packets in the Arduino IDE Serial Monitor
    Serial.print("Contents:\t");

    for(int i = 0; i < len; i++) {
      int8_t val = (int8_t)buf[i];
      Serial.print(val);
      Serial.print("\t");
    }

    Serial.println();
    */

    leftVal = (len > 19) ? (int8_t)buf[19] : leftVal; //only update when data received
    rightVal = (len > 23) ? (int8_t)buf[23] : rightVal;
    buttonVal = (len > 27) ? ((buf[27]) ? true : false) : buttonVal; 

    free(buf);

    #ifdef PACKETTRACK
    loopsSinceLast = 0;
    #endif
  }

/* Uncomment to dump current state via USB Serial into Arduino IDE
  Serial.print("Current Values:\t");
  Serial.print(leftVal);
  Serial.print("\t");
  Serial.print(rightVal);
  Serial.print("\t");
  Serial.println(buttonVal);
*/

  #ifdef PACKETTRACK

  delay(THROTTLE);
  loopsSinceLast++;

  if(loopsSinceLast == MAXPASSES) {
    leftVal = 0;
    rightVal = 0;
    buttonVal = false;
  }

  #endif
}
