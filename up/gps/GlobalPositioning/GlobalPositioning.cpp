
#include "Arduino.h"
#include "GlobalPositioning.h"

#include <SoftwareSerial.h>
#include "stdlib.h"
#include "string.h"

GlobalPositioning::GlobalPositioning(byte rxPin, byte txPin, unsigned int bRate)
    : serial(rxPin, txPin)
{
    // init serial
    serial.begin(bRate);

    // init variables
    type = NDATATYPE;
    tempCheck = 0;
    rawTimeOfFix = 0;
    latDeg = 0;
    latMin = 0;
    latSec = 0.0;
    latDir = 0;
    longDeg = 0;
    longMin = 0;
    longSec = 0.0;
    longDir = 0;
    altitude = 0.0;
    speedKnots = 0.0;
    rawDate = 0;
}

int GlobalPositioning::readData()
{
    // reset tempCheck, used for checksum verification
    tempCheck = 0;

    // check if serial data is available
    if (!serial.available())
        return -1;
    
    // check for '$'
    if (serial.available())
        if (serial.read() != '$')
            return -2;
    
    // check for "GP", waiting if necessary
    char gp[3] = {0,0,'\0'};
    getString(gp, 2);
    if (strcmp(gp, "GP") != 0)
        return -4;
    
    // read sentence data type
    char rawDataType[4] = {0,0,0,'\0'};
    getString(rawDataType, 3);
    
    // interpret sentence data type
    if (strcmp(rawDataType, "GGA") == 0)    // check for GGA sentence
        type = GGA;
    else if (strcmp(rawDataType, "RMC") == 0)   // check for RMC sentence
        type = RMC;
    else    // sentence type not recognised (or implemented)
        return -5;

    // read rest of sentence into buffer
    char buffer[150];
    unsigned int counter = 0;
    while (counter < 150)
    {
        while (!serial.available());    // poll for char availability
        buffer[counter] = getChar(); // read character

        // check if end has been reached
        if (buffer[counter] == '*')
        {
            buffer[counter] = '\0';
            break;
        }

        counter++;  // increment counter
    }

    tempCheck ^= '*';

    // the checksum should match the current value in tempCheck

    // save copy of checksum before it's overwritten
    byte generatedChecksum = tempCheck;

    // read the received checksum
    char rawReceivedChecksum[3] = {0,0,'\0'};
    getString(rawReceivedChecksum, 2);

    // convert the received checksum from ascii hex to binary
    char receivedChecksum =
        ((atoi(rawReceivedChecksum)/10)*16) + (atoi(rawReceivedChecksum)%10);

    // check if checksums match
    if (generatedChecksum != receivedChecksum)
        return -3;

    // parse sentence based on data type
    switch (type)
    {
        case GGA:   // TO BE IMPLEMENTED
         {
            // GGA sentences have 14 relevant fields:
            //  0 time of fix (HHMMSS UTC)
            //  1 latitude (DDMM.MMM)
            //  2 latitude direction (N/S)
            //  3 longitude (DDDMM.MM)
            //  4 longitude direction (E/W)
            //  5 fix quality
            //  6 number of satellites being tracked
            //  7 horizontal dilution of position
            //  8 altitude in metres
            //  9 M for metres
            // 10 don't care
            // 11 don't care
            // 12 don't care
            // 13 don't care

            char data[9][13];
            char* GGAData[9];
            for (unsigned int i = 0; i < 9; i++)
                GGAData[i] = (char*)data[i];
            parseFields(buffer, GGAData, 9);

            // store time of fix
            rawTimeOfFix = atoi(GGAData[0]);

            // get degrees of latitude
            char tempLatDeg[3] = {0,0,'\0'};
            strncpy(tempLatDeg, GGAData[1], 2);
            latDeg = atoi(tempLatDeg);
            GGAData[1][0] = '0';
            GGAData[1][1] = '0';

            // get minutes of latitude
            float tempLatMin = convertFloat(GGAData[1]);
            latMin = int(tempLatMin);

            // get seconds of latitude
            tempLatMin -= float(latMin);
            latSec = tempLatMin * 60.0;

            // get latitude direction
            strncpy(&latDir, GGAData[2], 1);

            // store longitude

            // get degrees of longitude
            char tempLongDeg[4] = {0,0,0,'\0'};
            strncpy(tempLongDeg, GGAData[3], 3);
            longDeg = atoi(tempLongDeg);
            GGAData[3][0] = '0';
            GGAData[3][1] = '0';
            GGAData[3][2] = '0';

            // get minutes of longitude
            float tempLongMin = convertFloat(GGAData[3]);
            longMin = int(tempLongMin);

            // get seconds of longitude
            tempLongMin -= float(longMin);
            longSec = tempLongMin * 60.0;

            // get longitude direction
            strncpy(&longDir, GGAData[4], 1);

            // get altitude in metres
            altitude = convertFloat(GGAData[8]);

            break;
         }

        case RMC:
        {
            // RMC sentences have 11 relevant fields:
            //  0 time of fix (HHMMSS UTC)
            //  1 status (active/void)
            //  2 latitude (DDMM.MMM)
            //  3 latitude direction (N/S)
            //  4 longitude (DDDMMM.MMM)
            //  5 longitude direction (E/W)
            //  6 speed over ground (knots)
            //  7 track angle (degrees true)
            //  8 date (DDMMYY)
            //  9 magnetic variation
            // 10 magnetic variation direction

            char data[11][13];
            char* RMCData[11];
            for (unsigned int i = 0; i < 11; i++)
                RMCData[i] = (char*)data[i];
            parseFields(buffer, RMCData, 11);

            // store time of fix
            rawTimeOfFix = atoi(RMCData[0]);

            // store latitude

            // get degrees of latitude
            char tempLatDeg[3] = {0,0,'\0'};
            strncpy(tempLatDeg, RMCData[2], 2);
            latDeg = atoi(tempLatDeg);
            RMCData[2][0] = '0';
            RMCData[2][1] = '0';

            // get minutes of latitude
            float tempLatMin = convertFloat(RMCData[2]);
            latMin = int(tempLatMin);

            // get seconds of latitude
            tempLatMin -= float(latMin);
            latSec = tempLatMin * 60.0;

            // get latitude direction
            strncpy(&latDir, RMCData[3], 1);

            // store longitude

            // get degrees of longitude
            char tempLongDeg[4] = {0,0,0,'\0'};
            strncpy(tempLongDeg, RMCData[4], 3);
            longDeg = atoi(tempLongDeg);
            RMCData[4][0] = '0';
            RMCData[4][1] = '0';
            RMCData[4][2] = '0';

            // get minutes of longitude
            float tempLongMin = convertFloat(RMCData[4]);
            longMin = int(tempLongMin);

            // get seconds of longitude
            tempLongMin -= float(longMin);
            longSec = tempLongMin * 60.0;

            // get longitude direction
            strncpy(&longDir, RMCData[5], 1);

            // store speed over ground in knots
            speedKnots = convertFloat(RMCData[6]);

            // store date
            rawDate = atoi(RMCData[8]);

            break;
        }

        default:
            return -6;
    }

    // everthing seemed to go fine
    return 0;
}

inline char GlobalPositioning::getChar()
{
    char temp = 0;
    if (serial.available())
    {
        temp = serial.read();
        tempCheck ^= temp;
    }
    
    return temp;
}

inline void GlobalPositioning::getString(char* string, unsigned int digits)
{
    for (unsigned int i = 0; i < digits; i++)
    {
        // poll for char availability
        while (!serial.available());
        string[i] = getChar();  // store char in string
    }
}

inline void GlobalPositioning::parseFields(
    char* source, char** fields, int nFields)
{
    for (unsigned int i = 0; i < nFields; i++)
        strcpy(fields[i], strtok((i == 0)? source : 0, ","));
}

inline float GlobalPositioning::convertFloat(char* source)
{
    // split the floating point number
    char rawBig[10];   // stores chars left of decimal point
    char rawSmall[10];  // stores chars right of decimal point
    strcpy(rawBig, strtok(source, "."));
    strcpy(rawSmall, strtok(0, "."));

    // convert characters into integers
    unsigned int big = atoi(rawBig);
    unsigned int small = atoi(rawSmall);

    return float(big) + (float(small) / pow(10, strlen(rawSmall)));
}
