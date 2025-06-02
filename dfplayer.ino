#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

#define dfPlayerTx 10  // Arduino RX
#define dfPlayerRx 11  // Arduino TX

SoftwareSerial mySerial(dfPlayerTx, dfPlayerRx);
DFRobotDFPlayerMini dfPlayer;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  Serial.println("DFPlayer test basladi...");

  if (!dfPlayer.begin(mySerial)) {
    Serial.println("DFPlayer baslatilamadi!");
    while (1);
  }

  dfPlayer.volume(20);
  Serial.println("Ses ayarlandi. 001.mp3 caliyor...");
  dfPlayer.play(1);  // 001.mp3
}

void loop() {
}
