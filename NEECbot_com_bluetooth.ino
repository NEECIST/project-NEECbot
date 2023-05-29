#include <Servo.h>        //add Servo Motor library                     
#include <NewPing.h>      //add Ultrasonic sensor library
#include <SoftwareSerial.h>

#define TxD 0
#define RxD 1
#define TRIG_PIN 3
#define ECHO_PIN 4
#define MAX_DISTANCE 50 // sets maximum useable sensor measuring distance to 300cm

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); // sets up sensor library to use the correct pins to measure distance.
SoftwareSerial btSerial(TxD, RxD);

// global vars
int leftDistance, rightDistance; //distances on either side
int state;
Servo rightservo;
Servo leftservo;
Servo reader;

void setup() {
  state = 0;
  Serial.begin(9600);
  btSerial.begin(9600);
  rightservo.attach(11);
  leftservo.attach(10);
  reader.attach(7);
  reader.write(110);
  rightservo.write(90);
  leftservo.write(90);
  delay(100); // delay for one seconds
  Serial.println("Began");
}
void loop() {
  if(btSerial.available()){
    state = btSerial.read();
    Serial.println(btSerial.read());
  }
  checkstate(state);
}
//-----------------------------------------------------------//Deciding which side to go//------------------------------------------------------------------------
void moveBot(int x, int y){
  rightservo.write(x);
  leftservo.write(y);
}
int GetDistance(){
  int duration, distance;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  
  digitalWrite(TRIG_PIN, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}

void checkstate(int state){
 /* int dist;
  dist = GetDistance();
  if( dist<30 ) moveBot(90,90);*/
  Serial.println(sonar.ping_cm());
  
   if(state=='1') moveautoBot();
   else if(state=='s') moveBot(135,45); //trás
   else if(state=='w') moveBot(45,135);
   else if(state=='a') moveBot(70,70);
   else if(state=='d') moveBot(110,110);
   else if (state=='2'){ turnAround(); }
   else moveBot(90,90);

  // if((state!='w') || (state!='s')) state='0'; //retorna ao estado parado , assim que concluir uma ação que não exija ser continuada, como o andar para a frente ou trás
  
}
  
void moveautoBot(){
  int distance;
  distance = GetDistance();
  if((distance<30)&&(distance!=0)){
    changePath();
  }
  moveBot(45,135);
}
 
void changePath() {
    moveBot(90,90);   // stop forward movement
    reader.write(36);  // check distance to the right
    delay(500);
    rightDistance = GetDistance(); //set right distance
    delay(500);
    reader.write(144);  // check distace to the left
    delay(700);
    leftDistance = GetDistance(); //set left distance
    delay(500);
    reader.write(90); //return to center
    delay(100);
    compare();
  }

void compare(){
  if ((leftDistance>rightDistance)&&(leftDistance>30)){ //if left is less obstructed 
    moveBot(70,70);
    delay(300); // run motors this way for 300
    moveBot(45,135);
  }
  else if ((rightDistance>leftDistance)&&(rightDistance>30)){ //if right is less obstructed
    moveBot(110,110);
    delay(300); // run motors this way for 300
    moveBot(45,135);
  } 
  else {
    moveBot(135,45);
    delay(500);
    changePath();
  }
}

void turnAround() {
  moveBot(140,140);
  delay(600); // run motors this way for 1700        

  moveBot(90,90); //Set motors back to forward 
}  

void turnRight() {
  moveBot(140,140);
  delay(150); // run motors this way for 1500     
  moveBot(90,90);
}  

void turnLeft() {
  moveBot(40,40);
  delay(150); // run motors this way for 1500     
  moveBot(90,90);
}  
