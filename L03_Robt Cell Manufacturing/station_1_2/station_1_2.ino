// 모터 A 핀 정의
const int ENA = 6;
const int IN1 = 7;
const int IN2 = 8;

// 모터 B 핀 정의
const int ENB = 9;
const int IN3 = 10;
const int IN4 = 11;

// 초음파 센서 핀 정의
const int trigPinA = 2, echoPinA = 3;  // 모터 A 센서
const int trigPinB = 4, echoPinB = 5;  // 모터 B 센서

// 거리 측정 변수
long durationA, distanceA;
long durationB, distanceB;

// 모터 상태 변수
bool motorARunning = false, motorAReversing = false;
bool motorBRunning = false, motorBReversing = false;

unsigned long motorRunStartTimeA = 0;
unsigned long motorRunStartTimeB = 0;

const unsigned long motorRunDuration = 15000; // 15초 (전진/후진 각각)

void setup() {
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  pinMode(trigPinA, OUTPUT); pinMode(echoPinA, INPUT);
  pinMode(trigPinB, OUTPUT); pinMode(echoPinB, INPUT);

  Serial.begin(115200);  // 🔹 시리얼 통신 속도 변경
}

void loop() {
  measureDistance(trigPinA, echoPinA, durationA, distanceA);
  measureDistance(trigPinB, echoPinB, durationB, distanceB);

  Serial.print("A: "); Serial.print(distanceA); Serial.print(" cm | ");
  Serial.print("B: "); Serial.print(distanceB); Serial.println(" cm");

  unsigned long currentTime = millis();

  // 모터 A 제어
  controlMotor(distanceA, motorARunning, motorAReversing, motorRunStartTimeA, ENA, IN1, IN2, currentTime);
  // 모터 B 제어
  controlMotor(distanceB, motorBRunning, motorBReversing, motorRunStartTimeB, ENB, IN3, IN4, currentTime);

  // 🔹 모터 속도 측정 코드 추가
  int motor_speed = analogRead(A0);
  Serial.print("Motor Speed: ");
  Serial.println(motor_speed);

  delay(100);
}

// 초음파 거리 측정 함수
void measureDistance(int trig, int echo, long &duration, long &distance) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);

  if (duration == 0) {
    distance = 999;  // 🛑 초음파 센서 오류 방지 (비정상 값 처리)
  } else {
    distance = (duration / 2) * 0.0344;
  }
}

// 모터 제어 함수 (공통)
void controlMotor(long distance, bool &motorRunning, bool &motorReversing, unsigned long &motorRunStartTime, int EN, int IN1, int IN2, unsigned long currentTime) {
  if (motorRunning) {
    if (currentTime - motorRunStartTime > motorRunDuration) {
      if (!motorReversing) {
        motorReversing = true;
        motorRunStartTime = currentTime;
        motorReverse(EN, IN1, IN2, 255);  // 후진 (최대 속도)
      } else {
        motorRunning = false;
        motorReversing = false;
        motorStop(EN, IN1, IN2);
        Serial.println("Motor stopped. Waiting for next trigger.");
        delay(500);
      }
    }
    return;
  }

  if (distance > 0 && distance < 7 && !motorRunning) {  // 7cm 이하 감지 시 동작
    Serial.println("Object detected! Motor starting...");
    motorRunning = true;
    motorReversing = false;
    motorRunStartTime = currentTime;
    motorForward(EN, IN1, IN2, 255);  // 전진 (최대 속도)
  }
}

// 모터 제어 함수
void motorForward(int EN, int IN1, int IN2, int speed) { 
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(EN, speed); 
  Serial.println("Motor moving forward.");
}

void motorReverse(int EN, int IN1, int IN2, int speed) { 
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH); analogWrite(EN, speed); 
  Serial.println("Motor moving backward.");
}

void motorStop(int EN, int IN1, int IN2) { 
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW); analogWrite(EN, 0); 
  Serial.println("Motor stopped.");
}
