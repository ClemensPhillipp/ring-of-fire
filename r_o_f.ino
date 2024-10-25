// Pin Definitionen
const int motorPin1 = 2;         // L298N IN1
const int motorPin2 = 4;         // L298N IN2
const int pwmPin = 3;            // L298N PWM (enable pin)
const int lichtschrankePin = 8;  // Lichtschranke Signalpin
const int pauseZeit = 20000;     // Pausezeit nach einer Fahrt (20 Sekunden)
// Variablen
int geschwindigkeit = 0;       // Anfangsgeschwindigkeit
int maxGeschwindigkeit = 255;  // Maximale Motor-Geschwindigkeit (PWM-Wert)
int umdrehungen = 0;           // Zähler für Umdrehungen
const int maxUmdrehungen =5;  // Maximale Anzahl der Umdrehungen

void setup() {
  // Setze die Pins
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(pwmPin, OUTPUT);
  pinMode(lichtschrankePin, INPUT);

  // Motor anfangs stoppen
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  analogWrite(pwmPin, 0);  // PWM auf 0, Motor stoppt

  Serial.begin(9600);
}

void loop() {
  // Motor langsam hochschaukeln
  hochschaukeln();

  // Umdrehungen durchführen
  while (umdrehungen < maxUmdrehungen) {
    if (lichtschrankeErkannt()) {
      umdrehungen++;
      Serial.print("Umdrehung: ");
      Serial.println(umdrehungen);
      delay(20);
    }
  }

  // Motor langsam abbremsen und stoppen, wenn die Lichtschranke die Ausgangsposition erkennt
  abbremsen();

  // Pause von 20 Sekunden nach der Fahrt
  delay(pauseZeit);

  // Umdrehungszähler zurücksetzen
  umdrehungen = 0;
}

void hochschaukeln() {
  Serial.println("beschleunigen");
  forward(50);
  delay(500);
  stopMotor();
  delay(200);
  backward(100);
  delay(1000);
  stopMotor();
  delay(200);
  forward(150);
  delay(1500);
  stopMotor();
  delay(200);
  backward(200);
  delay(2000);
  stopMotor();
  delay(200);
  forward(maxGeschwindigkeit);




  // for (int i = 1; i < 5; i++) {
  //  if(i % 2 == 0)
  //     backward(i * 30);
  //   else
  //     forward(i * 30);
  //   delay(500);
  //   Serial.println(i);
  // }

  // analogWrite(pwmPin, maxGeschwindigkeit);  // Startet die volle Fahrt
}


void abbremsen() {
  for (int i = 255; i >= 0; i -= 5) {
    forward(i);
    delay(80);
    Serial.println(i);
  }

  for (int i = 0; i <= 50; i += 5) {
    backward(i);
    delay(50);
    Serial.println(i);
  }
  while (true) {
    if (lichtschrankeErkannt()) {
      stopMotor();  // Motor stoppen, sobald Geschwindigkeit 0 erreicht
      break;
    }
  }
}

bool lichtschrankeErkannt() {
  // Lichtschranke gibt HIGH zurück, wenn sie die Ausgangsposition erkennt
  Serial.println(digitalRead(lichtschrankePin));
  return digitalRead(lichtschrankePin) == HIGH;
}

void backward(int speed) {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(pwmPin, speed);
}

void forward(int speed) {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  analogWrite(pwmPin, speed);
}

void stopMotor() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  analogWrite(pwmPin, 0);  // PWM auf 0, Motor stoppt
}
