#include <Arduino.h>

// Motor Pin-uud
#define BARUUN_MOTOR_HURD 5    // Baruun motor speed
#define BARUUN_MOTOR_CHIG1 7   // Baruun motor chig
#define BARUUN_MOTOR_CHIG2 6   // Baruun motor chig
#define ZUUN_MOTOR_HURD 10     // Zuun motor speed
#define ZUUN_MOTOR_CHIG1 9     // Zuun motor chig
#define ZUUN_MOTOR_CHIG2 8     // Zuun motor chig

// Shugum Medregch Pin-uud
#define ZUUN_SHUGUM_MEDREGCH A0
#define BARUUN_SHUGUM_MEDREGCH A1

// Togmol Utguud
const int MOTOR_HURD = 255;
const int ERGUULEH_HURD = 200;    // Udaan hurd shugum zasvarlah

// Calibration-ii hemjee
int HAR_HEMJEE = 0;
int TSAGAAN_HEMJEE = 1023;
int SHUGUM_HEMJEE = 500;  // Calibration deer tohiruulna

void setup() {
  // Motor tohirgoo
  pinMode(BARUUN_MOTOR_HURD, OUTPUT);
  pinMode(BARUUN_MOTOR_CHIG1, OUTPUT);
  pinMode(BARUUN_MOTOR_CHIG2, OUTPUT);
  pinMode(ZUUN_MOTOR_HURD, OUTPUT);
  pinMode(ZUUN_MOTOR_CHIG1, OUTPUT);
  pinMode(ZUUN_MOTOR_CHIG2, OUTPUT);
  
  // Shugum medregch tohirgoo
  pinMode(ZUUN_SHUGUM_MEDRUGCH, INPUT);
  pinMode(BARUUN_SHUGUM_MEDRUGCH, INPUT);
  
  Serial.begin(9600);
  delay(2000);
  
  // Calibration ehluuleh
  MedrugchKalibratsChiglel();
}

