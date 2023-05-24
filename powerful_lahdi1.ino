#include <Servo.h>
#define button 4
const int minDistance = 10;
const int maxDistance = 300;
int maxStep = 1000;
int minStep = 0;
int pos = 0;
Servo servo;
const int pingPin = 11;
long val;


void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  servo.attach(5);
  pinMode(button, INPUT);
  calibrateServo();
}

void loop() {
  val = getDistance();
  val = map(val, 32, 330, minStep, maxStep); // scale it to use it with the servo (value between 0 and 180)
  servo.write(val);               // sets the servo position according to the scaled value
  //Serial.println(val);
  delay(40);                       // waits for the servo to get there
  
  long cm = getDistance();
  
  
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

  delay(100);
}
long getDistance(){
	long duration;
 	pinMode(pingPin, OUTPUT);
  	digitalWrite(pingPin, LOW);
  	delayMicroseconds(2);
  	digitalWrite(pingPin, HIGH);
  	delayMicroseconds(5);
  	digitalWrite(pingPin, LOW);

  	pinMode(pingPin, INPUT);
  	duration = pulseIn(pingPin, HIGH);

	return microsecondsToCentimeters(duration);
}
void calibrateServo() {
  int position = 90;
  servo.write(position);
  bool isPressed = false;
  while(!isPressed){
    servo.write(position);
    delay(100);
    if(digitalRead(button)){
    isPressed = true;
    }
    position++;
  }
  Serial.println(position);
  maxStep = position;
  isPressed = false;
  while(!isPressed){
    servo.write(position);
    delay(100);
    if(digitalRead(button)){
    isPressed = true;
    }
    position--;
  }
  minStep = position;
}
long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}