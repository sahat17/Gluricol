int sensor = 34;  // Pin sensor pada ESP32

float glukosa;    // Variabel untuk kadar glukosa darah
float uricAcid;   // Variabel untuk kadar asam urat
float cholesterol; // Variabel untuk kadar kolesterol

void setup() {
  Serial.begin(9600);  // Inisialisasi komunikasi serial dengan baud rate 9600
  pinMode(sensor, INPUT);  // Mengatur pin sensor sebagai input
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

  // Menampilkan hasil di Serial Monitor
  Serial.println("=== Hasil Pengukuran ===");
  
  // Gula Darah
  Serial.print("Kadar glukosa: ");
  Serial.print(glukosa);
  Serial.print(" mg/dL - ");
  Serial.println(glukosa < 100 ? "Normal" : "Tidak Normal");

  // Asam Urat
  Serial.print("Kadar asam urat: ");
  Serial.print(uricAcid);
  Serial.print(" mg/dL - ");
  Serial.println(uricAcid < 7.0 ? "Normal" : "Tidak Normal");

  // Kolesterol
  Serial.print("Kadar kolesterol: ");
  Serial.print(cholesterol);
  Serial.print(" mg/dL - ");
  Serial.println(cholesterol < 200 ? "Normal" : "Tidak Normal");

  // Tegangan Sensor
  Serial.print("Tegangan sensor: ");
  Serial.print(voltage);
  Serial.println(" V");

  Serial.println(); // Baris kosong untuk memisahkan setiap pembacaan

  delay(1000);  // Menunggu 1 detik sebelum pembacaan berikutnya
}
