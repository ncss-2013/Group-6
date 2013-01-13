void setup()
{
  int button;
  
  Serial.begin(9600);
  
  //pinMode(2, INPUT);
}

void loop()
{ 
  int button = digitalRead(2);
  int ander = 1
  
  if (( button & ander) > 0)
  { 
    Serial.print(button);
  }
}



