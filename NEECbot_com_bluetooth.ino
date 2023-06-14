#include <Servo.h> // Add Servo Motor library
#include <NewPing.h> // Add Ultrasonic sensor library
#include <SoftwareSerial.h>

#define TxD 0
#define RxD 1
#define TRIG_PIN 3
#define ECHO_PIN 4
#define MAX_DISTANCE 300 // sets maximum usable sensor measuring distance to 50cm

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); // sets up sensor library to use the correct pins to measure distance.
SoftwareSerial btSerial(TxD, RxD);

// global vars
int leftDistance, rightDistance; // distances on either side
int state;
int angle = 0;
int angleIncrement = 50;
Servo rightservo;
Servo leftservo;
Servo reader;

void setup() {
  state = 0;
  Serial.begin(9600);
  btSerial.begin(9600);
  rightservo.attach(10);
  leftservo.attach(11);
  reader.attach(6);
  reader.write(110);
  rightservo.write(90);
  leftservo.write(90);
  delay(100); // delay for one second
  Serial.println("Began");
}

void loop() {
  if (btSerial.available()) {
    state = btSerial.read();
    Serial.println(state);
  }
  checkState(state);
  delay(100);
}

//-----------------------------------------------------------// Deciding which side to go //------------------------------------------------------------------------
void moveBot(int x, int y) {
  rightservo.write(x);
  leftservo.write(y);
}

float getDistance() {
  unsigned int duration;
  float distance;

  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  pinMode(ECHO_PIN, INPUT);
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)

  // Displays the distance on the Serial Monitor
 // Serial.print("Distance: ");
 // Serial.print(distance);
  //Serial.println(" cm");

  if (distance == 0) {
    distance = MAX_DISTANCE;
  }

  return distance;
}

void checkState(char state) {
/* int dist;
dist = getDistance();
if( dist<30 ) moveBot(90,90);*/
  Serial.println(sonar.ping_cm());
  //Serial.print(state);

  if (state == '1')
    moveAutoBot();
  else if (state == 's')
    moveBot(120, 58); // move back
  else if (state == 'w')
    moveBot(60, 122); // move forward
  else if (state == 'a')
    moveBot(70, 70);
  else if (state == 'd')
    moveBot(110, 110);
  else if (state == '2') {
    turnAround();
  } else {
    moveBot(90, 90);
}

// if((state!='w') || (state!='s')) state='0'; // return to the stopped state after completing an action that doesn't need to be continued, such as moving forward or backward
}

void moveAutoBot() {
  float distance;
  distance = getDistance();
  if ( distance < 25 ) {
    changePath();
}
  moveBot(45, 135);
}

void changePath() {
  moveBot(90, 90); // stop forward movement
  reader.write(36); // check distance to the right
  delay(300);
  rightDistance = getDistance(); // set right distance
  delay(300);
  reader.write(144); // check distance to the left
  delay(300);
  leftDistance = getDistance(); // set left distance
  delay(300);
  reader.write(90); // return to center
  delay(300);
  compare();
}

void compare() {
  if ((leftDistance > rightDistance) && (leftDistance > 25)) { // if left is less obstructed
    moveBot(60, 58);
    delay(500); // run motors this way for 300 milliseconds
    moveBot(0, 180);
  } else if ((rightDistance > leftDistance) && (rightDistance > 25)) { // if right is less obstructed
    moveBot(120, 122);
    delay(500); // run motors this way for 300 milliseconds
    moveBot(0, 180);
  } else {
    moveBot(120, 58);
    delay(800);
    changePath();
  }
}

void turnAround() {
  moveBot(140, 140);
  delay(1000); // run motors this way for 600 milliseconds
  moveBot(90, 90); // Set motors back to forward
}

void turnRight() {
  moveBot(120, 122);
  delay(500);
  moveBot(0, 180);
}

void turnLeft() {
  moveBot(60, 58);
  delay(500); // run motors this way for 150 milliseconds
  moveBot(0, 180);
}
