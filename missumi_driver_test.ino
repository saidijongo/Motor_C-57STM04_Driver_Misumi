
//24V driver with PUL, DIR, ENA and ALM terminals
//Driver: 7& 8 ON
int PUL=12; 
int DIR=13; 

const int numStepsPerRevolution = 5000; //6400
//int steps = 60 * numStepsPerRevolution / 360;
int steps = 4000;
void setup() {
  pinMode (PUL, OUTPUT);
  pinMode (DIR, OUTPUT);

}

void loop() {
  for (int i=0; i<steps; i++)    //Forward
  {
    digitalWrite(DIR,LOW);
    digitalWrite(PUL,LOW);
    delayMicroseconds(500);
    digitalWrite(PUL,HIGH);
    delayMicroseconds(500);
  }



  for (int i=0; i<steps; i++)   //Backward
  {
    digitalWrite(DIR,HIGH);
    digitalWrite(PUL,LOW);
    delayMicroseconds(500);
    digitalWrite(PUL,HIGH);
    delayMicroseconds(500);
  }
}
