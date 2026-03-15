#include <Servo.h>
Servo myservo;
int potPin = A1;
int buttonPin = 2;
bool teachMode = false;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  myservo.attach(9);
  Serial.begin(9600);
  myservo.write(180);
}

void loop() {
 
}
