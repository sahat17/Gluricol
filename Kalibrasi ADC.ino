int sensor = 34;  // Pin sensor pada ESP32

int glukosa;      // Variabel untuk kadar glukosa darah
int uricAcid;     // Variabel untuk kadar asam urat
int cholesterol;  // Variabel untuk kadar kolesterol

void setup() {
  Serial.begin(9600);      // Inisialisasi komunikasi serial dengan baud rate 9600
  pinMode(sensor, INPUT);  // Mengatur pin sensor sebagai input
}

void loop() {
  int a = analogRead(sensor);        // Membaca nilai ADC dari sensor
  float voltage = a * (3.3 / 4095);  // Menghitung tegangan dari nilai ADC

  // Menghitung kadar glukosa darah dalam mg/dl
  glukosa = (((-64.641 * voltage) + 188.17) - 22);

  // Menghitung kadar asam urat dalam mg/dl (disamakan dengan pendekatan gula darah)
  uricAcid = (((-64.641 * voltage) + 188.17) - 22);

  // Menghitung kadar kolesterol dalam mg/dl (disamakan dengan pendekatan gula darah)
  cholesterol = (((-64.641 * voltage) + 188.17) - 22);

  if (glukosa && uricAcid && cholesterol < 0) {
    glukosa = 0;  // Jika hasil negatif, set ke 0
    uricAcid = 0;
    cholesterol = 0;
  }
  // Menampilkan hasil di Serial Monitor
String labels[] = {"Kadar glukosa: ", "Kadar asam urat: ", "Kadar kolesterol: ", "Tegangan sensor: "};
  float values[] = {glukosa, uricAcid, cholesterol, voltage};
  String units[] = {" mg/dl", " mg/dl", " mg/dl", " V"};

  for (int i = 0; i < 4; i++) {
    Serial.print(labels[i]);
    Serial.print(values[i]);
    Serial.println(units[i]);
  }

  delay(1000);  // Menunggu 1 detik sebelum pembacaan berikutnya
}
