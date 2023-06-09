#include <Stepper.h>
#define button 2
const int stepsPerRevolution = 2038;

const int minDistance = 10;
const int maxDistance = 300;

int maxStep = 1000;
int minStep = 0;
int pos = 0;

Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);
const int pingPin = 13;
const int echoPin = 12;
const int stepSize = 10;
long val;


void setup() {
  // initialize serial communication:
  pinMode(button, INPUT_PULLUP);
  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  pinMode(button, INPUT);
  myStepper.setSpeed(10);
  delay(2000);
  // calibrateServo();
}

void loop() {
  val = getDistance();

  if (val > 300 || val < 20) return;
  val = map(val, 20, 150, 0, 170);  // scale it to use it with the servo (value between 0 and 180) 1350
  int move = val - pos;

  if (val > pos) {
    myStepper.step(stepSize);  // sets the servo position according to the scaled value
    pos = pos + stepSize;

  } else {
    myStepper.step(-stepSize);  // sets the servo position according to the scaled value
    pos = pos - stepSize;
  }

  Serial.print("Move");
  Serial.println(move);
  Serial.print("Pos");
  Serial.println(pos);
  delay(200);

  long cm = getDistance();

  Serial.print(cm);
  Serial.println("cm");
}
long getDistance() {
  long duration;
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);
  duration = pulseIn(echoPin, HIGH);

  return microsecondsToCentimeters(duration);
}
void calibrateServo() {
  int position = 0;
  myStepper.step(position);
  bool isPressed = false;
  while (!isPressed) {
    myStepper.step(position);
    delay(100);
    if (!digitalRead(button)) {
      Serial.println("gedrückt");
      isPressed = true;
    } else {
      Serial.println("elese 1");
    }
    position++;
  }
  Serial.println(position);

  maxStep = position;
  delay(100);
  isPressed = false;
  while (!isPressed) {
    myStepper.step(position);
    delay(100);
    Serial.println(digitalRead(button));
    if (!digitalRead(button)) {
      Serial.println("gedrückt");
      isPressed = true;
    }
    position--;
  }
  minStep = position;
  Serial.println(position);
}
long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}