int sinyalArtis(String dizi[] ){
int okunandeger = 0;
int oncekideger = 0;
int buyuk = 0 ;
int kucuk = 0 ;

  for(int i = 0; i < 10; i++)
  {
    Serial.print("i degeri : ");
    Serial.println(i);
    
    if( i > 0 ) {
      oncekideger = dizi[i-1].toInt();
      Serial.print("oncekideger : ");
      Serial.println(oncekideger);
    }

    okunandeger = dizi[i].toInt();  
    Serial.print("okunandeger : "); 
    Serial.println(okunandeger);

    if(oncekideger != 0){
       if(okunandeger > oncekideger ){
         buyuk++;
       }
       else
       {
         kucuk++;
       }
         
    }

    Serial.print("buyuk : ");
    Serial.println(buyuk);

    Serial.print("kucuk : ");
    Serial.println(kucuk);

    delay(2000);
 
  }
  //while(1); // Sonsuz döngü, program kilitlendi.

  if (buyuk > kucuk)
      return buyuk;
  else
      return kucuk; 

  
}


/*
void setup() {
  Serial.begin(9600);
}

void loop() {
 String dizi[10] = {"-11","-31","-50","-7","-49","-11","-13","-15","-17","-19"};
 
 sinyalArtis(dizi); 
}
*/