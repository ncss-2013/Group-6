void setup() {

  pinMode(9, OUTPUT);
  Serial.begin (9600);

}


void loop()
{

  int val = analogRead(A0);   // reads the value of the potentiometer (value between 0 and 1023) 
  Serial.println(val);
  val = map(val, 320, 130, 0, 2000);
  Serial.println(val);
  //int a = 0;

  //if (a == 0)  
  {

    digitalWrite(9, HIGH);

    delayMicroseconds (val);


    digitalWrite(9, LOW);

    delayMicroseconds (2000 - val);

  }
}

