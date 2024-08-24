/*
 * Rui Santos 
 * Complete Project Details https://randomnerdtutorials.com
 */

/*                 
    ssgps RX -> 2
    ssgps TX -> 1
*/


#include <SoftwareSerial.h>
#include <TinyGPS++.h>
//#include <TinyGPS.h>

//char GPSmess = 'N';
//char GSMmess;
bool GPSmess = false;
bool isGPSreceived = false;

// Initialize gps
TinyGPSPlus gps;

// The serial connection to the ssgps module
SoftwareSerial ssgps(3, 4); 
SoftwareSerial ssgsm(6, 7);

const unsigned long gps_eventInterval = 4000;
unsigned long gps_previousTime = 0;
const unsigned long gps_eventDuration = 1300;

float gpslat, gpslon;

void setup(){
  Serial.begin(9600);
  ssgsm.begin(9600);
  ssgps.begin(9600);


} 

void loop(){
  getGps();
}

void getGps(){
  ssgps.listen();
      if (ssgps.available() > 0){
        if(gps.encode(ssgps.read())){
          if(gps.location.isValid()){
                sendSms();
          } else{
              Serial.println(F("- location: INVALID"));
              ssgps.println(F("- location: INVALID"));
          }
        }
      }
}

void sendSms(){
  delay(1000);
  ssgsm.listen();
  delay(500);
  ssgsm.println("AT");
  ssgsm.println("AT+CMGF=1\r");
  delay(500)
  ssgsm.println("AT+CMGS=\"+639178813944\"\r");
  delay(500);
  
  Serial.print(F("- latitude: "));
  Serial.println(gps.location.lat());
  Serial.print(F("- longitude: "));
  Serial.println(gps.location.lng());

  ssgsm.print(F("- latitude: "));
  ssgsm.println(gps.location.lat(), 6);
  ssgsm.print(F("- longitude: "));
  ssgsm.println(gps.location.lng(), 6);
  
  ssgsm.print("Long:");
  Serial.print("Long:");
  ssgsm.println(gps.location.lng(), 6);
  Serial.println(gps.location.lng(), 6);
  Serial.println("Message Sent!");
  ssgsm.write(0x1A);
  delay(500);
  isGPSreceived = false;
}
