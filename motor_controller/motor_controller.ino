/*
  Simple Distance Sensor Sketch (HC-SR04 Ultrasonic)
  Measures distance and prints to Serial Monitor.
*/

const int trigPin = A0;
const int echoPin = A1;

// Left Module
const int ENA_LM = 3; // EMA -> 3~ pin on arduino
const int IN1_LM = 2;
const int IN2_LM = 4;
const int IN3_LM = 7;
const int IN4_LM = 8;
const int ENB_LM = 5;

// Right Module
const int ENA_RM = 6;
const int IN1_RM = 10;
const int IN2_RM = 11;
const int IN3_RM = 12;
const int IN4_RM = 13;
const int ENB_RM = 9;

enum MotorState {
  STOP,
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

enum MotorState motorState = STOP; // Initial state

void leftWheelFront();
void rightWheelFront();
void leftWheelBack();
void rightWheelBack();

// Reads the HC-SR04 and returns distance in cm, or -1 on timeout/no-echo.
long readUltrasonicCm(int trig, int echo, unsigned long timeout = 30000UL);
void distanceSensor();

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(ENA_LM, OUTPUT);
  pinMode(IN1_LM, OUTPUT);
  pinMode(IN2_LM, OUTPUT);
  pinMode(IN3_LM, OUTPUT);
  pinMode(IN4_LM, OUTPUT);
  pinMode(ENB_LM, OUTPUT);  

  pinMode(ENA_RM, OUTPUT);
  pinMode(IN1_RM, OUTPUT);
  pinMode(IN2_RM, OUTPUT);
  pinMode(IN3_RM, OUTPUT);
  pinMode(IN4_RM, OUTPUT);
  pinMode(ENB_RM, OUTPUT);
}

void loop() {
  distanceSensor();
  delay(500); // wait half a second between measurements
}

// Implementation: triggers the HC-SR04, measures echo pulse width (microseconds)
// and converts to centimeters. Returns -1 if no echo was received within
// the timeout (microseconds).
long readUltrasonicCm(int trig, int echo, unsigned long timeout) {
  // Make sure trigger is low
  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  // Send a 10us HIGH pulse to trigger the measurement
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  // Read the echo pulse width (in microseconds)
  unsigned long duration = pulseIn(echo, HIGH, timeout);
  if (duration == 0) {
    // timeout, no echo received
    return -1;
  }

  // Speed of sound ~343.0 m/s => 0.0343 cm/us. Distance is half the round-trip.
  float distanceCm = (duration * 0.0343f) / 2.0f;
  // Return rounded integer centimeters for simpler downstream logic
  return (long)(distanceCm + 0.5f);
}

void distanceSensor() {
  long distance = readUltrasonicCm(trigPin, echoPin);

  if (distance < 0) {
    Serial.println("Distance: no echo");
  } else {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }
}

void leftWheelFront() {
  // Left motor forward: IN1 = HIGH, IN2 = LOW, enable PWM on ENA_LM
  digitalWrite(IN1_LM, HIGH);
  digitalWrite(IN2_LM, LOW);
  analogWrite(ENA_LM, 200); // PWM (0-255) — tune as needed
}
void rightWheelFront() {
  // Right motor forward: IN1 = HIGH, IN2 = LOW, enable PWM on ENA_RM
  digitalWrite(IN1_RM, HIGH);
  digitalWrite(IN2_RM, LOW);
  analogWrite(ENA_RM, 200);
}
void leftWheelBack() {
  // Left motor reverse: IN1 = LOW, IN2 = HIGH
  digitalWrite(IN1_LM, LOW);
  digitalWrite(IN2_LM, HIGH);
  analogWrite(ENA_LM, 200);
}
void rightWheelBack() {
  // Right motor reverse: IN1 = LOW, IN2 = HIGH
  digitalWrite(IN1_RM, LOW);
  digitalWrite(IN2_RM, HIGH);
  analogWrite(ENA_RM, 200);
}
