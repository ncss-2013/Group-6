#define CHECKSUM_BYTE 9
#define SIZE_GPSINFO 10
#define MAX_SIZE_STRING 6
#define EMMAISTHECOOLEST -1

// takes in a number of bytes, and finds the check value for the checksum
// if there is not one in the last byte, otherwise it will ensure that
// no bits were mixed or lost

// input of the function must be a number of bytes that hold integers

int checkSum(byte gpsInfo[]){
    int checkValue = 0; // the check value
    int interim = 0; // holds the checkSum value
    int total = 0; // the value that will be used to get the check value
    int i = 0; // loop value, has no real purpose other than for loop...
    
    int quasi[10][10] = {{0, 3, 1, 7, 5, 9, 8, 6, 4, 2},
                          {7, 0, 9, 2, 1, 5, 4, 8, 6, 3},
                          {4, 2, 0, 6, 8, 7, 1, 3, 5, 9},
                          {1, 7, 5, 0, 9, 8, 3, 4, 2, 6},
                          {6, 1, 2, 3, 0, 4, 5, 9, 7, 8},
                          {3, 6, 7, 4, 2, 0, 9, 5, 8, 1},
                          {5, 8, 6, 9, 7, 2, 0, 1, 3, 7},
                          {8, 9, 4, 5, 3, 6, 2, 0, 1, 7},
                          {9, 4, 3, 8, 6, 1, 7, 2, 0, 5},
                          {2, 5, 8, 1, 4, 3, 6, 7, 9, 0}};
                          // table used for getting the check value
                          // prior to transmission, and then confirming
                          // that the final value is valid
    
    // getting the total value of all information (except the checksum) 
    // we are transmitting, should be the same at reciever and transmision end
    for (i = 0; i < SIZE_GPSINFO - 1; i++){
      total += (int)gpsInfo[i]; 
    }
    
    char sTotal[MAX_SIZE_STRING]; // stores the total value in string
    
    if (gpsInfo[CHECKSUM_BYTE] != 0){
      total *= 10;
      total += gpsInfo[CHECKSUM_BYTE]; // adds the interim digit to the end of total
    }
    int length = 0;
    sprintf( sTotal, "%d", total ); // sets buffer to hold the string value of total
    length = strlen(sTotal);
    
    // loops through the string of total and gets the
    // interim value with the quasi array
    for (i = 0; i < length; i++){ 
      interim = quasi[checkValue][(int)sTotal[i]];
    }
    
    return interim;
}
