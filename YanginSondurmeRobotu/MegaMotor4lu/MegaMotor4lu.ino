#include "ultrasonicTest.h"
#include "queueFunctions.h"
#include "sinyalArtis.h"

#define BRAKE 0
#define CW    1
#define CCW   2
#define CS_THRESHOLD 15   // Definition of safety current (Check: "1.3 Monster Shield Example").



//MOTOR 1-2 SAG
//MOTOR 3-4 SOL

//MOTOR 1 CW-CCW
#define MOTOR_A1_PIN_R 22
#define MOTOR_B1_PIN_R 24

//MOTOR 2 CW-CCW
#define MOTOR_A2_PIN_R 23
#define MOTOR_B2_PIN_R 25

//MOTOR 3 CW-CCW
#define MOTOR_A1_PIN_L 52
#define MOTOR_B1_PIN_L 50

//MOTOR 4 CW-CCW
#define MOTOR_A2_PIN_L 53
#define MOTOR_B2_PIN_L 51



#define PWM_MOTOR_1_R 8
#define PWM_MOTOR_2_R 9

#define PWM_MOTOR_1_L 6
#define PWM_MOTOR_2_L 7



#define CURRENT_SEN_1_R A2
#define CURRENT_SEN_2_R A3

#define CURRENT_SEN_1_L A10
#define CURRENT_SEN_2_L A11



#define EN_PIN_1_R A0
#define EN_PIN_2_R A1

#define EN_PIN_1_L A8
#define EN_PIN_2_L A9



#define MOTOR_1 0
#define MOTOR_2 1

#define MOTOR_3 2
#define MOTOR_4 3

//*******FlameSensorTanıları**********
//const int sensorPin= 0;
//int smoke_level;

short usSpeed = 30;  //default motor speed
unsigned short usMotor_Status = BRAKE;

byte atesCikti = 1;

Queue<String> queue(10);
byte sonSinyalDegeri = -1;
 
void setup()
{
  setupUltra();
  
  pinMode(MOTOR_A1_PIN_R, OUTPUT);
  pinMode(MOTOR_B1_PIN_R, OUTPUT);
  pinMode(MOTOR_A1_PIN_L, OUTPUT);
  pinMode(MOTOR_B1_PIN_L, OUTPUT);
  

  pinMode(MOTOR_A2_PIN_R, OUTPUT);
  pinMode(MOTOR_B2_PIN_R, OUTPUT);
  pinMode(MOTOR_A2_PIN_L, OUTPUT);
  pinMode(MOTOR_B2_PIN_L, OUTPUT);


  pinMode(PWM_MOTOR_1_R, OUTPUT);
  pinMode(PWM_MOTOR_2_R, OUTPUT);
  pinMode(PWM_MOTOR_1_L, OUTPUT);
  pinMode(PWM_MOTOR_2_L, OUTPUT);


  pinMode(CURRENT_SEN_1_R, OUTPUT);
  pinMode(CURRENT_SEN_2_R, OUTPUT);
  pinMode(CURRENT_SEN_1_L, OUTPUT);
  pinMode(CURRENT_SEN_2_L, OUTPUT);


  pinMode(EN_PIN_1_R, OUTPUT);
  pinMode(EN_PIN_2_R, OUTPUT);
  pinMode(EN_PIN_1_L, OUTPUT);
  pinMode(EN_PIN_2_L, OUTPUT);
  

  Serial.begin(115200);              // Initiates the serial to do the monitoring 

  //For ESP
  Serial1.begin(115200);
  
  Serial.println("Begin motor control");
  Serial.println(); //Print function list for user selection
  Serial.println("Enter number for control option:");
  Serial.println("1. STOP");
  Serial.println("2. FORWARD");
  Serial.println("3. REVERSE");
  Serial.println("4. READ CURRENT");
  Serial.println("+. INCREASE SPEED");
  Serial.println("-. DECREASE SPEED");
  Serial.println();

  delay(5000);//TEST
}

