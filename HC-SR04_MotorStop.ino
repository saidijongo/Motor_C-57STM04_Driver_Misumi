int PUL = 2; // Define Pulse pin
int DIR = 3; // Define Direction pin
int trigPin = 7; // Trig pin for HC-SR04
int echoPin = 6; // Echo pin for HC-SR04

void setup() {
  pinMode(PUL, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW); // Ensure the Trig pin is low initially
  Serial.begin(9600); 
}

void loop() {
  // Measure distance using the HC-SR04
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2; // Convert the duration to distance in cm

  // Print the distance to the serial monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // If the distance is less than 10cm, stop the motor and print a message
  if (distance < 10) {
    stopMotor();
    Serial.println("Motor Stopped!");
  } else {
    // Move the motor forward and backward
    moveMotor(6400, LOW);  // Forward 5000 steps
    moveMotor(6400, HIGH); // Backward 5000 steps
  }
}

void moveMotor(int steps, int dir) {
  digitalWrite(DIR, dir);
  for (int i = 0; i < steps; i++) {
    digitalWrite(PUL, LOW);
    delayMicroseconds(1000);
    digitalWrite(PUL, HIGH);
    delayMicroseconds(50);
  }
}

void stopMotor() {
  digitalWrite(PUL, LOW);
  digitalWrite(DIR, LOW);
}
