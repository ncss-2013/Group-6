
int temperaturePin = 0;
  
  
  
 int raw_value=0;
 float temperature = 0.0;


void setup()
{
  Serial.begin(9600);
}

void loop()
{
  //float temperature = 0.0
 // int raw_value = 0;
  raw_value = analogRead(temperaturePin);
  
 temperature = raw_value * .004882814;
  
  temperature = (temperature - .5) *100; 
  
  
  Serial.println(raw_value);
  Serial.println(temperature);
  
  
  delay(1000);
}
