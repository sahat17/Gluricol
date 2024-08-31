#include <TFT_eSPI.h>    // Graphics and font library for ILI9341 driver chip
#include <SPI.h>
#include <DFRobotDFPlayerMini.h>

#define TFT_GREY 0x5AEB // New colour
#define TFT_RED_ORANGE 0xFBA0 // Custom color for red-orange background

#define FPSerial Serial2  // Menggunakan Serial2 (GPIO 16 dan GPIO 17 default pada ESP32)
static const uint8_t PIN_MP3_TX = 16; // Connects to module's RX 
static const uint8_t PIN_MP3_RX = 17; // Connects to module's TX 
TFT_eSPI tft = TFT_eSPI();  // Invoke library
DFRobotDFPlayerMini myDFPlayer;// Create DFPlayer object
void printDetail(uint8_t type, int value);

int sensor = 34;  // Pin sensor pada ESP32

float glukosa;    // Variabel untuk kadar glukosa darah
float uricAcid;   // Variabel untuk kadar asam urat
float cholesterol; // Variabel untuk kadar kolesterol

void setup() {
  
  FPSerial.begin(9600, SERIAL_8N1, PIN_MP3_RX, PIN_MP3_TX); // Menggunakan pin 16 untuk RX dan 17 untuk TX
  Serial.begin(115200);  // Inisialisasi komunikasi serial dengan baud rate 9600
  
  delay(1000); // Tambahkan delay untuk memastikan modul siap
  
  pinMode(sensor, INPUT);  // Mengatur pin sensor sebagai input
  tft.init();
  tft.setRotation(2);

  // Initialize DFPlayer Mini
  if (!myDFPlayer.begin(Serial2)) {  // Mulai komunikasi dengan DFPlayer Mini
    Serial.println("DFPlayer Mini gagal diinisialisasi!");
    while (true);  // Berhenti jika gagal
  }
  
  Serial.println("DFPlayer Mini online.");
  myDFPlayer.volume(20);  // Set volume to a reasonable level
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
  int audioTrack = 0; // Track number for DFPlayer

  if (title == "GULA DARAH") {
    if (value < 100) {
      status = "NORMAL";
      audioTrack = 1;  // Track 1: Audio untuk glukosa normal
    } else {
      status = "TIDAK NORMAL";
      audioTrack = 2;  // Track 2: Audio untuk glukosa tidak normal
    }
  } else if (title == "ASAM URAT") {
    if (value < 7.0) {
      status = "NORMAL";
      audioTrack = 3;  // Track 3: Audio untuk asam urat normal
    } else {
      status = "TIDAK NORMAL";
      audioTrack = 4;  // Track 4: Audio untuk asam urat tidak normal
    }
  } else if (title == "KOLESTEROL") {
    if (value < 200) {
      status = "NORMAL";
      audioTrack = 5;  // Track 5: Audio untuk kolesterol normal
    } else {
      status = "TIDAK NORMAL";
      audioTrack = 6;  // Track 6: Audio untuk kolesterol tidak normal
    }
  }

  // Mainkan audio sesuai status
  myDFPlayer.play(audioTrack);

  tft.setTextColor(TFT_PINK, TFT_GREY);
  tft.drawCentreString(status, posX, posY + 40, 2);
}
