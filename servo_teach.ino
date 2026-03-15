#include <Servo.h>

Servo myservo;

// ------------------ PINS ------------------
const int servoPin = 9;
const int potPin   = A1;

const int teachBtn = 2;
const int saveBtn  = 3;
const int playBtn  = 4;

// ------------------ MODES ------------------
enum Mode {
  PLAY_MODE,
  TEACH_MODE
};

Mode currentMode = PLAY_MODE;

// ------------------ TEACH STORAGE ------------------
float taughtAngles[20];
int taughtCount = 0;

// ------------------ FILTER ------------------
float filteredADC = 0;
float alpha = 0.1;

// ------------------ UTILS ------------------
float readPotAngle() {
  int raw = analogRead(potPin);
  filteredADC = alpha * raw + (1 - alpha) * filteredADC;

  // ⚠️ Adjust mapping to your servo
  return map(filteredADC, 60, 640, 0, 180);
}

// Smooth transition to prevent gear shock
void smoothMove(float from, float to) {
  if (from < to) {
    for (float a = from; a <= to; a++) {
      myservo.write(a);
      delay(10);
    }
  } else {
    for (float a = from; a >= to; a--) {
      myservo.write(a);
      delay(10);
    }
  }
}

// ------------------ SETUP ------------------
void setup() {
  pinMode(teachBtn, INPUT_PULLUP);
  pinMode(saveBtn, INPUT_PULLUP);
  pinMode(playBtn, INPUT_PULLUP);

  Serial.begin(9600);

  myservo.attach(servoPin);
  filteredADC = analogRead(potPin);

  Serial.println("SYSTEM READY — PLAY MODE");
}

// ------------------ TEACH MODE ------------------
void teachLoop() {
  float angle = readPotAngle();

  Serial.print("Teach angle: ");
  Serial.println(angle);

  // SAVE POSITION
  if (digitalRead(saveBtn) == LOW) {
    if (taughtCount < 20) {
      taughtAngles[taughtCount++] = angle;
      Serial.print("Saved position #");
      Serial.println(taughtCount);
      delay(300); // debounce
    }
  }
}

// ------------------ PLAY MODE ------------------
void playLoop() {
  if (taughtCount == 0) return;

  for (int i = 0; i < taughtCount; i++) {

    float cmdAngle = taughtAngles[i];
    myservo.write(cmdAngle);

    // Give servo time to move
    unsigned long tStart = millis();
    while (millis() - tStart < 800) {

      float fbAngle = readPotAngle();

      Serial.print("PLAY | Cmd: ");
      Serial.print(cmdAngle);
      Serial.print(" | Feedback: ");
      Serial.println(fbAngle);

      delay(50);   // feedback update rate
    }
  }
}


// ------------------ MAIN LOOP ------------------
void loop() {

  // ENTER TEACH MODE
  if (digitalRead(teachBtn) == LOW && currentMode != TEACH_MODE) {
    currentMode = TEACH_MODE;
    myservo.detach();   // motor OFF
    Serial.println("== TEACH MODE ==");
    delay(300);
  }

  // ENTER PLAY MODE
  if (digitalRead(playBtn) == LOW && currentMode != PLAY_MODE) {
    currentMode = PLAY_MODE;

    float currentAngle = readPotAngle();
    myservo.attach(servoPin);

    // Move smoothly to first taught position
    if (taughtCount > 0) {
      smoothMove(currentAngle, taughtAngles[0]);
    }

    Serial.println("== PLAY MODE ==");
    delay(300);
  }

  // RUN CURRENT MODE
  if (currentMode == TEACH_MODE) {
    teachLoop();
  }

  if (currentMode == PLAY_MODE) {
    playLoop();
  }
}
