// Search for a low to high transition and read the bit into 3 10-long byte arrays

#include "bitstream.h"
#define SENSOR_PIN 2
#define REFV 700

int prev = -1;
int now = 0;

void setup(){
	Serial.begin(9600);
}

void loop(){
	unsigned char ibstream[8];
	bitStream obstream;
	// looks for a low to high transition in the signal
	prev = (analogRead(SENSOR_PIN)>REFV)?1:0;
	now = (analogRead(SENSOR_PIN)>REFV)?1:0;
	if (prev == 0 && now == 1){
		// for (int i = 0; i < 3; i++){
		for (int j = 0; j < 8; j++){
			for (int k = 0; k < 8; k++){
				int blv = (analogRead(SENSOR_PIN) > REFV)?1:0;
				ibstream[i][j] |= (blv << k);
				delaymicroseconds(333);
			}
		}
		// }
		obstream.fromBitStream(ibstream);
		boolean strue = (obstream.getChecksum() == checkSum(ibstream));
		if (strue)
			printBitStream(obstream);
	}
}

/*unsigned char xorCheck(unsigned char *arr, int n)
{
	unsigned char chk = arr[0];
	for (int i= 1; i < n; i++)
		chk ^= arr[i];
	return chk;
}*/