int sinyalArtis(String dizi[])
{
	int okunandeger = 0;
	int oncekideger = 0;
	int buyuk = 0;
	int kucuk = 0;

	for (int i = 0; i < 10; i++)
	{
		if (i > 0) {
			oncekideger = dizi[i - 1].toInt();
			Serial.print("oncekideger : ");
			Serial.print(oncekideger);
		}

		okunandeger = dizi[i].toInt();
		//Serial.print("okunandeger : "); 
		//Serial.println(okunandeger);

		if (oncekideger != 0 || okunandeger !=0) {
			if (okunandeger > oncekideger) {
				buyuk++;
			}
			else if (okunandeger < oncekideger)
			{
				kucuk++;
			}
		}
	}
	//while(1); // Sonsuz döngü, program kilitlendi.

  Serial.print("\nbuyuk : ");
  Serial.println(buyuk);

  Serial.print("kucuk : ");
  Serial.println(kucuk);

	if (buyuk > kucuk)
		return 1; // return 1 : sinyal artıyor
	else if(kucuk > buyuk) 
		return 0; // return 0 : sinyal azalıyor
  else
    return -1; // return -1 : sinyal değerleri eşit     
}

double randBit(){
    double r=((double)rand())/((double)RAND_MAX);
    return r>0.5?1:0;
}

/*
  String stringArray[10];
  int i = 0;

  while(queue.count() > 0){
	stringArray[i] = queue.pop();

	Serial.print(i);
	Serial.print("String:");
	Serial.print(stringArray[i]);

	i++;
  }

  Serial.println(sinyalArtis(stringArray));
*/
