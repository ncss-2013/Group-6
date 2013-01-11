// Search for a low to high transition and read the bit into 3 10-long byte arrays

#define SENSOR_PIN 2
#define REFV 700

int prev = -1;
int now = 0;
unsigned char ibstream[3][10];

void setup(){
	Serial.begin(9600);
}

void loop(){
	static bitStream obstream;
	static bitStream obstream2;
	static bitStream obstream3;
	prev = (analogRead(SENSOR_PIN)>REFV)?1:0;
	now = (analogRead(SENSOR_PIN)>REFV)?1:0;
	if (prev == 0 && now == 1){
		for (int i = 0; i < 3; i++){
			for (int j = 0; j < 10; j++){
				for (int k = 0; k < 8; k++){
					int blv = (analogRead(SENSOR_PIN) > REFV)?1:0;
					ibstream[i][j] |= (blv << k);
					delaymicroseconds(333);
				}
			}
		}
		boolean strue[3];
		for (int i = 0; i < 3; i++)
			strue[i] = (xorCheck == ibstream[i][9]); 
		if (strue[0])
			obstream.fromBitStream(ibstream[0]);
		if (strue[1])
			obstream2.fromBitStream(ibstream[1]);
		if (strue[2])
			obstream3.fromBitStream(ibstream[2]);
	}
}

unsigned char xorCheck(unsigned char *arr, int n)
{
	unsigned char chk = arr[0];
	for (int i= 1; i < n; i++)
		chk ^= arr[i];
	return chk;
}