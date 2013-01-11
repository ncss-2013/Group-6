
// DO NOT FORGET TO INCLUDE SoftwareSerial.h IN SOURCE FILES

#ifndef GLOBALPOSITIONING_H
#define GLOBALPOSITIONING_H

#include "Arduino.h"

#include <SoftwareSerial.h>

class GlobalPositioning
{
    public:
    
    enum DataType
    {
        GGA,
        RMC,
        NDATATYPE
    };
    
    // MEMBER FUNCTIONS //
    
    GlobalPositioning(byte rxPin, byte txPin, unsigned int bRate);
    
    // read an NMEA sentence; returns:
    // 0 if sentence was read correctly
    // -1 if no serial data was available
    // -2 if no sentence was available
    // -3 if checksum didn't match
    // -4 if improper sentence
    // -5 if sentence type not recognised (or implemented)
    // -6 for miscellaneous (mysterious) errors
    int readData();
    
    // private:
    
    // PRIVATE MEMBER FUNCTIONS //
    
    // returns a character from serial input,
    // updating the checksum value
    // checks for char availability
    inline char getChar();
    
    // returns a C string of given length read from serial
    inline void getString(char* string, unsigned int digits);

    // parse csv data
    inline void parseFields(char* source, char** fields, int nFields);

    // convert an ASCII representation of a float to a float
    inline float convertFloat(char* source);
    
    // PRIVATE VARIABLES //
    
    // SoftwareSerial object
    SoftwareSerial serial;

    // NMEA sentence data type
    DataType type;
    
    // used for calculating checksum
    byte tempCheck;

    // time of fix (HHMMSS UTC)
    unsigned int rawTimeOfFix;

    // coordinates
    unsigned int latDeg;
    unsigned int latMin;
    float latSec;
    char latDir;
    unsigned int longDeg;
    unsigned int longMin;
    float longSec;
    char longDir;

    // speed in knots
    float speedKnots;

    // date (DDMMYY)
    unsigned int rawDate;
};

#endif // GLOBALPOSITIONING_H
