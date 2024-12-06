#include <Servo.h>
#include <NewPing.h>

// Motor Pin-uud
#define BARUUN_MOTOR_HURD 5    // Baruun motor speed
#define BARUUN_MOTOR_CHIG1 7   // Baruun motor chig
#define BARUUN_MOTOR_CHIG2 6   // Baruun motor chig
#define ZUUN_MOTOR_HURD 10     // Zuun motor speed
#define ZUUN_MOTOR_CHIG1 9     // Zuun motor chig
#define ZUUN_MOTOR_CHIG2 8     // Zuun motor chig

// Ultrasonic Sensor Pin-uud
#define SERVO_PIN 3
#define TRIGGER_PIN 2
#define ECHO_PIN 4
// Togmol Utguud
const int MOTOR_HURD = 255;
const int ERGUULEH_HUGATSAA = 550;
const int SAAD_ZAI = 15;
const int DEED_ZAI = 200;

NewPing zaiMedrugch(TRIGGER_PIN, ECHO_PIN, DEED_ZAI);
Servo servoMotor;

void setup() {
  // Motor tohirgoo
  pinMode(BARUUN_MOTOR_HURD, OUTPUT);
  pinMode(BARUUN_MOTOR_CHIG1, OUTPUT);
  pinMode(BARUUN_MOTOR_CHIG2, OUTPUT);
  pinMode(ZUUN_MOTOR_HURD, OUTPUT);
  pinMode(ZUUN_MOTOR_CHIG1, OUTPUT);
  pinMode(ZUUN_MOTOR_CHIG2, OUTPUT);
  
  // Servo tohirgoo
  servoMotor.attach(SERVO_PIN);
  servoMotor.write(90);  // Goliin bairshil 
  
  Serial.begin(9600);
  delay(2000);
}

void UragshYavah() {
  // Baruun motor uragsh
  analogWrite(BARUUN_MOTOR_HURD, MOTOR_HURD);
  digitalWrite(BARUUN_MOTOR_CHIG1, LOW);
  digitalWrite(BARUUN_MOTOR_CHIG2, HIGH);
  
  // Zuun motor uragsh
  analogWrite(ZUUN_MOTOR_HURD, MOTOR_HURD);
  digitalWrite(ZUUN_MOTOR_CHIG1, HIGH);
  digitalWrite(ZUUN_MOTOR_CHIG2, LOW);
}

void HoishYavah() {
  // Baruun motor hoish
  analogWrite(BARUUN_MOTOR_HURD, MOTOR_HURD);
  digitalWrite(BARUUN_MOTOR_CHIG1, HIGH);
  digitalWrite(BARUUN_MOTOR_CHIG2, LOW);
  
  // Zuun motor hoish
  analogWrite(ZUUN_MOTOR_HURD, MOTOR_HURD);
  digitalWrite(ZUUN_MOTOR_CHIG1, LOW);
  digitalWrite(ZUUN_MOTOR_CHIG2, HIGH);
}

void BaruunTiishErgeh() {
  
  // Baruun motor hoish
  analogWrite(BARUUN_MOTOR_HURD, MOTOR_HURD);
  digitalWrite(BARUUN_MOTOR_CHIG1, HIGH);
  digitalWrite(BARUUN_MOTOR_CHIG2, LOW);
  
  // Zuun motor uragsh
  analogWrite(ZUUN_MOTOR_HURD, MOTOR_HURD);
  digitalWrite(ZUUN_MOTOR_CHIG1, HIGH);
  digitalWrite(ZUUN_MOTOR_CHIG2, LOW);
}

void ZuunTiishErgeh() {
  // Baruun motor uragsh
  analogWrite(BARUUN_MOTOR_HURD, MOTOR_HURD);
  digitalWrite(BARUUN_MOTOR_CHIG1, LOW);
  digitalWrite(BARUUN_MOTOR_CHIG2, HIGH);
  
  // Zuun motor hoish
  analogWrite(ZUUN_MOTOR_HURD, MOTOR_HURD);
  digitalWrite(ZUUN_MOTOR_CHIG1, LOW);
  digitalWrite(ZUUN_MOTOR_CHIG2, HIGH);
}

void MotorZogsooh() {
  digitalWrite(BARUUN_MOTOR_CHIG1, LOW);
  digitalWrite(BARUUN_MOTOR_CHIG2, LOW);
  digitalWrite(ZUUN_MOTOR_CHIG1, LOW);
  digitalWrite(ZUUN_MOTOR_CHIG2, LOW);
  analogWrite(BARUUN_MOTOR_HURD, 0);
  analogWrite(ZUUN_MOTOR_HURD, 0);
}

int BaruunTiishHarah() {
  servoMotor.write(0);  // Baruun tish harah
  delay(500);
  int zai = zaiMedrugch.ping_cm();
  delay(100);
  servoMotor.write(90);  // Butsaad gol bairshild
  return zai;
}

int ZuunTiishHarah() {
  servoMotor.write(180);  // Zuun tish harah
  delay(500);
  int zai = zaiMedrugch.ping_cm();
  delay(100);
  servoMotor.write(90);  // Butsaad gol bairshild
  return zai;
}

void loop() {
  int zai = zaiMedrugch.ping_cm();
  
  // 0 utgiig zasah
  if (zai == 0) {
    zai = DEED_ZAI;
  }
  
  Serial.print("Zai: ");
  Serial.println(zai);
  
  if (zai < SAAD_ZAI) {
    // Saad oldloo!
    Serial.println("Saad oldloo!");
    
    // Zogsooj, baga zertei hoish
    MotorZogsooh();
    delay(200);
    HoishYavah();
    delay(300);
    MotorZogsooh();
    delay(200);
    
    // Baruun zuun tish harah
    int baruunZai = BaruunTiishHarah();
    delay(200);
    int zuunZai = ZuunTiishHarah();
    delay(200);
    
    // Hamgiin sain chig songoh
    if (baruunZai == 0) baruunZai = DEED_ZAI;
    if (zuunZai == 0) zuunZai = DEED_ZAI;
    
    if (baruunZai > zuunZai) {
      Serial.println("Baruun tish ergeh");
      BaruunTiishErgeh();
    } else {
      Serial.println("Zuun tish ergeh");
      ZuunTiishErgeh();
    }
    
    delay(ERGUULEH_HUGATSAA);
    MotorZogsooh();
    delay(200);
  } else {
    UragshYavah();
  }
}
