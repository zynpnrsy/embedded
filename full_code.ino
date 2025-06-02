# bu kodda tüm modüller var. dfplayer,rtc,distance,buton.




#include <Wire.h>
#include <RTClib.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

#define trigPin 7
#define echoPin 6
#define buttonPin 4
#define dfPlayerTx 10  // Arduino RX
#define dfPlayerRx 11  // Arduino TX

long duration;
int distance;

RTC_DS3231 rtc;
SoftwareSerial mySerial(dfPlayerTx, dfPlayerRx);
DFRobotDFPlayerMini dfPlayer;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  Serial.begin(9600);
  mySerial.begin(9600);

  if (!rtc.begin()) {
    Serial.println("RTC bulunamadi!");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC saati ayarlanıyor...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  if (!dfPlayer.begin(mySerial)) {
    Serial.println("DFPlayer baslatilamadi!");
    while (1);
  }
  dfPlayer.volume(20);  // Ses seviyesi 0-30
}

void loop() {
  // Mesafe ölçümü
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // 50 cm altındaysa 001.mp3 çal (engeli uyar)
  if (distance > 0 && distance < 50) {
    dfPlayer.play(1); // 001.mp3 dosyası
    delay(2000);      // ses bitene kadar bekle
  }

  // Butona basıldığında saati hem seri monitöre yaz hem de sesli söyle
  if (digitalRead(buttonPin) == LOW) {
    DateTime now = rtc.now();

    // Seri monitöre yazdır
    Serial.print("Saat: ");
    if (now.hour() < 10) Serial.print('0');
    Serial.print(now.hour());
    Serial.print(':');
    if (now.minute() < 10) Serial.print('0');
    Serial.print(now.minute());
    Serial.print(':');
    if (now.second() < 10) Serial.print('0');
    Serial.println(now.second());

    // Saat için uygun mp3'leri çal
    // Örnek: saat dosyaları 11.mp3 (saat 1), 12.mp3 (saat 2), ... (varsayım)
    // dakika dosyaları 51.mp3, 52.mp3, ... (varsayım)
    // Burada hour + 10 ve minute + 50 örneği kullanılıyor:
    dfPlayer.play(now.hour() + 10);   // saat dosyası, 11.mp3 = 1 o’clock gibi
    delay(1500);
    dfPlayer.play(now.minute() + 50); // dakika dosyası, 51.mp3 = 1 dakika gibi
    delay(1500);

    delay(1000); // Buton debounce
  }

  delay(200);
}
