
//24V driver with PUL, DIR, ENA and ALM terminals
//Driver: 7& 8 ON
int PUL=46; 
int DIR=45; 
const int numStepsPerRevolution = 5000; // Number of steps per revolution
int input_angle = 300; // Desired angle of rotation
//int steps = (input_angle * numStepsPerRevolution) / 360;


//int steps = 5000;
void setup() {
  pinMode (PUL, OUTPUT);
  pinMode (DIR, OUTPUT);

}

void loop() {
  int steps = 5000;

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
