this is my existing working code
#include <Arduino.h>

// New motor pin assignments (from working code)
const uint8_t AIN1 = 7;  // Left Motor Direction 1
const uint8_t AIN2 = 8;  // Left Motor Direction 2
const uint8_t PWMA = 5;  // Left Motor Speed (PWM)

const uint8_t BIN1 = 4;  // Right Motor Direction 1
const uint8_t BIN2 = 9;  // Right Motor Direction 2
const uint8_t PWMB = 6;  // Right Motor Speed (PWM)

String input = "";

void setup() {
  Serial.begin(9600);

  // Setup pins
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);

  // Stop motors initially
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
}

void loop() {
  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n' || c == '\r' || c == ';') {
      parseAndExecute(input);
      input = "";
    } else {
      input += c;
    }
  }
}

void parseAndExecute(String cmd) {
  int sep = cmd.indexOf(':');
  if (sep != -1 && cmd.length() >= 6) {
    String leftCmd = cmd.substring(0, sep);     // e.g., LF5
    String rightCmd = cmd.substring(sep + 1);   // e.g., RB9
    controlMotor(leftCmd, 'L');
    controlMotor(rightCmd, 'R');
  }
}

void controlMotor(String cmd, char side) {
  if (cmd.length() != 3) return;

  char dir = cmd[1];
  int speed = (cmd[2] - '0') * 28;  // Map 0–9 to 0–252 PWM

  if (side == 'L') {
    if (dir == 'F') {
      digitalWrite(AIN1, HIGH);
      digitalWrite(AIN2, LOW);
    } else if (dir == 'B') {
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, HIGH);
    } else {
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, LOW);
    }
    analogWrite(PWMA, speed);
  }

  if (side == 'R') {
    if (dir == 'F') {
      digitalWrite(BIN1, HIGH);
      digitalWrite(BIN2, LOW);
    } else if (dir == 'B') {
      digitalWrite(BIN1, LOW);
      digitalWrite(BIN2, HIGH);
    } else {
      digitalWrite(BIN1, LOW);
      digitalWrite(BIN2, LOW);
    }
    analogWrite(PWMB, speed);
  }
}
