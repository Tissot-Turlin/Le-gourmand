#include <Servo.h>  // Inclusion de la bibliothèque pour contrôler un servo

// Déclaration des broches utilisées
const int trigPin = 5;      // Broche TRIG du capteur HC-SR04 connectée à D5
const int echoPin = 7;      // Broche ECHO du capteur HC-SR04 connectée à D7
const int servoPin = A3;    // Broche de contrôle du servo connectée à A3

Servo myServo;  // Création de l'objet servo

bool moved = false;              // Indique si le servo a déjà bougé
unsigned long lastMoveTime = 0;  // Enregistre le moment du dernier mouvement

void setup() {
  pinMode(trigPin, OUTPUT);  // TRIG en sortie (envoi d'impulsion)
  pinMode(echoPin, INPUT);   // ECHO en entrée (réception de l'impulsion)
  myServo.attach(servoPin);  // Attache le servo à la broche A3
  myServo.write(0);          // Position initiale à 0°
  Serial.begin(9600);        // Démarre la communication série pour le moniteur
}

void loop() {
  long duration, distance;

  // Envoi d'une impulsion de 10 microsecondes sur TRIG
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Lecture du temps entre l'envoi et la réception de l'onde ultrason
  duration = pulseIn(echoPin, HIGH);

  // Conversion du temps en distance (cm)
  distance = duration * 0.034 / 2;

  // Affichage de la distance dans le moniteur série
  Serial.print("Distance : ");
  Serial.print(distance);
  Serial.println(" cm");

  // Temps actuel depuis le démarrage de la carte (en millisecondes)
  unsigned long currentTime = millis();

  // Si un objet est à moins de 30 cm et que le servo n’a pas encore bougé
  if (distance < 30 && !moved) {
    myServo.write(90);        // Tourne le servo à 90° (quart de tour)
    moved = true;             // Indique que le servo a bougé
    lastMoveTime = currentTime; // Enregistre le temps du mouvement
  }

  // Si le servo a bougé et que 2 minutes sont passées
  if (moved && (currentTime - lastMoveTime >= 120000)) {
    moved = false;  // Réinitialise l'état pour autoriser un nouveau mouvement
  }

  delay(100);  // Petite pause avant de refaire une lecture
}
