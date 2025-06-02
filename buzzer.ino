#dfplayer olmadan, distance ve rtc kullanıyor + BUZZER VAR


#include <Wire.h>
#include <RTClib.h>

#define trigPin 7
#define echoPin 6
#define buttonPin 4
#define buzzerPin 9

long duration;
int distance;

RTC_DS3231 rtc;

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  if (!rtc.begin()) {
    Serial.println("RTC bulunamadi!");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC ayarlaniyor...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  Serial.println("Sistem hazir.");
}

void loop() {
  // HC-SR04 mesafe ölçümü
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Mesafe: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance > 0 && distance < 50) {
    // Engel varsa buzzer uyarısı
    digitalWrite(buzzerPin, HIGH);
    delay(500);
    digitalWrite(buzzerPin, LOW);
    delay(500);
  }

  // Buton kontrolü
  if (digitalRead(buttonPin) == LOW) {
    DateTime now = rtc.now();

    Serial.print("Saat: ");
    if (now.hour() < 10) Serial.print('0');
    Serial.print(now.hour());
    Serial.print(':');
    if (now.minute() < 10) Serial.print('0');
    Serial.print(now.minute());
    Serial.print(':');
    if (now.second() < 10) Serial.print('0');
    Serial.println(now.second());

    // Saat bildirimi yerine beep-beep
    for (int i = 0; i < 2; i++) {
      digitalWrite(buzzerPin, HIGH);
      delay(200);
      digitalWrite(buzzerPin, LOW);
      delay(200);
    }

    delay(1000); // Buton debounce
  }

  delay(200);
}