void loop() 
{
  digitalWrite(EN_PIN_1_R, HIGH);
  digitalWrite(EN_PIN_2_R, HIGH);
  digitalWrite(EN_PIN_1_L, HIGH);
  digitalWrite(EN_PIN_2_L, HIGH);
  
  char user_input;
  int donus = -1;

  String rssi;
  char serialBuffer[10];
  unsigned long sure = millis();
  
  //while(Serial.available())
  while(true)//TEST
  {
    rssi = -199; //Default
    
    if(Serial1.available())
      {
        rssi = Serial1.readStringUntil('\n');
        Serial.print("Signal Value: ");
        Serial.println(rssi);
      }

    if(!rssi.equals("/1/\r") && atesCikti == 0)
      {
        Serial.println("Ates gelmedi!");
        delay(100);
        break;
      }
    else if(atesCikti == 0)
    {
      Serial.println("START...");
      atesCikti = 1;
      //delay(2000);
    }
    
    donus = loopUltra();
    Serial.println(donus);
    user_input = donus + '0';
    Serial.print("Donus : ");
    Serial.println(user_input);

    //Gelen rssi kuyruga ekleniyor.
    rssi.toCharArray(serialBuffer, 10);
    if(serialBuffer[0] == '-' )//&& !rssi.equals(-199))
    {
      Serial.print("Gelen rssi:");
      Serial.println(rssi);
      
      rssi[3] = '\0';
      queue.pushToQueue(String(rssi));//Bu ana kuyruk olacak, diger kuyruklara ekleme asagidaki if de yapılacak.
      
      Serial.println("Queue;");
      queue.printQueue();
    }
    else if(serialBuffer[0] == '+') // IP Info
    {
      Serial.print("Gelen rssi IP:");
      Serial.println(rssi);
      
      rssi[3] = '\0';

      //TODO gelen ip bilgisine uygun diziye, rssi degeri push edilecek.
    }

    if(millis() - sure > 3000)// 5sn
    {
      while(Serial.available())//Flush Serial Rx Buffer
        Serial.read();
      
      if(user_input == '1' && queue.count() > 0)//Engel yoksa
      {
        String stringArray[10];
        int i = 0;
      
        while(queue.count() > 0)//Queue to String Array
        {
          stringArray[i] = queue.pop();
        
          Serial.print(i);
          Serial.print("String:");
          Serial.print(stringArray[i]);
          
          if(queue.count() == 0)
            sonSinyalDegeri = stringArray[i].toInt();
            
          i++;
        }

        int sinyalArtisDegeri = sinyalArtis(stringArray); //0:azaliyor , 1:artiyor, -1:sinyal degerleri esit
        if(sinyalArtisDegeri == 1)
        {
          Serial.println("Sensore yaklasiliyor, duz devam et.");
          if(sonSinyalDegeri != -1)
            {
              if(sonSinyalDegeri > -55)//Dur
                {
                  Stop();
                  delay(10000);
                  sonSinyalDegeri = -1;
                }
            }
        }
        else if(sinyalArtisDegeri == 0)
        {
          Serial.println("Sensorden uzaklasiyor, donus yap.");
          TurnLeft();
          delay(3000);
          //if
          //sagadon
          //soladon
        }
        else
        {
          Serial.println("Sinyal degerleri esit! Donus yok.");
        }
        sure = millis();
      }
    }
    
    //user_input = '2';//TEST
    //user_input = Serial.read(); //Read user input and trigger appropriate function
    if (user_input =='0')
    {
       Stop();
    }
    else if(user_input =='1')
    {
      Forward();
      //Reverse();//TEST
    }
    else if(user_input =='9')
    {
      Reverse();
    }
    else if(user_input =='2')
    {
      TurnRight();
    }
    else if(user_input =='3')
    {
      TurnLeft();
    }
    else if(user_input =='+')
    {
      IncreaseSpeed();
    }
    else if(user_input =='-')
    {
      DecreaseSpeed();
    }
    else
    {
      Serial.println("Invalid option entered.");
    }   
  }
}

void Stop()
{
  Serial.println("Stop");
  usMotor_Status = BRAKE;
  motorGo(MOTOR_1, usMotor_Status, 0);
  motorGo(MOTOR_3, usMotor_Status, 0);
  
  motorGo(MOTOR_2, usMotor_Status, 0);
  motorGo(MOTOR_4, usMotor_Status, 0);
}

void Forward()
{
  Serial.println("Forward");
  usMotor_Status = CW;
  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_3, usMotor_Status, usSpeed);
  
  motorGo(MOTOR_2, usMotor_Status, usSpeed);
  motorGo(MOTOR_4, usMotor_Status, usSpeed);
}

