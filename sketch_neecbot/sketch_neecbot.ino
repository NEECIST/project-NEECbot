#include <Servo.h>        //add Servo Motor library            
#include <NewPing.h>      //add Ultrasonic sensor library

#define TRIG_PIN 3 
#define ECHO_PIN 4
#define MAX_DISTANCE 400 // sets maximum useable sensor measuring distance to 300cm
#define COLL_DIST 30 // sets distance at which robot stops and reverses to 30cm
#define TURN_DIST COLL_DIST+20 // sets distance at which robot veers away from object

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); // sets up sensor library to use the correct pins to measure distance.

Servo left_servo;  // create servo object to control servos
Servo right_servo;
Servo reader;

//Global variables
int leftDistance, rightDistance; //distances on either side
int curDist = 0;
int angle;
int up;

//-------------------------------------------- SETUP LOOP ----------------------------------------------------------------------------
void setup() {
  left_servo.attach(11); 
  right_servo.attach(10);
  reader.attach(6);
  reader.write(90);
  Serial.begin(9600);
  delay(1000); // delay for one seconds
  angle=90;
  up=1;
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN,INPUT);
 }
//------------------------------------------------------------------------------------------------------------------------------------

//---------------------------------------------MAIN LOOP ------------------------------------------------------------------------------
void loop() {
  
 /* if(angle<130 and up==1){
    angle=angle+10;
  } else if (angle>50 and up==0){
    angle=angle-10;
  } else if (angle>=125){
    up=0;
  } else  if (angle<=55) {
    up=1;
  }*/
  int i=0;
  reader.write(angle);  // move eyes forward
  i = readPing() ;
  if ( (i < COLL_DIST) and (i>0)) {
    reader.write(90);
    changePath();
  }  // if forward is blocked change direction
  else {
    moveForward();  // move forward
  }
  //delay(500);
 }
//-----------------------------------------------------------//Deciding which side to go//------------------------------------------------------------------------

void changePath() {
  moveStop();   // stop forward movement
    reader.write(36);  // check distance to the right
    delay(200);
    rightDistance = readPing(); //set right distance
    delay(400);
    reader.write(144);  // check distace to the left
    delay(600);
    leftDistance = readPing(); //set left distance
    delay(400);
    reader.write(90); //return to center
    delay(200);
    compareDistance();
  }

  
void compareDistance()   // find the longest distance
{
  if (leftDistance>rightDistance  and leftDistance>30) //if left is less obstructed 
  {
    turnLeft();
    delay(50);
  }
  else if (rightDistance>leftDistance and rightDistance>30) //if right is less obstructed
  {
    turnRight();
    delay(50);
  }
   else  if (leftDistance<15 and rightDistance<15) //if they are equally obstructed
  {
    turnAround();
    delay(50);
  } else {

    moveBackward();
    delay(200);
    changePath();
    
  }
}


//--------------------------------------------//Reading the distance//-------------------------------------------------------------------------------------

int readPing() { // read the ultrasonic sensor distance
  long duration, cm;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
    
  duration = pulseIn(ECHO_PIN, HIGH);

  cm = duration /29 /2;

  Serial.print(cm);
  Serial.println();

  return cm;
}
//------------------------------------------------//movement---------------------------------------------------------------------------------
void moveStop() { right_servo.write(90); left_servo.write(90);}  // stop the motors.
//-------------------------------------------------------------------------------------------------------------------------------------
void moveForward() {
  right_servo.write(45);
  left_servo.write(135);
}
//-------------------------------------------------------------------------------------------------------------------------------------
void moveBackward() {
    right_servo.write(135);
    left_servo.write(45);
}  
//-------------------------------------------------------------------------------------------------------------------------------------
void turnRight() {
  left_servo.write(135);
  right_servo.write(135);
  delay(150); // run motors this way for 1500        

   right_servo.write(45); //Set motors back to forward
   left_servo.write(135); 
}  
//-------------------------------------------------------------------------------------------------------------------------------------
void turnLeft() {
  left_servo.write(45);
  right_servo.write(45);
  delay(150); // run motors this way for 1500     
  right_servo.write(45); //Set motors back to forward
  left_servo.write(135); 
}  
//-------------------------------------------------------------------------------------------------------------------------------------
void turnAround() {
  left_servo.write(135);
  right_servo.write(135);
  delay(600); // run motors this way for 1700        

  right_servo.write(45); //Set motors back to forward
  left_servo.write(135); 
}  
//--------------------------------------------------------------------------------------------------------------------------------------
//void 