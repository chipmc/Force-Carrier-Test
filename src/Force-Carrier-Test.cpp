/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/chipmc/Documents/Maker/Particle/Utilities/Force-Carrier-Test/src/Force-Carrier-Test.ino"
/*
 * Project Force-Carrier-Test
 * Description: Test to see if we can force one carrier or another on our devices.
 * Author: Chip McClelland
 * Date: 7-30-20
 */

/*  For use in setting the carriers for our Rwanda sensors
Data from: https://www.mcc-mnc.com

MCC	MNC	ISO	Country	      Country Code	Network
310	410	us	United States	1	            AT&T Wireless Inc
310	260	us	United States	1	            T-Mobile

635	14	rw	Rwanda	      250	          Airtel
635	10	rw	Rwanda	      250	          MTN/Rwandacell
635	13	rw	Rwanda	      250	          TIGO

*/

// This section is to make it easier to integrate this code into the production code after this Test
// For general use, you can omit all of this and set your EEMPROM address to a value that makes sense to you
// ***********************************************************
// Particle Product definitions
void setup();
void loop();
void carrierSelectionMode(byte mode);
int setCarrierMode(String command);
bool connectToParticle();
#line 25 "/Users/chipmc/Documents/Maker/Particle/Utilities/Force-Carrier-Test/src/Force-Carrier-Test.ino"
PRODUCT_ID(10709);                                  // Boron Connected Counter Header
PRODUCT_VERSION(37);
#define DSTRULES isDSTusa
char currentPointRelease[6] ="37.02";

byte carrierModeAddr = 0x55;
// ***********************************************************


SYSTEM_MODE(SEMI_AUTOMATIC);                        // This will enable user code to start executing automatically.
SYSTEM_THREAD(ENABLED);                             // Means my code will not be held up by Particle processes.

char carrierModeStr[16] = "Automatic";              // Default mode is Automatic
byte carrierMode = 0;                               // Default settings - automatic carrier selection

void setup() {

  Particle.variable("carrierMode",carrierModeStr);
  Particle.variable("Version",currentPointRelease);

  Particle.function("setCarrierMode",setCarrierMode);

  carrierMode = EEPROM.read(carrierModeAddr);       // Load Carrier Mode from EEPROM and implement it

  carrierSelectionMode(carrierMode);

  connectToParticle();
}


void loop() {

}

void carrierSelectionMode(byte mode) {
  Cellular.on();
  Cellular.disconnect();

  switch (mode) {
    case 0:
      strcpy(carrierModeStr,"Automatic");
      Cellular.command("AT+COPS=2\r\n");                      // First de-register from the network
      Cellular.command("AT+COPS=0\r\n");                      // Automatic mode
    break;

    case 1:
      strcpy(carrierModeStr,"AT&T");
      Cellular.command("AT+COPS=2\r\n");                      // First de-register from the network
      Cellular.command("AT+COPS=4,2,\"310410\"\r\n");         // Manually connect to AT&T, and fallback to automatic mode if failed
    break;

    case 2:
      strcpy(carrierModeStr,"T-Mobile");
      Cellular.command("AT+COPS=2\r\n");                      // First de-register from the network
      Cellular.command("AT+COPS=4,2,\"310260\"\r\n");         // Manually connect to T-mobile, and fallback to automatic mode if failed
    break;

    case 3:
      strcpy(carrierModeStr,"Airtel");
      Cellular.command("AT+COPS=2\r\n");                      // First de-register from the network
      Cellular.command("AT+COPS=4,2,\"63514\"\r\n");         // Manually connect to Airtel, and fallback to automatic mode if failed
    break;

    case 4:
      strcpy(carrierModeStr,"MTN/Rwandacell");
      Cellular.command("AT+COPS=2\r\n");                      // First de-register from the network
      Cellular.command("AT+COPS=4,2,\"63510\"\r\n");         // Manually connect to MTN/RwandaCell, and fallback to automatic mode if failed
    break;

    case 5:
      strcpy(carrierModeStr,"TIGO");
      Cellular.command("AT+COPS=2\r\n");                      // First de-register from the network
      Cellular.command("AT+COPS=4,2,\"63513\"\r\n");         // Manually connect to TIGO, and fallback to automatic mode if failed
    break;

    default:
      EEPROM.write(carrierModeAddr,0);                      // Bad carrier code - go to default - Auto
      strcpy(carrierModeStr,"Default");
    break;
  }
}

int setCarrierMode(String command)                          // Function to force sending data in current hour
{
  if (command == "0")
  {
    EEPROM.write(carrierModeAddr,0); 
    if (Particle.connected()) Particle.publish("Mode","Carrier Selection Mode to Automatic - Resetting", PRIVATE);
  }
  else if (command == "1")
  {
    EEPROM.write(carrierModeAddr,1); 
    if (Particle.connected()) Particle.publish("Mode","Carrier Forced to AT&T - Resetting", PRIVATE);
  }
  else if (command == "2")
  {
    EEPROM.write(carrierModeAddr,2); 
    if (Particle.connected()) Particle.publish("Mode","Carrier Forced to T-Mobile - Resetting", PRIVATE);
  }
  else if (command == "3")
  {
    EEPROM.write(carrierModeAddr,3); 
    if (Particle.connected()) Particle.publish("Mode","Carrier Forced to Airtel - Resetting", PRIVATE);
  }
  else if (command == "4")
  {
    EEPROM.write(carrierModeAddr,4); 
    if (Particle.connected()) Particle.publish("Mode","Carrier Forced to MTN/RwandaCell - Resetting", PRIVATE);
  }
  else if (command == "5")
  {
    EEPROM.write(carrierModeAddr,5); 
    if (Particle.connected()) Particle.publish("Mode","Carrier Forced to TIGO - Resetting", PRIVATE);
  }
  else return 0;
  delay(2000);                // Give the Particle Publish a second or two to go through
  System.reset();
  return 1;
}

bool connectToParticle() {
  Particle.connect();
  // wait for *up to* 5 minutes
  for (int retry = 0; retry < 300 && !waitFor(Particle.connected,1000); retry++) {
    Particle.process();
  }
  if (Particle.connected()) return 1;                               // Were able to connect successfully
  else return 0;                                                    // Failed to connect
}
