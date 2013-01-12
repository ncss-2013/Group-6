
#include <EEPROM.h>

void writeEEPROM(long latitude, long longitude)
{
	byte temp[4] = {0,0,0,0};

	// turn latitude into an array of bytes
	temp[0] = latitude >> 12;
	temp[1] = (latitude >> 8) & 0b1111;
	temp[2] = (latitude >> 4) & 0b1111;
	temp[3] = latitude & 15;

	// write latitude to EEPROM
	EEPROM.write(0, temp[0]);
	EEPROM.write(1, temp[1]);
	EEPROM.write(2, temp[2]);
	EEPROM.write(3, temp[3]);

	// turn longitude into an array of bytes
	temp[0] = longitude >> 12;
	temp[1] = (longitude >> 8) & 0b1111;
	temp[2] = (longitude >> 4) & 0b1111;
	temp[3] = longitude & 15;

	// write longitude to EEPROM
	EEPROM.write(4, temp[0]);
	EEPROM.write(5, temp[1]);
	EEPROM.write(6, temp[2]);
	EEPROM.write(7, temp[3]);
}

void readEEPROM(long* latitude, long* longitude)
{
	*latitude = (EEPROM.read(0) * 4096) +
		(EEPROM.read(1) * 256) +
		(EEPROM.read(2) * 16) +
		EEPROM.read(3);

	*longitude = (EEPROM.read(4) * 4096) +
		(EEPROM.read(5) * 256) +
		(EEPROM.read(6) * 16) +
		EEPROM.read(7);
}
