#include <Servo.h>

Servo myservo;

int pos = 0;
int potPin = A1;

float filteredADC = 0;     // smoothed value
float alpha = 0.1;         // smoothing factor (0–1)

void setup() {
  myservo.attach(9);
  Serial.begin(9600);
}

void loop() {
delay(5000);
  for (pos = 0; pos <= 100; pos++) {                     // first 0 to 60
    myservo.write(pos);
    delay(20);

    int rawADC = analogRead(potPin);

    // Exponential smoothing
    filteredADC = alpha * rawADC + (1 - alpha) * filteredADC;

    float angle = map(filteredADC, 60, 700, 0, 180);

    Serial.print("Cmd: ");
    Serial.print(pos);
    Serial.print(" | Raw: ");
    Serial.print(rawADC);
    Serial.print(" | Smooth: ");
    Serial.print(filteredADC);
    Serial.print(" | angle: ");
    Serial.println(angle);
  }
  delay(5000);
    for (pos = 100; pos <= 180; pos++) {                //sec 60 to 100
    myservo.write(pos);
    delay(20);

    int rawADC = analogRead(potPin);

    // Exponential smoothing
    filteredADC = alpha * rawADC + (1 - alpha) * filteredADC;

    float angle = map(filteredADC, 60, 700, 0, 180);

    Serial.print("Cmd: ");
    Serial.print(pos);
    Serial.print(" | Raw: ");
    Serial.print(rawADC);
    Serial.print(" | Smooth: ");
    Serial.print(filteredADC);
    Serial.print(" | angle: ");
    Serial.println(angle);
  }
  delay(5000);
 for (pos = 180; pos >= 0; pos--) {                    // 100 to 0 fast(5%)
    myservo.write(pos);
    delay(1);

    int rawADC = analogRead(potPin);

    // Exponential smoothing
    filteredADC = alpha * rawADC + (1 - alpha) * filteredADC;

    float angle = map(filteredADC, 60, 630, 0, 180);

    Serial.print("Cmd: ");
    Serial.print(pos);
    Serial.print(" | Raw: ");
    Serial.print(rawADC);
    Serial.print(" | Smooth: ");
    Serial.print(filteredADC);
    Serial.print(" | angle: ");
    Serial.println(angle);
  }
}