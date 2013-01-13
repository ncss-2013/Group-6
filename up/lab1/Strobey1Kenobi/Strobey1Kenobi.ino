const int LEDPIN = 13;
const int MAGICBUTTON = 2;

void setup()
{
  pinMode(LEDPIN, OUTPUT);
  pinMode(MAGICBUTTON, INPUT);
}
void loop()
{
  if (digitalRead(MAGICBUTTON) == HIGH 
  {
    toggleStrobe(LEDPIN);
  }
  
//  delay(30);
//int a = 0;

//if (a == 0)
//{
//  digitalWrite(LEDPIN, HIGH);
//  delay(30);
//  digitalWrite(LEDPIN, LOW);
//  delay(30);

//}
}

void toggleStrobe(int pin)
{
  int v = digitalRead(LEDPIN);
  if (v == HIGH)
  {
  digitalWrite(LEDPIN, LOW);
  delay(150)
  }
  
  else
  {
    digitalWrite(LEDPIN, HIGH);
    delay(150)
  }
}

void toggleStrobe1(int pins)
{
  int k = digitalRead(LEDPIN);
  if (v == HIGH)
  {
    digitalWrite(LEDPIN, LOW);
  }
  else
  {
    digitalWrite(LEDPIN, HIGH);
  }
}

