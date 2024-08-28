#include <WiFi.h>
#include "DFRobotDFPlayerMini.h"
#include "Arduino.h"

#define FPSerial Serial2 //penggunaan UART2 alias rx pin 16 dan tx pin 17
int sensor = 34;  // Pin sensor pada ESP32
int glukosa;  // Variabel untuk kadar glukosa darah
int uricAcid;  // Variabel untuk kadar asam urat
int cholesterol;  // Variabel untuk kadar kolesterol

DFRobotDFPlayerMini myDFPlayer;

void setup() {
  FPSerial.begin(9600);  //memulai Serial2
  Serial.begin(115200);  // Inisialisasi komunikasi serial dengan baud rate 9600
  pinMode(sensor, INPUT);  // Mengatur pin sensor sebagai input
  myDFPlayer.volume(30);
}

void loop() {
  
  int a = analogRead(sensor);  // Membaca nilai ADC dari sensor
  float voltage = a * (3.3 / 4095);  // Menghitung tegangan dari nilai ADC

  // Menghitung kadar glukosa darah dalam mg/dl
  glukosa = (((-64.641 * voltage) + 300.17) - 22);

  // Menghitung kadar asam urat dalam mg/dl (disamakan dengan pendekatan gula darah)
  uricAcid = (((-64.641 * voltage) + 300.17) - 22);

  // Menghitung kadar kolesterol dalam mg/dl (disamakan dengan pendekatan gula darah)
  cholesterol = (((-64.641 * voltage) + 300.17) - 22);

  if (gulaDarah < 100 && cholesterol < 200) {
    myDFPlayer.play(1);  // Mainkan file 001_normal.mp3
  } else if (gulaDarah >= 100) {
    myDFPlayer.play(2);  // Mainkan file 002_guladarah.mp3
  } else if (cholesterol>= 200) {
    myDFPlayer.play(3);  // Mainkan file 003_kolesterol.mp3
  }

  delay(5000);  // Tunggu beberapa saat sebelum pengukuran berikutnya
}

  if (glukosa < 0) {
    glukosa = 0;  // Jika hasil negatif, set ke 0
  }

  if (uricAcid < 0) {
    uricAcid = 0;  // Jika hasil negatif, set ke 0
  }

  if (cholesterol < 0) {
    cholesterol = 0;  // Jika hasil negatif, set ke 0
  }

  // Menampilkan hasil di Serial Monitor
  Serial.print("Kadar glukosa: ");
  Serial.print(glukosa);
  Serial.println(" mg/dl");

  Serial.print("Kadar asam urat: ");
  Serial.print(uricAcid);
  Serial.println(" mg/dl");

  Serial.print("Kadar kolesterol: ");
  Serial.print(cholesterol);
  Serial.println(" mg/dl");

  Serial.print("Tegangan sensor: ");
  Serial.print(voltage);
  Serial.println(" V");

  delay(1000);  // Menunggu 1 detik sebelum pembacaan berikutnya
}
