#include <Servo.h>
#include <NewPing.h>

// EXACTLY THE SAME PIN DEFINITIONS AS WORKING CODE
#define ENA 5   // Right motor speed
#define IN1 7   // Right motor direction
#define IN2 6   // Right motor direction
#define ENB 10  // Left motor speed
#define IN3 9   // Left motor direction
#define IN4 8   // Left motor direction

// Sensor pins
#define SERVO_PIN 3
#define TRIG_PIN 2
#define ECHO_PIN 4

// Constants
const int MOTOR_SPEED = 255;
const int TURN_TIME = 200;
const int OBSTACLE_DISTANCE = 25;
const int MAXIMUM_DISTANCE = 200;  

NewPing sonar(TRIG_PIN, ECHO_PIN, MAXIMUM_DISTANCE);
Servo servo_motor;

void setup() {
  // Motor setup
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  // Servo setup
  servo_motor.attach(SERVO_PIN);
  servo_motor.write(65);
  
  Serial.begin(9600);
  delay(2000);
}
void moveForward() {
  // Right motor forward
  analogWrite(ENA, MOTOR_SPEED);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  
  // Left motor forward
  analogWrite(ENB, MOTOR_SPEED);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  // Right motor backward
  analogWrite(ENA, MOTOR_SPEED);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  
  // Left motor backward
  analogWrite(ENB, MOTOR_SPEED);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnRight() {
  // Right motor backward
  analogWrite(ENA, MOTOR_SPEED);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  
  // Left motor forward
  analogWrite(ENB, MOTOR_SPEED);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnLeft() {
  // Right motor forward
  analogWrite(ENA, MOTOR_SPEED);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  
  // Left motor backward
  analogWrite(ENB, MOTOR_SPEED);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

int lookRight() {
  servo_motor.write(-20);
  delay(500);
  int distance = sonar.ping_cm();
  delay(100);
  servo_motor.write(65);
  return distance;
}

int lookLeft() {
  servo_motor.write(170);
  delay(500);
  int distance = sonar.ping_cm();
  delay(100);
  servo_motor.write(65);
  return distance;
}

void loop() {
  int distance = sonar.ping_cm();
  
  // Handle zero readings
  if (distance == 0) {
    distance = MAXIMUM_DISTANCE;
  }
  
  Serial.print("Distance: ");
  Serial.println(distance);
  
  if (distance < OBSTACLE_DISTANCE) {
    // Obstacle detected!
    Serial.println("Obstacle detected!");
    
    // Stop and back up slightly
    stopMotors();
    delay(200);
    moveBackward();
    delay(300);
    stopMotors();
    delay(200);
    
    // Look right and left
    int rightDistance = lookRight();
    delay(200);
    int leftDistance = lookLeft();
    delay(200);
    
    // Choose better direction
    if (rightDistance == 0) rightDistance = MAXIMUM_DISTANCE;
    if (leftDistance == 0) leftDistance = MAXIMUM_DISTANCE;
    
    if (rightDistance > leftDistance) {
      Serial.println("Turning right");
      turnRight();
    } else {
      Serial.println("Turning left");
      turnLeft();
    }
    
    delay(TURN_TIME);
    stopMotors();
    delay(200);
  } else {
    moveForward();
  }
  
  delay(50);
}
