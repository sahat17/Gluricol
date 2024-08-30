#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

// Menggunakan UART hardware ESP32
#define FPSerial Serial2  // Menggunakan Serial2 (GPIO 16 dan GPIO 17 default pada ESP32)
static const uint8_t PIN_MP3_TX = 16; // Connects to module's RX 
static const uint8_t PIN_MP3_RX = 17; // Connects to module's TX 

DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

void setup() {
  FPSerial.begin(9600, SERIAL_8N1, PIN_MP3_RX, PIN_MP3_TX); // Menggunakan pin 16 untuk RX dan 17 untuk TX
  Serial.begin(115200);

  delay(1000); // Tambahkan delay untuk memastikan modul siap

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(FPSerial, true, true)) {  // Gunakan serial untuk komunikasi dengan mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true) {
      delay(0); // Kode untuk kompatibilitas dengan watchdog ESP8266/ESP32.
    }
  }
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.volume(30);  // Setel volume. Dari 0 hingga 30
  myDFPlayer.play(1);  // Mainkan mp3 pertama
}

void loop() {
  if (myDFPlayer.available()) {
    uint8_t type = myDFPlayer.readType();
    int value = myDFPlayer.read();
    printDetail(type, value);

    if (type == DFPlayerPlayFinished) {
      // Lagu selesai diputar, mainkan lagu berikutnya
      myDFPlayer.next();
    }
  }
}

void printDetail(uint8_t type, int value) {
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
