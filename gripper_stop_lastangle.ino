#include <Servo.h>

Servo myservo;

int pos = 180;
int potPin = A1;

float filteredADC = 0;
float alpha = 0.1;

float lastAngle = 0;
int stallCount = 0;

const float ANGLE_EPS = 0.5;
const int STALL_LIMIT = 10;

bool objectGripped = false;
int holdPos = 0;

void setup() {
  myservo.attach(9);
  Serial.begin(9600);
}

void loop() {

  // -------- CLOSE / GRIP --------
  if (!objectGripped) {

    for (pos = 180; pos >= 0; pos--) {

      myservo.write(pos);
      delay(10);

      int rawADC = analogRead(potPin);
      filteredADC = alpha * rawADC + (1 - alpha) * filteredADC;
      float angle = map(filteredADC, 60, 640, 0, 180);

      // ----- STALL DETECTION -----
      if (abs(angle - lastAngle) < ANGLE_EPS) {
        stallCount++;
      } else {
        stallCount = 0;
      }

      lastAngle = angle;

      // ----- OBJECT GRIPPED -----
      if (stallCount >= STALL_LIMIT) {
        holdPos = pos;                  // ✅ save grip angle
        objectGripped = true;           // ✅ enter hold state
        Serial.println("OBJECT GRIPPED — HOLDING");
        Serial.println(holdPos+30);
        break;
      }

      Serial.print("Cmd: ");
      Serial.print(pos);
      Serial.print(" | angle: ");
      Serial.println(angle);
    }
  }

  // -------- HOLD MODE --------
  if (objectGripped) {
    myservo.write(holdPos+30);             // ✅ keep torque
    delay(50);
    return;                             // block further motion
  }

  // -------- OPEN (only if not gripping) --------
  for (pos = 0; pos <= 180; pos++) {

    myservo.write(pos);
    delay(10);

    int rawADC = analogRead(potPin);
    filteredADC = alpha * rawADC + (1 - alpha) * filteredADC;
    float angle = map(filteredADC, 60, 640, 0, 180);

    Serial.print("Cmd: ");
    Serial.print(pos);
    Serial.print(" | angle: ");
    Serial.println(angle);
  }
}
