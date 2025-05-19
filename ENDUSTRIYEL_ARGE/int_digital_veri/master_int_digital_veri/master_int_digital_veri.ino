#include <ModbusRtu.h>

// Veri saklamak için dizi
uint16_t au16data[16]; 
uint8_t u8state;

// Modbus Master nesnesi
Modbus master(0, Serial, 2);  // 2. pin TX Enable için

// Modbus isteği için yapı
modbus_t telegram;

unsigned long u32wait;

void setup() {
  Serial.begin(19200); // Baud Rate
  master.start();
  master.setTimeOut(2000); // Timeout süresi (2 saniye)
  u32wait = millis() + 1000;
  u8state = 0; 
}

void loop() {
  switch (u8state) {
    case 0: 
      if (millis() > u32wait) u8state++; // Bekleme süresi
      break;
    case 1: 
      telegram.u8id = 1; // Slave ID = 1
      telegram.u8fct = 3; // Read Holding Registers
      telegram.u16RegAdd = 0; // **Register başlangıç adresi**
      telegram.u16CoilsNo = 4; // **4 kayıt oku**
      telegram.au16reg = au16data; // Gelen veriyi kaydetmek için

      master.query(telegram); // Sorguyu gönder
      u8state++;
      break;
    case 2:
      master.poll(); // Gelen verileri işle
      if (master.getState() == COM_IDLE) {
        Serial.print("Master Alınan Veri: ");
        for (int i = 0; i < 4; i++) {
          Serial.print(au16data[i]);
          Serial.print(" ");
        }
        Serial.println();
        u8state = 0;
        u32wait = millis() + 100; 
      }
      break;
  }
}
