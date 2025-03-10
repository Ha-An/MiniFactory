#define IN1 8  // 모터 드라이버 IN1
#define IN2 9  // 모터 드라이버 IN2
#define ENA 10 // 모터 속도 제어 (PWM 핀)

// 기본 설정
const int minTime = 20000;  // 최소 동작 시간 (20초)
int processTime = minTime;  // 기본 동작 시간
int moveTime = 9750;        // 1cm 이동하는 시간
int delayTime = 500;        // 점 찍는 시간 (초기값)
int manualMoveTime = 500;   // 수동 이동 시간 (약 1mm)

// 반복 동작 여부
bool running = false; 
bool stopRequested = false;  // 정지 요청 상태

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  
  Serial.begin(9600); // 시리얼 통신 시작
  
  analogWrite(ENA, 255); // 모터 속도 최대
  
  Serial.println("명령어:");
  Serial.println("'up' - 펜을 위로 이동");
  Serial.println("'down' - 펜을 아래로 이동");
  Serial.println("숫자 입력 - 해당 시간 동안 동작 (예: 0 = 20초, 1000 = 21초, 10000 = 30초)");
  Serial.println("'stop' - 즉시 동작 정지");
}

void loop() {
  // 사용자가 명령어 입력
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');  
    command.trim();  

    if (command.equalsIgnoreCase("up")) {
      Serial.println("펜 위로 이동");
      moveUp(manualMoveTime);
    } 
    else if (command.equalsIgnoreCase("down")) {
      Serial.println("펜 아래로 이동");
      moveDown(manualMoveTime);
    } 
    else if (command.equalsIgnoreCase("stop")) {
      Serial.println("정지 요청됨!");
      stopRequested = true;  // stop 신호 설정
    }
    else if (isNumber(command)) { // 숫자 입력이면 실행
      int extraTime = command.toInt();  
      processTime = minTime + extraTime;  // 최소시간(20초) + 추가 시간
      delayTime = 500 + extraTime;  // 입력된 숫자만큼 점 찍는 시간 증가

      Serial.print("설정된 동작 시간: ");
      Serial.print(processTime / 1000);
      Serial.println(" 초");

      Serial.print("점 찍는 시간: ");
      Serial.print(delayTime / 1000);
      Serial.println(" 초");

      stopRequested = false; // stop 상태 초기화
      runProcess();  // 동작 시작
    } 
    else {
      Serial.println("올바른 명령을 입력하세요: 'up', 'down', 숫자, 'stop'");
    }
  }
}

// 숫자인지 확인하는 함수
bool isNumber(String str) {
  for (int i = 0; i < str.length(); i++) {
    if (!isDigit(str[i])) return false;
  }
  return true;
}

// 펜 위로 이동
void moveUp(int duration) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  delay(duration);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

// 펜 아래로 이동
void moveDown(int duration) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  delay(duration);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

// 지정된 시간 동안 동작하는 함수 (stop 요청 반영)
void runProcess() {
  Serial.println("동작 시작!");
  long startTime = millis();

  while ((millis() - startTime < processTime) && !stopRequested) {
    // 1️⃣ 펜 내려가기 (점 찍을 위치로 이동)
    moveDown(moveTime);
    
    // 2️⃣ 점 찍기 (사용자 입력에 따라 대기 시간 조정)
    Serial.print("점 찍는 중... ");
    Serial.print(delayTime / 1000);
    Serial.println("초 대기");
    delay(delayTime);
    
    // 3️⃣ 펜 올라가기 (원래 위치로 복귀)
    moveUp(moveTime);
    
    // 4️⃣ 정지 (다음 동작을 위해 대기)
    Serial.println("대기 중...");
    delay(delayTime);
  }

  Serial.println("동작 완료. 새로운 명령을 입력하세요.");
  stopRequested = false; // stop 요청 초기화
}
