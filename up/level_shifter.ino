#define ANA_PIN A0
#define REF_PIN A1
#define MESSAGE_PIN 7

int ref_signal = 0;
int signal = 0;
byte demod_signal = LOW;

void setup()
{
  pinMode(MESSAGE_PIN, OUTPUT);
  
  Serial.begin(9600);
  
}

void loop()
{
  ref_signal = analogRead(REF_PIN);
  signal = analogRead(ANA_PIN);
  
  demod_signal = signal > ref_signal ? HIGH : LOW;
  /*
  Serial.print("Mod: ");
  Serial.print(signal, DEC);
  
  Serial.print(", Ref: ");
  Serial.println(ref_signal,DEC);
  
  Serial.print("\tDemod: ");
  Serial.println(demod_signal,DEC);
  */
  digitalWrite(MESSAGE_PIN, demod_signal);
}

