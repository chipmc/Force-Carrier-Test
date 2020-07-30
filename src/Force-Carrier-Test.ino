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

// Particle Product definitions
PRODUCT_ID(10709);                                  // Boron Connected Counter Header
PRODUCT_VERSION(37);
#define DSTRULES isDSTusa
char currentPointRelease[6] ="37.02";

SYSTEM_MODE(SEMI_AUTOMATIC);                        // This will enable user code to start executing automatically.
SYSTEM_THREAD(ENABLED);                             // Means my code will not be held up by Particle processes.


char carrierModeStr[16] = "Automatic";                      // Default mode is Automatic
int carrierMode = 0;                                        // Default settings - automatic carrier selection



void setup() {

  Particle.connect();

  Particle.variable("carrierMode",carrierModeStr);
  Particle.variable("Version",currentPointRelease);

  Particle.function("setCarrierMode",setCarrierMode);

}


void loop() {


}

void carrierSelectionMode() {
  Cellular.on();
  Cellular.disconnect();

  if (carrierMode == 0) {
    Cellular.command("AT+COPS=2\r\n");                      // First de-register from the network
    Cellular.command("AT+COPS=0\r\n");                      // Automatic mode
  }
  else if (carrierMode == 1) {
    Cellular.command("AT+COPS=2\r\n");                      // First de-register from the network
    Cellular.command("AT+COPS=4,2,\"310410\"\r\n");         // Manually connect to AT&T, and fallback to automatic mode if failed
  }
  else if (carrierMode == 2) {
    Cellular.command("AT+COPS=2\r\n");                      // First de-register from the network
    Cellular.command("AT+COPS=4,2,\"310260\"\r\n");         // Manually connect to T-mobile, and fallback to automatic mode if failed
  }
  else if (carrierMode == 3) {
    Cellular.command("AT+COPS=2\r\n");                      // First de-register from the network
    Cellular.command("AT+COPS=4,2,\"63514\"\r\n");         // Manually connect to Airtel, and fallback to automatic mode if failed
  }
  else if (carrierMode == 4) {
    Cellular.command("AT+COPS=2\r\n");                      // First de-register from the network
    Cellular.command("AT+COPS=4,2,\"63510\"\r\n");         // Manually connect to MTN/RwandaCell, and fallback to automatic mode if failed
  }
  else if (carrierMode == 5) {
    Cellular.command("AT+COPS=2\r\n");                      // First de-register from the network
    Cellular.command("AT+COPS=4,2,\"63513\"\r\n");         // Manually connect to TIGO, and fallback to automatic mode if failed
  }
}

int setCarrierMode(String command)                          // Function to force sending data in current hour
{
  if (command == "0")
  {
    carrierMode = 0;
    strcpy(carrierModeStr,"Automatic");
    if (Particle.connected()) Particle.publish("Mode","Carrier Selection Mode to Automatic", PRIVATE);
  }
  else if (command == "1")
  {
    carrierMode = 1;
    strcpy(carrierModeStr,"AT&T");
    if (Particle.connected()) Particle.publish("Mode","Carrier Forced to AT&T", PRIVATE);
  }
  else if (command == "2")
  {
    carrierMode = 2;
    strcpy(carrierModeStr,"T-Mobile");
    if (Particle.connected()) Particle.publish("Mode","Carrier Forced to T-Mobile", PRIVATE);
  }
  else if (command == "3")
  {
    carrierMode = 3;
    strcpy(carrierModeStr,"Airtel");
    if (Particle.connected()) Particle.publish("Mode","Carrier Forced to Airtel", PRIVATE);
  }
  else if (command == "4")
  {
    carrierMode = 4;
    strcpy(carrierModeStr,"MTN");
    if (Particle.connected()) Particle.publish("Mode","Carrier Forced to MTN/RwandaCell", PRIVATE);
  }
  else if (command == "5")
  {
    carrierMode = 5;
    strcpy(carrierModeStr,"TIGO");
    if (Particle.connected()) Particle.publish("Mode","Carrier Forced to TIGO", PRIVATE);
  }
  else return 0;
      
  disconnectFromParticle();
  carrierSelectionMode();
  connectToParticle();

  return 1;
}

bool connectToParticle() {
  Cellular.on();
  Particle.connect();
  // wait for *up to* 5 minutes
  for (int retry = 0; retry < 300 && !waitFor(Particle.connected,1000); retry++) {
    Particle.process();
  }
  if (Particle.connected()) return 1;                               // Were able to connect successfully
  else return 0;                                                    // Failed to connect
}

bool disconnectFromParticle()                                     // Ensures we disconnect cleanly from Particle
{
  Particle.disconnect();
  waitFor(notConnected, 15000);                                   // make sure before turning off the cellular modem
  Cellular.off();
  return true;
}

bool notConnected() {                                             // Companion function for disconnectFromParticle
  return !Particle.connected();
}