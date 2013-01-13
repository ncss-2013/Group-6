const int RED_LED_PIN = 9;
const int GREEN_LED_PIN = 10;
const int BLUE_LED_PIN = 11;

void setup() {
  
}

void loop () 
{
  
  int sensorValue =analogRead (0);
  
  int redValue = constrain(map(sensorValue, 0, 512, 0, 255), 0, 255);
  
  int greenValue =
  constrain(map(sensorValue, 0, 512, 0,255), 0, 255) -
  constrain(map(sensorValue, 512, 1023, 0, 255), 0, 255);
  
  
  int blueValue = constrain(map(sensorValue, 512, 1023, 0, 255), 0, 255);
  
  analogWrite(RED_LED_PIN, redValue);
  analogWrite(GREEN_LED_PIN, greenValue);
  analogWrite(BLUE_LED_PIN, blueValue);
  
}
  
