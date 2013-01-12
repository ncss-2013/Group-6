// Search for a low to high transition and read the bit into 3 10-long byte arrays

#include "bitstream.h"
#include "checksum.h"
#define SENSOR_PIN 2
#define REF_PIN 1
#define DELAY (1000/40)

void ordPrint(ordinate inord){
	Serial.print(inord.deg);
	Serial.print("|");
	Serial.print(inord.min);
	Serial.print("|");
	Serial.print(inord.sec);
}

void printBitStream(bitStream ibs){
  Serial.print("~LAT");
  ordPrint(ibs.getLatitude());
  Serial.print("|LON");
  ordPrint(ibs.getLongitude());
  Serial.print("|ALT");
  Serial.print(ibs.getAltitude());
  Serial.print("|END");
}

void printBin(char *iarr, int len){
	for (int i = 0; i < len; i++){
    	for (int j = 0; j < 8; j++){
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
	int REFV = analogRead(REF_PIN);
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
	/*ordinate lat1,long1;
	unsigned int alt1,temp1;
	char chksum;
	alt1 = random(64000+1);
	temp1 = random(63+1);
	lat1.deg = random(4+1)-2;
	lat1.min = random(60+1);
	lat1.sec = random(600+1);
	long1.deg = random(4+1)-2;
	long1.min = random(60+1);
	long1.sec = random(600+1);
	chksum = random(9+1);
	bitStream tbs;
	tbs.toBitStream(lat1,long1,alt1,temp1,chksum);
	obstream.fromBitStream(tbs.getStream());*/
	// printBin(ibstream, 8);
	obstream.fromBitStream(ibstream);
	boolean strue = (checkSum(ibstream) == 0);
	if (strue)
		printBitStream(obstream);
	// delay(1000);
}

/*unsigned char xorCheck(unsigned char *arr, int n)
{
	unsigned char chk = arr[0];
	for (int i= 1; i < n; i++)
		chk ^= arr[i];
	return chk;
}*/