void Reverse()
{
  Serial.println("Reverse");
  usMotor_Status = CCW;
  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_3, usMotor_Status, usSpeed);
  
  motorGo(MOTOR_2, usMotor_Status, usSpeed);
  motorGo(MOTOR_4, usMotor_Status, usSpeed);
}
void TurnRight()
{
   Serial.println("TurnRight");
  usMotor_Status = CW;
  motorGo(MOTOR_3, usMotor_Status, (usSpeed+50)%250);
  motorGo(MOTOR_4, usMotor_Status, (usSpeed+50)%250);
  usMotor_Status = CCW;
  motorGo(MOTOR_1, usMotor_Status, 0);
  motorGo(MOTOR_2, usMotor_Status, 0);
}
void TurnLeft()
{
   Serial.println("TurnLeft");
  usMotor_Status = CW;
  motorGo(MOTOR_1, usMotor_Status, (usSpeed+50)%250);
  motorGo(MOTOR_2, usMotor_Status, (usSpeed+50)%250);
  usMotor_Status = CCW;
  motorGo(MOTOR_3, usMotor_Status, 0);
  motorGo(MOTOR_4, usMotor_Status, 0);  
}



void IncreaseSpeed()
{
  usSpeed = usSpeed + 10;
  if(usSpeed > 255)
  {
    usSpeed = 255;
  }
  
  Serial.print("Speed +: ");
  Serial.println(usSpeed);

  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed);
  
  motorGo(MOTOR_3, usMotor_Status, usSpeed);
  motorGo(MOTOR_4, usMotor_Status, usSpeed);
}

void DecreaseSpeed()
{
  usSpeed = usSpeed - 10;
  if(usSpeed < 0)
  {
    usSpeed = 0;  
  }
  
  Serial.print("Speed -: ");
  Serial.println(usSpeed);

  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed);
  
  motorGo(MOTOR_3, usMotor_Status, usSpeed);
  motorGo(MOTOR_4, usMotor_Status, usSpeed);  
}

void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm)         //Function that controls the variables: motor(0 ou 1), direction (cw ou ccw) e pwm (entra 0 e 255);
{
  if(motor == MOTOR_1)
  {
    if(direct == CW)
    {
      digitalWrite(MOTOR_A1_PIN_R, LOW); 
      digitalWrite(MOTOR_B1_PIN_R, HIGH);
    }
    else if(direct == CCW)
    {
      digitalWrite(MOTOR_A1_PIN_R, HIGH);
      digitalWrite(MOTOR_B1_PIN_R, LOW);      
    }
    else
    {
      digitalWrite(MOTOR_A1_PIN_R, LOW);
      digitalWrite(MOTOR_B1_PIN_R, LOW);            
    }
    
    analogWrite(PWM_MOTOR_1_R, pwm); 
  }
  else if(motor == MOTOR_2)
  {
    if(direct == CW)
    {
      digitalWrite(MOTOR_A2_PIN_R, LOW);
      digitalWrite(MOTOR_B2_PIN_R, HIGH);
    }
    else if(direct == CCW)
    {
      digitalWrite(MOTOR_A2_PIN_R, HIGH);
      digitalWrite(MOTOR_B2_PIN_R, LOW);      
    }
    else
    {
      digitalWrite(MOTOR_A2_PIN_R, LOW);
      digitalWrite(MOTOR_B2_PIN_R, LOW);            
    }
    
    analogWrite(PWM_MOTOR_2_R, pwm);
  }
  else if(motor == MOTOR_3)
  {
    if(direct == CW)
    {
      digitalWrite(MOTOR_A1_PIN_L, LOW);
      digitalWrite(MOTOR_B1_PIN_L, HIGH);
    }
    else if(direct == CCW)
    {
      digitalWrite(MOTOR_A1_PIN_L, HIGH);
      digitalWrite(MOTOR_B1_PIN_L, LOW);      
    }
    else
    {
      digitalWrite(MOTOR_A1_PIN_L, LOW);
      digitalWrite(MOTOR_B1_PIN_L, LOW);            
    }
    
    analogWrite(PWM_MOTOR_1_L, pwm); 
  }
  else if(motor == MOTOR_4)
  {
    if(direct == CW)
    {
      digitalWrite(MOTOR_A2_PIN_L, LOW);
      digitalWrite(MOTOR_B2_PIN_L, HIGH);
    }
    else if(direct == CCW)
    {
      digitalWrite(MOTOR_A2_PIN_L, HIGH);
      digitalWrite(MOTOR_B2_PIN_L, LOW);      
    }
    else
    {
      digitalWrite(MOTOR_A2_PIN_L, LOW);
      digitalWrite(MOTOR_B2_PIN_L, LOW);            
    }
    
    analogWrite(PWM_MOTOR_2_L, pwm);
  }
}
/*
int FlameSensor{
  smoke_level= analogRead(sensorPin); //arduino reads the value from the smoke sensor
  Serial.println(smoke_level);//prints just for debugging purposes, to see what values the sensor is picking up
  if(smoke_level < 50){ //if smoke level is greater than 500, the buzzer will go off
    return 1;
    
}*/
