/*
  Motor test sketch (example)
  Adjust pins to match your wiring; this toggles forward/back for quick verification.
*/

const int ENA = 3; // PWM
const int IN1 = 2;
const int IN2 = 4;

void setup() {
  Serial.begin(9600);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
}

void forward(int pwm) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, pwm);
}

void backward(int pwm) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, pwm);
}

void stopMotor() {
  analogWrite(ENA, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void loop() {
  Serial.println("Forward");
  forward(180);
  delay(1000);
  stopMotor();
  delay(250);
  Serial.println("Backward");
  backward(180);
  delay(1000);
  stopMotor();
  delay(1000);
}
