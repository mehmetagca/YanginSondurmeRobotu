#include <Servo.h>

#define flameAnalog1 A7
#define servoDigital 12

Servo myservomotor; // sınıfın bir örneğini aldık
const int sensorMin = 0; // sensor minimum
const int sensorMax = 1024; // sensor maximum
int derece;

void servoSetup(){
  myservomotor.attach(servoDigital);  // arduinonun 8. pinini çıkış yaptık.
  myservomotor.write(0);  // motora ilk 0.derecesinden başlaması komutunu verdik
}

byte isFireClose(){
  byte returnValue = 0; 
  int sensorReading = analogRead(flameAnalog1);
  int range = map(sensorReading, sensorMin, sensorMax, 0, 100);    
  Serial.print("range  : ");
  Serial.println(range);
  if(range < 4)
  {
    derece=90;  // 180 derecelik açı yap
    myservomotor.write(derece);
    returnValue = 1;
    //delay(10);   // her 1 derece açıyı 10 milisaniyede yapacak  
  }
  else{
    derece=0;
    myservomotor.write(derece);
    
  }
  return returnValue;//1:ates var, 0:ates yok
}

/*
#include "servoFlame.h"

void setup()
{
 servoSetup();
}

void loop()
{
  Serial.print("isFireClose() : ");
  Serial.print(isFireClose());
}
*/
