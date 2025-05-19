// Bu kod, Arduino'yu bir Modbus RTU slave cihazı olarak yapılandırarak
// RS485 haberleşmesi üzerinden 4 adet float değeri master cihaza
// göndermeyi sağlar. Öncelikle, TXEN pini (2. pin) RS485 yönlendirmesi
// için çıkış olarak ayarlanır ve seri haberleşme 19200 baud hızında
// başlatılır. Slave ID’si 1 olarak belirlenir ve Modbus haberleşmesi
// başlatılır. Ardından, 4 adet float değer (3.14, 431415.56, 9265.0, 1.0)
// 2’şer adet 16-bit register’a bölünerek Modbus register dizisine yazılır.
// Döngü içinde, RS485 modülü veri gönderme moduna alınarak
// (TXEN = HIGH) Modbus haberleşmesi yönetilir ve 8 adet 16-bit
// register master cihaz tarafından okunabilir hale getirilir. 
// Gönderim tamamlandıktan sonra, RS485 alım moduna geçirilir 
// (TXEN = LOW), böylece yeni bir isteğe hazır hale gelir. Aynı zamanda, 
// seri monitörde her register’ın değeri gösterilerek debug
// amaçlı çıktı alınır ve işlem 500 ms aralıklarla tekrar edilir.

//ENDUSTRIYEL ARGE MERKEZİ

#include <ModbusRtu.h>

#define TXEN 2  // RS485 TX Enable pini

// Modbus veri dizisi (4 float için 8 register)
// Her bir float değeri 32-bit (4 byte) olduğu 
// için iki adet 16-bit register gerektirir.
uint16_t au16data[8]; 

// Modbus Slave nesnesi
Modbus slave(1, Serial, TXEN); // Slave ID = 1, 2. pin TX Enable için

// Fonksiyon: Float değerini iki uint16_t'ye dönüştür
void floatToModbusRegisters(float value, uint16_t &reg1, uint16_t &reg2) {
  uint32_t floatAsInt = *(uint32_t*)&value;  // float'ı uint32_t'ye dönüştür
  reg1 = (uint16_t)(floatAsInt >> 16);       // İlk 16 bit
  reg2 = (uint16_t)(floatAsInt & 0xFFFF);    // Sonraki 16 bit
}

void setup() {
  pinMode(TXEN, OUTPUT);
  Serial.begin(19200); // Baud Rate
  slave.start();

  // 4 adet float değeri her biri 2 register'da olmak üzere doldur
  float values[4] = {3.14, 431415.56, 9265.0, 1.0};

  for (int i = 0; i < 4; i++) {
    floatToModbusRegisters(values[i], au16data[2*i], au16data[2*i + 1]); // Her float için 2 register
  }
}

void loop() {
  digitalWrite(TXEN, HIGH);  // Veri gönderirken aç
  slave.poll(au16data, 8);   // 8 register gönderilecek (4 float * 2 register)
  digitalWrite(TXEN, LOW);   // Gönderme bitince kapat

  // Slave’in hangi veriyi gönderdiğini görmek için
  Serial.print("Slave Gönderilen Veri: ");
  for (int i = 0; i < 8; i++) {
    Serial.print(au16data[i]);
    Serial.print(" ");
  }
  Serial.println();
  delay(500);
}
