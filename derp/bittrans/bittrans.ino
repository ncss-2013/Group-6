// Search for a low to high transition and read the bit into 3 10-long byte arrays

#include "bitstream.h"
#define SENSOR_PIN 2
#define REFV 700
#define DELAY (1000/40)

int prev = -1;
int now = 0;

void ordPrint(ordinate inord){
  Serial.print("DEG");
  Serial.print(inord.deg);
  Serial.print("MIN");
  Serial.print(inord.min);
  Serial.print("SEC");
  Serial.print(inord.sec);
}

void printBitStream(bitStream ibs){
  Serial.print("LAT");
  ordPrint(ibs.getLatitude());
  Serial.print("LON");
  ordPrint(ibs.getLongitude());
  Serial.print("ALT");
  Serial.print(ibs.getAltitude());
  Serial.print("TMP");
  Serial.print(ibs.getTemperature());
  Serial.print("END");
}

void printBin(char *iarr, int len){
	for (int i = 0; i < len; i++){
    	for (int j = 0; j < 8; j++)
    	{
    		char cbyte = iarr[i];
    		char mask = (1<<j);
    		bool bit = !!(cbyte&mask);
    		Serial.print(bit);
    	}
    	Serial.print(" ");
    }
    Serial.println("");
}

void setup(){
	Serial.begin(9600);
}

void loop(){
	char ibstream[8] = {0};
	bitStream obstream;
	// looks for a low to high transition in the signal
	while (analogRead(SENSOR_PIN) < REFV);
	delay((DELAY*3)/2);
	long stime = millis();
	// for (int i = 0; i < 3; i++){
	for (int j = 0; j < 8; j++){
		for (int k = 0; k < 8; k++){
			int blv = (analogRead(SENSOR_PIN) > REFV)?1:0;
			ibstream[j] |= (blv << k);
			while (millis()-stime < DELAY);
			stime = millis();
		}
	}
	// }
	printBin(ibstream, 8);
	obstream.fromBitStream(ibstream);
	// boolean strue = (checkSum(ibstream) == 0);
	// if (strue)
	printBitStream(obstream);

}

/*unsigned char xorCheck(unsigned char *arr, int n)
{
	unsigned char chk = arr[0];
	for (int i= 1; i < n; i++)
		chk ^= arr[i];
	return chk;
}*/
