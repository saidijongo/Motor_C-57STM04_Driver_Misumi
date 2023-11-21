int PUL=2; 
int DIR=3; 
void setup() {
  pinMode (PUL, OUTPUT);
  pinMode (DIR, OUTPUT);

}

void loop() {
  for (int i=0; i<6400; i++)    //Forward
  {
    digitalWrite(DIR,LOW);
    digitalWrite(PUL,LOW);
    delayMicroseconds(1000);
    digitalWrite(PUL,HIGH);
    delayMicroseconds(60);
  }
  for (int i=0; i<6400; i++)   //Backward
  {
    digitalWrite(DIR,HIGH);
    digitalWrite(PUL,LOW);
    delayMicroseconds(1000);
    digitalWrite(PUL,HIGH);
    delayMicroseconds(60);
  }
}
