/*
  Ultrasonic sensor test sketch (example)
  Use TRIG -> A0, ECHO -> A1
*/

const int trigPin = A0;
const int echoPin = A1;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH, 30000UL);
  if (duration == 0) {
    Serial.println("Distance: no echo");
  } else {
    float cm = (duration * 0.0343) / 2.0;
    Serial.print("Distance: "); Serial.print(cm); Serial.println(" cm");
  }
  delay(200);
}
