void setup()
{
  Serial.begin(9600);

}

void loop()
{  
  byte serial_data = 0;
  if (Serial.available())
  {
    serial_data = Serial.read();
    
    Serial.write(serial_data);
    Serial.print("\n");
    
    if (serial_data == 'H')
    { 
      digitalWrite(13, HIGH);
    }
    else if (serial_data == 'L')
    {
      digitalWrite(13, LOW);
    }
    else if (serial_data == '?')
    {
      Serial.println("Type H to turn the LED on and L to turn it off");
    }
    else if (serial_data == 'S')
    { 
      digitalWrite (13, HIGH);
      delay(30);
      digitalWrite (13, LOW);
      delay(30);
    }
    else
    {
      Serial.println("I'm afraid i can't do that.");
    }
  } 
    
  delay(1000);
  
}

