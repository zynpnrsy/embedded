#rtc ve distance sensörü birlikte

#include <Wire.h>
#include <RTClib.h>

#define trigPin 7
#define echoPin 6
#define buttonPin 4

long duration;
int distance;

RTC_DS3231 rtc;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  Serial.begin(9600);

  if (!rtc.begin()) {
    Serial.println("RTC not found!");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting time...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  // Measure distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // Get current time
  DateTime now = rtc.now();

  // Print distance and time
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm | Time: ");
  if (now.hour() < 10) Serial.print('0');
  Serial.print(now.hour());
  Serial.print(':');
  if (now.minute() < 10) Serial.print('0');
  Serial.print(now.minute());
  Serial.print(':');
  if (now.second() < 10) Serial.print('0');
  Serial.println(now.second());

  // When button is pressed, repeat the time
  if (digitalRead(buttonPin) == LOW) {
    Serial.println("Button pressed - Time is:");
    Serial.print("Time: ");
    if (now.hour() < 10) Serial.print('0');
    Serial.print(now.hour());
    Serial.print(':');
    if (now.minute() < 10) Serial.print('0');
    Serial.print(now.minute());
    Serial.print(':');
    if (now.second() < 10) Serial.print('0');
    Serial.println(now.second());
    delay(1000); // debounce
  }

  delay(1000); // loop delay
}
