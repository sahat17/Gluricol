#include "DFPlayerMini_Fast.h"

DFPlayerMini_Fast myDFPlayer;

int sensorPin = 34; // Pin sensor yang terhubung ke ESP32
int sensorValue;    // Variabel untuk menyimpan nilai sensor
bool isMale = true; // True jika pria, False jika wanita
byte glukosa;
byte uricAcid;
byte kolestrol;

void setup() {
  Serial.begin(9600);
  myDFPlayer.begin(Serial);

  pinMode(sensorPin, INPUT); // Set pin sensor sebagai input
}

void loop() {
  sensorValue = analogRead(sensorPin); // Baca nilai dari sensor

  // Asumsikan bahwa nilai dari sensor sudah dikonversi atau sesuai dengan
  // rentang nilai yang dibutuhkan untuk gula darah, kolesterol, dan asam urat.
  float voltage = sensorValue * (3.3 / 4095);  // Menghitung tegangan dari nilai ADC

  // Menghitung kadar glukosa darah dalam mg/dl
  glukosa = (((-64.641 * voltage) + 300.17) - 22);

  // Menghitung kadar asam urat dalam mg/dl (disamakan dengan pendekatan gula darah)
  uricAcid = (((-64.641 * voltage) + 300.17) - 22);

  // Menghitung kadar kolesterol dalam mg/dl (disamakan dengan pendekatan gula darah)
  kolestrol = (((-64.641 * voltage) + 300.17) - 22);
  
  // Misalnya:
  byte bloodGlucoseFasting = glukosa; // Nilai sensor untuk gula darah puasa
  byte bloodGlucosePostMeal = glukosa; // Nilai sensor untuk gula darah 2 jam setelah makan
  byte randomBloodGlucose = glukosa; // Nilai sensor untuk gula darah acak
  byte cholesterol = kolestrol; // Nilai sensor untuk kolesterol
  //float uricAcid = uricAcid / 100.0; // Nilai sensor untuk asam urat (jika dalam format float)

  // Cek Gula Darah Puasa
  if (bloodGlucoseFasting >= 70 && bloodGlucoseFasting <= 99) {
    myDFPlayer.play(1); // Mainkan file untuk Gula Darah Puasa Normal
  } else if (bloodGlucoseFasting >= 100 && bloodGlucoseFasting <= 125) {
    myDFPlayer.play(2); // Mainkan file untuk Gula Darah Puasa Pra-diabetes
  } else if (bloodGlucoseFasting >= 126) {
    myDFPlayer.play(3); // Mainkan file untuk Gula Darah Puasa Diabetes
  }

  // Cek Gula Darah 2 Jam Setelah Makan
  if (bloodGlucosePostMeal < 140) {
    myDFPlayer.play(4); // Mainkan file untuk Gula Darah 2 Jam Setelah Makan Normal
  } else if (bloodGlucosePostMeal >= 140 && bloodGlucosePostMeal <= 199) {
    myDFPlayer.play(5); // Mainkan file untuk Gula Darah 2 Jam Setelah Makan Pra-diabetes
  } else if (bloodGlucosePostMeal >= 200) {
    myDFPlayer.play(6); // Mainkan file untuk Gula Darah 2 Jam Setelah Makan Diabetes
  }

  // Cek Gula Darah Acak
  if (randomBloodGlucose < 140) {
    myDFPlayer.play(7); // Mainkan file untuk Gula Darah Acak Normal
  } else if (randomBloodGlucose >= 200) {
    myDFPlayer.play(8); // Mainkan file untuk Gula Darah Acak Diabetes
  }

  // Cek Kolesterol
  if (cholesterol < 200) {
    myDFPlayer.play(9); // Mainkan file untuk Kolesterol Normal
  } else if (cholesterol >= 200 && cholesterol <= 239) {
    myDFPlayer.play(10); // Mainkan file untuk Kolesterol Batas Tinggi
  } else if (cholesterol >= 240) {
    myDFPlayer.play(11); // Mainkan file untuk Kolesterol Tinggi
  }

  // Cek Asam Urat (berdasarkan jenis kelamin)
  if (isMale) {
    if (uricAcid >= 3.4 && uricAcid <= 7.0) {
      myDFPlayer.play(12); // Mainkan file untuk Asam Urat Normal pada Pria
    }
  } else {
    if (uricAcid >= 2.4 && uricAcid <= 6.0) {
      myDFPlayer.play(13); // Mainkan file untuk Asam Urat Normal pada Wanita
    }
  }
  
  delay(1000); // Tunda sejenak sebelum membaca ulang
}
