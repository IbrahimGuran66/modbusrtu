#include <ModbusRtu.h>

// Veri saklamak için dizi
uint16_t au16data[8];  // 4 float için 8 register

// bu kodla 62 farklı float türünde veriyi okuyabilirsiniz,
// biz sadece 4 değer okuduk.
uint8_t u8state;

// Modbus Master nesnesi
Modbus master(0, Serial, 2);  // 2. pin TX Enable için

// Modbus isteği için gerekli bilgileri tutan yapı.
modbus_t telegram;

// Zaman gecikmesi için kullanılan değişken.
unsigned long u32wait;

// Fonksiyon: Modbus register'larını float'a dönüştür
float modbusRegistersToFloat(uint16_t reg1, uint16_t reg2) {
  uint32_t floatAsInt = ((uint32_t)reg1 << 16) | reg2; // 2 register'ı birleştir
  float* value = (float*)&floatAsInt;  // uint32_t'yi float'a dönüştür
  return *value;
}

void setup() {
  Serial.begin(19200); // Baud Rate
  master.start();
  master.setTimeOut(2000); // Timeout süresi (2 saniye)
  u32wait = millis() + 1000; // İlk sorgu için 1 saniyelik bekleme ayarlanıyor.
  u8state = 0; 
}
// loop fonksiyonu 3 aşamadan oluşur: başlangıç gecikmesi, masterın
// okuma isteği göndermesi ve gelen veriyi okuyup ekrana yazdırma işlemi
void loop() {
  switch (u8state) {
    case 0: 
      if (millis() > u32wait) u8state++; // Bekleme süresi
      break;
    case 1: 
      telegram.u8id = 1; // Slave ID = 1
      telegram.u8fct = 3; // Read Holding Registers
      telegram.u16RegAdd = 0; // Register başlangıç adresi
      telegram.u16CoilsNo = 8; // 8 register oku (4 float * 2 register)
      telegram.au16reg = au16data; // Gelen veriyi kaydetmek için

      master.query(telegram); // Sorguyu gönder
      u8state++;
      break;
    case 2:
      master.poll(); // Gelen verileri işle
      if (master.getState() == COM_IDLE) {
        Serial.print("Master Alınan Veri: ");
        for (int i = 0; i < 4; i++) {
          float receivedValue = modbusRegistersToFloat(au16data[2*i], au16data[2*i + 1]); // 2 register'dan float'a dönüştür
          Serial.print(receivedValue);
          Serial.print(" ");
        }
        Serial.println();
        u8state = 0;
        u32wait = millis() + 100; 
      }
      break;
  }
}
