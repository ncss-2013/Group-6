
const int LED1 = 13;
const int LED2 = 12;
const int LED3 = 8;
const int LED4 = 7;
const int LED5 = 1;
const int LED6 = 2;
const int LED7 = 3;
const int LED8 = 4;

void setup()
{
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);
  pinMode(LED7, OUTPUT);
  pinMode(LED8, OUTPUT);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  digitalWrite(LED5, LOW);
  digitalWrite(LED6, LOW);
  digitalWrite(LED7, LOW);
  digitalWrite(LED8, LOW);
  
  serial.begin(9600);
}

/*
void loop()
{
  digitalWrite(13, HIGH);                // turn the LED on   
  delay(100);                           // wait                    
  digitalWrite(13, LOW);                 // turn the LED off
  digitalWrite(12, HIGH);
  delay(100);                           // wait 
  digitalWrite(12, LOW);               //turn the LED on
  digitalWrite(8, HIGH);
  delay(100);
  digitalWrite(8, LOW);
  digitalWrite(7, HIGH);
  delay(100);
  digitalWrite(7, LOW);
}
*/

int a = 1;

void loop()
{ 
  //Checks if the a ber is odd or even
  if ((a & 1) > 0)
  {
    digitalWrite(LED1, HIGH);
  }
  
  else
  {
    digitalWrite(LED1, LOW);
  }
  
  //checks if number is a power of 2
  if ((a & 2) > 0)
  {
    digitalWrite(LED2, HIGH);
  }
  
  else
  {
    digitalWrite(LED2, LOW);
  }
  
  // Checks if the number is a power of 4
  if ((a & 4) > 0)
  {
    digitalWrite(LED3, HIGH);
  }
  else
  {
    digitalWrite(LED3, LOW);
  }
  
  //Checks if the number is a power of 8
  if ((a & 8) > 0)
  {
    digitalWrite(LED4, HIGH);
  }
  else 
  { 
    digitalWrite(LED4, LOW);
  }
  
if ((a & 16) > 0)
  {
    digitalWrite(LED5, HIGH);
  }
  else 
  { 
    digitalWrite(LED5, LOW);
  }
  if ((a & 32) > 0)
  {
    digitalWrite(LED6, HIGH);
  }
  else 
  { 
    digitalWrite(LED6, LOW);
  }
  if ((a & 64) > 0)
  {
    digitalWrite(LED7, HIGH);
  }
  else 
  { 
    digitalWrite(LED7, LOW);
  }
  if ((a & 128) > 0)
  {
    digitalWrite(LED8, HIGH);
  }
  else 
  { 
    digitalWrite(LED8, LOW);
  }
  delay(500);
  a += 1;
}
