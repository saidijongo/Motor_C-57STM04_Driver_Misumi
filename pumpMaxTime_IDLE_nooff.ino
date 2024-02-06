#include <Arduino.h>

const int motorPins[] = {54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83};
const int dirPin = 45;
const int pulPin = 46;
const int servoPWM = 44;

const int driverOut1 = 84;
const int driverOut2 = 85;

const int numPumps = sizeof(motorPins) / sizeof(motorPins[0]);

bool isReverse = true;

// Find the maximum runTime for param2 values.
int maxRunTime = 0;

unsigned long pumpStartTimes[numPumps] = {0};
unsigned long lastPumpEndTime = 0; // Variable to store the end time of the last pump

String currentMotorType; // Add this line to store the current motor type

enum MotorState
{
  IDLE,
  RUNNING_PUMPS,
  RUNNING_STEPPER
};

MotorState motorState = IDLE;

void setup()
{
  for (int i = 0; i < numPumps; i++)
  {
    pinMode(motorPins[i], OUTPUT);
    digitalWrite(motorPins[i], LOW);
  }

  pinMode(dirPin, OUTPUT);
  pinMode(pulPin, OUTPUT);
  pinMode(servoPWM, OUTPUT);

  Serial.begin(115200);
}

void runPumps(int pumpNumber, int runTime)
{
  digitalWrite(isReverse ? driverOut2 : driverOut1, HIGH);
  digitalWrite(motorPins[pumpNumber - 54], HIGH);
  pumpStartTimes[pumpNumber - 54] = millis();

  // Update lastPumpEndTime if the current pump has a later end time
  if (millis() + runTime > lastPumpEndTime)
  {
    lastPumpEndTime = millis() + runTime;
  }

  Serial.print("Running pump: ");
  Serial.println(pumpNumber);
}

void runServo(int angle, int runTime)
{
  // Move the servo to the specified angle
  analogWrite(servoPWM, angle);

  // Wait for the specified time
  delay(runTime);
}

void runStepper(int angle, int runTime)
{
  int direction = (angle >= 0) ? HIGH : LOW;
  angle = abs(angle);
  digitalWrite(dirPin, direction);

  // Calculate the number of steps based on the angle
  int steps = int(2 * (angle / 1.8));

  // Run the stepper motor
  for (int i = 0; i < steps; i++)
  {
    digitalWrite(pulPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(pulPin, LOW);
    delayMicroseconds(500);
  }

  // Run for the requested time
  delay(runTime);
}

void processCommand(String command)
{
  char separator = ',';

  int firstBracketIndex = command.indexOf('(');
  int secondBracketIndex = command.indexOf(')', firstBracketIndex + 1);

  if (firstBracketIndex != -1 && secondBracketIndex != -1)
  {
    // Extract motor type and SID from the first set of brackets
    String motorTypeAndSID = command.substring(firstBracketIndex + 1, secondBracketIndex);
    int spaceIndex = motorTypeAndSID.indexOf(' ');

    if (spaceIndex != -1)
    {
      String motorType = motorTypeAndSID.substring(0, spaceIndex);
      String SID = motorTypeAndSID.substring(spaceIndex + 1);

      currentMotorType = motorType; // Store the current motor type

      // Extract pump number/angle and run time from subsequent brackets
      int index = secondBracketIndex + 1;
      while (index < command.length())
      {
        int nextBracketIndex = command.indexOf('(', index);
        int endIndex = command.indexOf(')', nextBracketIndex + 1);

        if (nextBracketIndex != -1 && endIndex != -1)
        {
          String inputData = command.substring(nextBracketIndex + 1, endIndex);

          int commaIndex = inputData.indexOf(separator);
          if (commaIndex != -1)
          {
            int param1 = inputData.substring(0, commaIndex).toInt();
            int param2 = inputData.substring(commaIndex + 1).toInt();

            Serial.print(currentMotorType); // Use the stored motor type
            Serial.print(": Param1: ");
            Serial.print(param1);
            Serial.print(", Param2: ");
            Serial.println(param2);

            if (currentMotorType == "PUMPMOTOR_OPERATION")
            {
              runPumps(param1, param2);
              maxRunTime = max(maxRunTime, param2);
              motorState = RUNNING_PUMPS;
              Serial.println("maxRunTime: " + String(maxRunTime));
            }
            else if (currentMotorType == "SERVOMOTOR_OPERATION")
            {
              runServo(param1, param2);
            }
            else if (currentMotorType == "STEPPERMOTOR_OPERATION")
            {
              runStepper(param1, param2);
            }
            else
            {
              Serial.println("Unknown motor type");
            }
          }
          else
          {
            Serial.println("Invalid pump data format");
          }

          index = endIndex + 1;
        }
        else
        {
          break;
        }
      }
    }
    else
    {
      Serial.println("Invalid motor type and SID format");
    }
  }
  else
  {
    Serial.println("Invalid command format");
  }

  // Check and deactivate pumps if the run time has elapsed
  bool allPumpsOff = true;
  for (int i = 0; i < numPumps; i++)
  {
    if (millis() < pumpStartTimes[i] || digitalRead(motorPins[i]) == HIGH)
    {
      allPumpsOff = false;
      break;
    }
  }

  if (allPumpsOff && motorState == RUNNING_PUMPS)
  {
    if (currentMotorType == "PUMPMOTOR_OPERATION")
    {
      digitalWrite(driverOut1, LOW);
    }
    else if (currentMotorType == "REVERSE_PUMPMOTOR_OPERATION")
    {
      digitalWrite(driverOut2, LOW);
    }

    motorState = IDLE;
  }
}

void loop()
{
  if (motorState == IDLE)
  {
    if (Serial.available() > 0)
    {
      String data = Serial.readStringUntil('\n');
      Serial.println(data);
      processCommand(data);
    }
  }
  else if (motorState == RUNNING_PUMPS)
  {
    // Check and deactivate pumps if the run time has elapsed
    for (int i = 0; i < numPumps; i++)
    {
      if (millis() >= pumpStartTimes[i] + lastPumpEndTime && digitalRead(motorPins[i]) == HIGH)
      {
        digitalWrite(motorPins[i], LOW);
      }
    }

    if (millis() >= lastPumpEndTime)
    {
      // Run the stepper motor after all pumps have finished
      motorState = RUNNING_STEPPER;
    }
  }
  else if (motorState == RUNNING_STEPPER)
  {
    // Run the stepper motor with appropriate values
    runStepper(5000,2000);

    // Reset motor state to IDLE
    motorState = IDLE;
  }
}
