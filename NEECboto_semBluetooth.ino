#include <Servo.h>        //add Servo Motor library            
#include <NewPing.h>      //add Ultrasonic sensor library
#include <MedianFilter.h>

#define TRIG_PIN 3 
#define ECHO_PIN 4
#define MAX_DISTANCE 300 // sets maximum useable sensor measuring distance to 300cm
#define COLL_DIST 30 // sets distance at which robot stops and reverses to 30cm
#define TURN_DIST COLL_DIST+20 // sets distance at which robot veers away from object
#define MEDIAN_FILTER_SIZE 5 // sets the size of the median filter to 5

MedianFilter filter(MEDIAN_FILTER_SIZE); // creates a median filter object with size 5

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); // sets up sensor library to use the correct pins to measure distance.

Servo left_servo;  // create servo object to control servos
Servo right_servo;
Servo reader;

//Global variables
int leftDistance, rightDistance; //distances on either side
int curDist = 0;
int angle;
int up;
int p;
//-------------------------------------------- SETUP LOOP ----------------------------------------------------------------------------
void setup() {
  left_servo.attach(11); 
  right_servo.attach(10);
  reader.attach(6);
  reader.write(90);
  Serial.begin(9600);
  delay(100); // delay for one seconds
  angle=90;
  up=1;
  performReadings();
 }
//------------------------------------------------------------------------------------------------------------------------------------

//---------------------------------------------MAIN LOOP ------------------------------------------------------------------------------
void loop() {
  reader.write(angle);  // move eyes forward
  a= readPing();
  filter.addValue(a);
  p=filter.getMedian();
  if ( p < COLL_DIST) {
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
    delay(300);
    performReadings();
    rightDistance = filter.getMedian(); //set right distance
    delay(300);
    reader.write(144);  // check distace to the left
    delay(500);
    performReadings();
    leftDistance = filter.getMedian(); //set left distance
    delay(300);
    reader.write(90); //return to center
    delay(100);
    compareDistance();
  }

  
void compareDistance()   // find the longest distance
{
  if (leftDistance>rightDistance  and leftDistance>30) //if left is less obstructed 
  {
    turnLeft();
  }
  else if (rightDistance>leftDistance and rightDistance>30) //if right is less obstructed
  {
    turnRight();
  }
   else  if (leftDistance>15 and rightDistance>15) //if they are equally obstructed
  {
    turnAround();
  } else {

    moveBackward();
    changePath();
    
  }
  filter.dumpBuffer();
  performReadings();
}


//--------------------------------------------//Reading the distance//-------------------------------------------------------------------------------------

int readPing() { // read the ultrasonic sensor distance
  delay(50);   
  unsigned int uS = sonar.ping();
  int cm = uS/US_ROUNDTRIP_CM;
  if(cm == 0){
    
    cm = MAX_DISTANCE;
  }
  Serial.println(cm);
  return cm;
}
//------------------------------------------------//movement---------------------------------------------------------------------------------
void moveStop() { right_servo.write(90); left_servo.write(90);}  // stop the motors.
//-------------------------------------------------------------------------------------------------------------------------------------
void moveForward() {
  right_servo.write(75);
  left_servo.write(109);
}
//-------------------------------------------------------------------------------------------------------------------------------------
void moveBackward() {
    right_servo.write(105);
    left_servo.write(71);
    delay(1000);
}  
//-------------------------------------------------------------------------------------------------------------------------------------
void turnRight() {
  left_servo.write(105);
  right_servo.write(105);
  delay(500); // run motors this way for 1500        

   right_servo.write(90); //Set motors back to forward
   left_servo.write(90); 
   delay(200);
}  
//-------------------------------------------------------------------------------------------------------------------------------------
void turnLeft() {
  left_servo.write(75);
  right_servo.write(75);
  delay(500); // run motors this way for 1500     
  right_servo.write(90); //Set motors back to forward
  left_servo.write(90); 
  delay(200);
}  
//-------------------------------------------------------------------------------------------------------------------------------------
void turnAround() {
  left_servo.write(110);
  right_servo.write(110);
  delay(1000); // run motors this way for 1700        

  right_servo.write(90); //Set motors back to forward
  left_servo.write(90); 
}  
//--------------------------------------------------------------------------------------------------------------------------------------
//void 
void performReadings(){
  for (int i = 0; i < MEDIAN_FILTER_SIZE; i++) {
    filter.addValue(readPing());
    delay(10);
  }
}