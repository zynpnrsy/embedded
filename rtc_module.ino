#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  Serial.println("RTC test basladi...");

  if (!rtc.begin()) {
    Serial.println("RTC bulunamadi!");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC saati ayarlaniyor...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
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

  delay(1000);
}
