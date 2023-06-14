#include <Servo.h>        // Biblioteca para controle do servo motor
#include <NewPing.h>      // Biblioteca para o sensor ultrassônico

#define TRIG_PIN 3 
#define ECHO_PIN 4
#define MAX_DISTANCE 300 // Define a distância máxima de medição do sensor para 300cm
#define COLL_DIST 25 // Define a distância em que o robô para e reverte o movimento para 30cm
#define TURN_DIST COLL_DIST + 20 // Define a distância em que o robô desvia de um objeto

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); // Configura o sensor para usar os pinos corretos

Servo left_servo;  // Criação do objeto para controle do servo motor esquerdo
Servo right_servo; // Criação do objeto para controle do servo motor direito
Servo reader; // Criação do objeto para controle do servo motor dos olhos

// Variáveis globais
int leftDistance, rightDistance; // Distâncias em cada lado
int angle = 0;
int angleIncrement = 50;

//-------------------------------------------- CONFIGURAÇÃO --------------------------------------------------------

void setup() {
  left_servo.attach(11); 
  right_servo.attach(10);
  reader.attach(6);
  reader.write(90);
  Serial.begin(9600);
  delay(1000); // Aguarda 1 segundo
}

//--------------------------------------------- LOOP PRINCIPAL ----------------------------------------------------

void loop() {
  reader.write(angle);  
  if (readPing() < COLL_DIST) {
    reader.write(90);
    changePath();
  } else {
    moveForward();
  }
  
  delay(100); 
  
  // new
  angle += angleIncrement;
  if (angle <= 0 || angle >= 100) {
    angleIncrement *= -1;  
  }
}

//------------------------------------------------- MÉTODOS -------------------------------------------------------

void changePath() {
  moveStop();
  reader.write(36);
  delay(300); // Atraso adicional para estabilizar o servo motor antes de ler a distância à direita
  rightDistance = readPing();
  delay(300); // Atraso adicional antes de ler a distância à esquerda
  reader.write(144);
  delay(300); // Atraso adicional para estabilizar o servo motor antes de ler a distância à esquerda
  leftDistance = readPing();
  delay(300);
  reader.write(90);
  delay(300);
  compareDistance();
}

void compareDistance() {
  if (leftDistance > rightDistance && leftDistance > 25) {
    turnLeft();
  } else if (rightDistance > leftDistance && rightDistance > 25) {
    turnRight();
  } else if (leftDistance < 15 && rightDistance < 15) {
    turnAround();
  } else {
    moveBackward();
    delay(500); // Aumenta o tempo de movimento para trás para 2 segundos (ou ajuste conforme necessário)
    changePath();
  }
}

int readPing() {
  delay(50); // Atraso para 50 milissegundos
  int distance = sonar.ping_cm();
  if (distance == 0) {
    distance = MAX_DISTANCE;
  }
  Serial.println(distance);
  return distance;
}

void moveStop() {
  right_servo.write(90);
  left_servo.write(90);
}

void moveForward() {
  right_servo.write(60);
  left_servo.write(122);
}

void moveBackward() {
  right_servo.write(120);
  left_servo.write(58);
  delay(300); // Aumenta o tempo de movimento para trás para 2 segundos (ou ajuste conforme necessário)
}

void turnRight() {
  left_servo.write(122);
  right_servo.write(120);
  delay(500);
  right_servo.write(0); // Define os motores de volta para a frente
  left_servo.write(180);
}

void turnLeft() {
  left_servo.write(58);
  right_servo.write(60);
  delay(500);
  right_servo.write(0); // Define os motores de volta para a frente
  left_servo.write(180);
}

void turnAround() {
  left_servo.write(140);
  right_servo.write(140);
  delay(1000);
  right_servo.write(90); // Define os motores de volta para a frente
  left_servo.write(90);
}
