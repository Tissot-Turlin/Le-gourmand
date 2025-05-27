#include <Servo.h>

const int trigPin = A3;
const int echoPin = A2;
const int servoPin = A1;

// marron -> GND
// rouge -> 5V USB

Servo myServo;

unsigned long lastMoveTime = 0;             // Dernier mouvement
const unsigned long delayBetweenMoves = 3000; // 2 minutes = 120000 ms

int servoAngle = 0;  // Position actuelle du servo

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myServo.attach(servoPin);
  myServo.write(servoAngle);  // Position initiale
  Serial.begin(9600);
}

void loop() {
  long duration, distance;

  // Déclencheur ultrason
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance : ");
  Serial.print(distance);
  Serial.println(" cm");

  unsigned long currentTime = millis();

  // Si un objet est détecté à moins de 30 cm ET qu'au moins 2 min se sont écoulées
  if (distance < 30 && (currentTime - lastMoveTime >= delayBetweenMoves)) {
    servoAngle += 90;  // Incrémente de 90°
    if (servoAngle > 180) {  // Pour rester dans les limites mécaniques
      servoAngle = 0;
    }

    myServo.write(servoAngle);         // Tourner le servo
    lastMoveTime = currentTime;        // Enregistrer le temps du mouvement
    Serial.print("Servo tourné à : ");
    Serial.println(servoAngle);
  }

  delay(200);  // Pause pour réduire la fréquence de lecture
}
