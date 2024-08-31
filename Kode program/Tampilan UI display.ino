#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>

#define TFT_GREY 0x5AEB // New colour
#define TFT_RED_ORANGE 0xFBA0 // Custom color for red-orange background

TFT_eSPI tft = TFT_eSPI();  // Invoke library

int sensor = 34;  // Pin sensor pada ESP32

float glukosa;    // Variabel untuk kadar glukosa darah
float uricAcid;   // Variabel untuk kadar asam urat
float cholesterol; // Variabel untuk kadar kolesterol

void setup() {
  Serial.begin(9600);  // Inisialisasi komunikasi serial dengan baud rate 9600
  pinMode(sensor, INPUT);  // Mengatur pin sensor sebagai input
  tft.init();
  tft.setRotation(2);
}

void loop() {
  int a = analogRead(sensor);  // Membaca nilai ADC dari sensor
  float voltage = a * (3.3 / 4095);  // Menghitung tegangan dari nilai ADC

  // Menghitung kadar glukosa darah dalam mg/dl
  glukosa = (((-64.641 * voltage) + 188.17) - 22);

  // Menghitung kadar asam urat dalam mg/dl (disamakan dengan pendekatan gula darah)
  uricAcid = (((-64.641 * voltage) + 188.17) - 22);

  // Menghitung kadar kolesterol dalam mg/dl (disamakan dengan pendekatan gula darah)
  cholesterol = (((-64.641 * voltage) + 188.17) - 22);

  if (glukosa < 0) glukosa = 0;
  if (uricAcid < 0) uricAcid = 0;
  if (cholesterol < 0) cholesterol = 0;

  // Membersihkan layar
  tft.fillScreen(TFT_GREY);
  
  // Setel ukuran teks
  tft.setTextFont(2);

  // Fungsi untuk menampilkan parameter
  displayParameter("GULA DARAH", glukosa, 120, 30);
  displayParameter("KOLESTEROL", cholesterol, 120, 140);
  displayParameter("ASAM URAT", uricAcid, 120, 250);

  delay(1000);  // Menunggu 1 detik sebelum pembacaan berikutnya
}

void displayParameter(String title, float value, int posX, int posY) {
  // Tampilkan judul parameter
  tft.setTextColor(TFT_BLACK, TFT_GREY);
  tft.drawCentreString(title, posX, posY - 20, 4);

  // Tampilkan latar belakang nilai
  tft.fillRoundRect(posX - 60, posY, 120, 50, 10, TFT_RED_ORANGE);

  // Tampilkan nilai parameter
  tft.setTextColor(TFT_YELLOW, TFT_RED_ORANGE);
  tft.drawCentreString(String(value, 1), posX, posY + 10, 4);

  // Tampilkan satuan
  tft.setTextColor(TFT_BLACK, TFT_GREY);
  tft.drawString("mg/dL", posX + 70, posY + 15, 2);

  // Tampilkan status normal/tidak normal
  String status = "";
  if (title == "GULA DARAH") {
    status = value < 100 ? "NORMAL" : "TIDAK NORMAL";
  } else if (title == "ASAM URAT") {
    status = value < 7.0 ? "NORMAL" : "TIDAK NORMAL";
  } else if (title == "KOLESTEROL") {
    status = value < 200 ? "NORMAL" : "TIDAK NORMAL";
  }
  tft.setTextColor(TFT_PINK, TFT_GREY);
  tft.drawCentreString(status, posX, posY + 40, 2);
}
