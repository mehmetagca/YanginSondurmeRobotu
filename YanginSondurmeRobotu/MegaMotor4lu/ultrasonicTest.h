#ifndef ULTRASONIC
#define ULTRASONIC

byte solTrig=3;
byte solEcho=2;

byte ortaTrig=5;
byte ortaEcho=4;

byte sagTrig=10;
byte sagEcho=11;
long sure;
long uzaklik;

double mesafe,mesafeSag,mesafeSol;

byte flameInput = A5;
byte yanginMesafe = 0;

//Method Prototip
int ultrasonicSensor(byte trig,byte echo);
int flameSensorControl();
int ultrasonicGelenDeger();

void setupUltra(){
  
  pinMode(solTrig, OUTPUT); /* trig pini çıkış olarak ayarlandı */
  pinMode(solEcho,INPUT); /* echo pini giriş olarak ayarlandı */

  pinMode(ortaTrig, OUTPUT); /* trig pini çıkış olarak ayarlandı */
  pinMode(ortaEcho,INPUT); /* echo pini giriş olarak ayarlandı */

  pinMode(sagTrig, OUTPUT); /* trig pini çıkış olarak ayarlandı */
  pinMode(sagEcho,INPUT); /* echo pini giriş olarak ayarlandı */

  pinMode(flameInput, INPUT);
  
  //Serial.begin(9600); /* Seri haberlesme baslatildi */

  mesafe=0,mesafeSag=0,mesafeSol=0;
  
}

int loopUltra()
{

//FlameBasi------------------------------
 /*yanginMesafe = flameSensorControl();
 
 switch (yanginMesafe) 
 {
 case 0: // A fire closer than 1.5 feet away.
 //Serial.println("** Close Fire **");
 break;
 case 1: // A fire between 1-3 feet away.
 //Serial.println("** Distant Fire **");
 break;
 case 2: // No fire detected.
 //Serial.println("No Fire");
 break;
 }
 delay(1); // delay between reads
 */
/*
  if(yanginMesafe == 0)
  {
      //Serial.println("DUR ve Atesi Sondur...");
      delay(100);
      return;//Bir sonraki adim
  }
*/
//FlameSonu---------------------------------



//Ultrasonic---------------------------------
  /**********SAĞ SOL ORTA************/
  //mesafeSag = ultrasonicSensor(sagTrig, sagEcho);
  //mesafeSol = ultrasonicSensor(solTrig, solEcho);
  //mesafe = ultrasonicSensor(ortaTrig, ortaEcho);

 return ultrasonicGelenDeger();
//UltrasonicSonu---------------------------------
}

int ultrasonicSensor(byte trig, byte echo)
{
  digitalWrite(trig, LOW); /* sensör pasif hale getirildi */
  delayMicroseconds(10);
  digitalWrite(trig, HIGH); /* Sensore ses dalgasının üretmesi için emir verildi */
  delayMicroseconds(10);
  digitalWrite(trig, LOW);  /* Yeni dalgaların üretilmemesi için trig pini LOW konumuna getirildi */ 
  sure = pulseIn(echo, HIGH); /* ses dalgasının geri dönmesi için geçen sure ölçülüyor */
  uzaklik= sure /29.1/2; /* ölçülen sure uzaklığa çevriliyor   //süre*0.034   */          
  if(uzaklik > 200)
    uzaklik = 200;
      
  //delay(25);
  return uzaklik;
}

int flameSensorControl()
{
 int sensorReading = analogRead(A5);
 // map the sensor range (four options):
 // ex: 'long int map(long int, long int, long int, long int, long int)'
 int range = map(sensorReading, 0, 1024, 0, 3);

 return range;
}
int ultrasonicGelenDeger()
{
  mesafe = ultrasonicSensor(ortaTrig, ortaEcho);
  mesafeSol = ultrasonicSensor(solTrig, solEcho);
  mesafeSag = ultrasonicSensor(sagTrig, sagEcho);
  
  /********SAĞSOL***********/
  if(mesafeSol < 110 && mesafeSag < 110 )
   {
      return 0;//DUR
   }
   
    /**********ORTA************/
  if(mesafe<90) //orta
  { 
    //mesafeSol = ultrasonicSensor(solTrig, solEcho);
    if(mesafeSol < 110)//sol
    {
        //mesafeSag = ultrasonicSensor(sagTrig, sagEcho);
        if(mesafeSag < 110)//sag
        {
            return 0;//DUR
        }
        else
          return 2;//SAG
    }
    else
    {
       return 3;//SOL
    }
  }
  else if(mesafeSol < 110)//sol
  {
      /********SOL***********/
    if(mesafeSag < 110)//sag
    {
      return 0;//DUR
    }
    else
    {
      return 2;//SAG
    }
  }
  else if(mesafeSag < 110)//sag
  {
    /********SAĞ***********/ 
    return 3;//SOL
  }
  else
  {
      return 1;//DUZ
  }
}

#endif // ULTRASONIC
