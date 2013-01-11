// The format of the information is:
  //   Longitude: dddmm.mmmmm
  //   Latitude:   ddmm.mmmmm




    char line[1500];
     int length = GPSSerial.readBytesUntil(',', line, 11);
     line[length] = 0;
  
     char * pch;
     int i = 0;
     
     strtok( line, "." );
     
     while (pch != NULL)
     {
       GPSinfo[i] = pch;
       pch = strtok (NULL, ".");
       
       i++;
     }
