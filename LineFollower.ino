#include <Arduino.h>

// Motor Pins
#define ENA 5   // Right motor speed
#define IN1 7   // Right motor direction
#define IN2 6   // Right motor direction
#define ENB 10  // Left motor speed
#define IN3 9   // Left motor direction
#define IN4 8   // Left motor direction

// Line Sensor Pins
#define LEFT_LINE_SENSOR A0
#define RIGHT_LINE_SENSOR A1

// Constants
const int MOTOR_SPEED = 255;
const int TURN_SPEED = 200;    // Slower speed for line corrections

// Variables for calibration
int whiteThreshold = 0;
int blackThreshold = 1023;
int LINE_THRESHOLD = 500;  // Will be set during calibration

void setup() {
  // Motor setup
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  // Line sensor setup
  pinMode(LEFT_LINE_SENSOR, INPUT);
  pinMode(RIGHT_LINE_SENSOR, INPUT);
  
  Serial.begin(9600);
  delay(2000);
  
  // Run calibration
  calibrateSensors();
}

void calibrateSensors() {
  Serial.println("\n=== SENSOR CALIBRATION ===");
  Serial.println("1. Place both sensors on WHITE surface");
  Serial.println("2. Waiting 5 seconds...");
  delay(5000);
  
  // Read white surface
  int maxWhite = 0;
  for(int i = 0; i < 10; i++) {
    int leftValue = analogRead(LEFT_LINE_SENSOR);
    int rightValue = analogRead(RIGHT_LINE_SENSOR);
    maxWhite = max(maxWhite, max(leftValue, rightValue));
    delay(100);
  }
  
  Serial.println("3. Place both sensors on BLACK line");
  Serial.println("4. Waiting 5 seconds...");
  delay(5000);
  
  // Read black surface
  int minBlack = 1023;
  for(int i = 0; i < 10; i++) {
    int leftValue = analogRead(LEFT_LINE_SENSOR);
    int rightValue = analogRead(RIGHT_LINE_SENSOR);
    minBlack = min(minBlack, min(leftValue, rightValue));
    delay(100);
  }
  
  // Calculate thresholds
  whiteThreshold = maxWhite + 50;  // Add margin
  blackThreshold = minBlack - 50;  // Subtract margin
  LINE_THRESHOLD = (whiteThreshold + blackThreshold) / 2;
  
  Serial.println("\n=== CALIBRATION RESULTS ===");
  Serial.print("White Threshold: ");
  Serial.println(whiteThreshold);
  Serial.print("Black Threshold: ");
  Serial.println(blackThreshold);
  Serial.print("Line Threshold: ");
  Serial.println(LINE_THRESHOLD);
  
  Serial.println("\nStarting in 5 seconds...");
  delay(5000);
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

void adjustRight() {
  // Right motor forward at turn speed
  analogWrite(ENA, TURN_SPEED);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  
  // Stop left motor
  analogWrite(ENB, 0);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void adjustLeft() {
  // Stop right motor
  analogWrite(ENA, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  
  // Left motor forward at turn speed
  analogWrite(ENB, TURN_SPEED);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void handleLineDetection() {
  int leftValue = analogRead(LEFT_LINE_SENSOR);
  int rightValue = analogRead(RIGHT_LINE_SENSOR);
  
  // Print sensor values for debugging
  Serial.print("Left: ");
  Serial.print(leftValue);
  Serial.print(" Right: ");
  Serial.println(rightValue);
  
  // If both sensors detect white, stop
  if (leftValue > LINE_THRESHOLD && rightValue > LINE_THRESHOLD) {
    Serial.println("Both sensors on WHITE - Stopping!");
    stopMotors();
    delay(500);  // Longer delay for full stop
    return;
  }
  
  // If either sensor detects white line
  if (leftValue > LINE_THRESHOLD || rightValue > LINE_THRESHOLD) {
    stopMotors();
    delay(300);  // Longer delay for full stop
    
    if (leftValue > LINE_THRESHOLD) {
      // Left sensor on white, adjust left (away from white)
      Serial.println("White detected on LEFT - Adjusting LEFT");
      adjustLeft();
      delay(300);  // Longer adjustment time
    } else {
      // Right sensor on white, adjust right (away from white)
      Serial.println("White detected on RIGHT - Adjusting RIGHT");
      adjustRight();
      delay(300);  // Longer adjustment time
    }
    stopMotors();  // Full stop after adjustment
    delay(200);
  }
}

void loop() {
  // Check line sensors
  if (analogRead(LEFT_LINE_SENSOR) < LINE_THRESHOLD && 
      analogRead(RIGHT_LINE_SENSOR) < LINE_THRESHOLD) {
    // Both sensors on black - safe to move
    moveForward();
    delay(50);
  } else {
    // White detected - need correction
    handleLineDetection();
  }
}
