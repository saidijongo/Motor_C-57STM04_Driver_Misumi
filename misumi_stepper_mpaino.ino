//24V driver with PUL, DIR, ENA and ALM terminals
//Driver: 7& 8 ON
int PUL=11; 
int DIR=12; 
void setup() {
  pinMode (PUL, OUTPUT);
  pinMode (DIR, OUTPUT);

}

void loop() {
  for (int i=0; i<1600; i++)    //Forward
  {
    digitalWrite(DIR,LOW);
    digitalWrite(PUL,LOW);
    delayMicroseconds(1000);
    digitalWrite(PUL,HIGH);
    delayMicroseconds(1000);
  }
  for (int i=0; i<1600; i++)   //Backward
  {
    digitalWrite(DIR,HIGH);
    digitalWrite(PUL,LOW);
    delayMicroseconds(1000);
    digitalWrite(PUL,HIGH);
    delayMicroseconds(1000);
  }
}