void MedrugchKalibratsChiglel() {
  Serial.println("\n=== MEDRUGCH KALIBRATS ===");
  Serial.println("1. Medrugchuudiig HAR ghadarguu deer talbina");
  Serial.println("2. 5 sekund huleene...");
  delay(5000);
  
  // Har ghadarguu unshih
  int ihHar = 0;
  for(int i = 0; i < 10; i++) {
    int zuunUtga = analogRead(ZUUN_SHUGUM_MEDRUGCH);
    int baruunUtga = analogRead(BARUUN_SHUGUM_MEDRUGCH);
    ihHar = max(ihHar, max(zuunUtga, baruunUtga));
    delay(100);
  }
  
  Serial.println("3. Medrugchuudiig TSAGAAN shugum deer talbina");
  Serial.println("4. 5 sekund huleene...");
  delay(5000);
  
  // Tsagaan shugum unshih
  int bagaTsagaan = 1023;
  for(int i = 0; i < 10; i++) {
    int zuunUtga = analogRead(ZUUN_SHUGUM_MEDRUGCH);
    int baruunUtga = analogRead(BARUUN_SHUGUM_MEDRUGCH);
    bagaTsagaan = min(bagaTsagaan, min(zuunUtga, baruunUtga));
    delay(100);
  }
  
  // Hemjeeg tootsooloh
  HAR_HEMJEE = ihHar + 50;  // Nemelt zai
  TSAGAAN_HEMJEE = bagaTsagaan - 50;  // Hasah zai
  SHUGUM_HEMJEE = (HAR_HEMJEE + TSAGAAN_HEMJEE) / 2;
  
  Serial.println("\n=== KALIBRATS UTGA ===");
  Serial.print("Har Hemjee: ");
  Serial.println(HAR_HEMJEE);
  Serial.print("Tsagaan Hemjee: ");
  Serial.println(TSAGAAN_HEMJEE);
  Serial.print("Shugum Hemjee: ");
  Serial.println(SHUGUM_HEMJEE);
  
  Serial.println("\n5 sekundiin daraa ehelnee...");
  delay(5000);
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

void BaruunTiishtZasah() {
  // Baruun motor uragsh
  analogWrite(BARUUN_MOTOR_HURD, ERGUULEH_HURD);
  digitalWrite(BARUUN_MOTOR_CHIG1, LOW);
  digitalWrite(BARUUN_MOTOR_CHIG2, HIGH);
  
  // Zuun motor zogsooh
  analogWrite(ZUUN_MOTOR_HURD, 0);
  digitalWrite(ZUUN_MOTOR_CHIG1, LOW);
  digitalWrite(ZUUN_MOTOR_CHIG2, LOW);
}

void ZuunTiishtZasah() {
  // Baruun motor zogsooh
  analogWrite(BARUUN_MOTOR_HURD, 0);
  digitalWrite(BARUUN_MOTOR_CHIG1, LOW);
  digitalWrite(BARUUN_MOTOR_CHIG2, LOW);
  
  // Zuun motor uragsh
  analogWrite(ZUUN_MOTOR_HURD, ERGUULEH_HURD);
  digitalWrite(ZUUN_MOTOR_CHIG1, HIGH);
  digitalWrite(ZUUN_MOTOR_CHIG2, LOW);
}

void MotorZogsooh() {
  digitalWrite(BARUUN_MOTOR_CHIG1, LOW);
  digitalWrite(BARUUN_MOTOR_CHIG2, LOW);
  digitalWrite(ZUUN_MOTOR_CHIG1, LOW);
  digitalWrite(ZUUN_MOTOR_CHIG2, LOW);
  analogWrite(BARUUN_MOTOR_HURD, 0);
  analogWrite(ZUUN_MOTOR_HURD, 0);
}

void ShugumMedreltShalgah() {
  int zuunUtga = analogRead(ZUUN_SHUGUM_MEDRUGCH);
  int baruunUtga = analogRead(BARUUN_SHUGUM_MEDRUGCH);
  
  // Debug medeelel
  Serial.print("Zuun: ");
  Serial.print(zuunUtga);
  Serial.print(" Baruun: ");
  Serial.println(baruunUtga);
  
  // Hoyor medrugch har deer baival zogsooh
  if (zuunUtga > SHUGUM_HEMJEE && baruunUtga > SHUGUM_HEMJEE) {
    Serial.println("Hoyor tal HAR - Zogsooh!");
    MotorZogsooh();
    delay(500);  // Urt zogsolttoi
    return;
  }
  
  // Ali neg medrugch har deer baival
  if (zuunUtga > SHUGUM_HEMJEE || baruunUtga > SHUGUM_HEMJEE) {
    MotorZogsooh();
    delay(300);  // Urt zogsolttoi
    
    if (zuunUtga > SHUGUM_HEMJEE) {
      // Zuun medrugch har deer, zuun tiish zasah
      Serial.println("Zuun tal HAR - Zuun tiish zasah");
      ZuunTiishtZasah();
      delay(300);  // Urt zasvariin hugatsaa
    } else {
      // Baruun medrugch har deer, baruun tiish zasah
      Serial.println("Baruun tal HAR - Baruun tiish zasah");
      BaruunTiishtZasah();
      delay(300);  // Urt zasvariin hugatsaa
    }
    MotorZogsooh();  // Zasvariin daraa zogsooh
    delay(200);
  }
}

void loop() {
  // Shugum medrugchuudiig shalgah
  if (analogRead(ZUUN_SHUGUM_MEDRUGCH) < SHUGUM_HEMJEE && 
      analogRead(BARUUN_SHUGUM_MEDRUGCH) < SHUGUM_HEMJEE) {
    // Hoyor medrugch tsagaan deer - yavj bolno
    UragshYavah();
    delay(50);
  } else {
    // Har oldson - zasvar hiih
    ShugumMedreltShalgah();
  }
}
