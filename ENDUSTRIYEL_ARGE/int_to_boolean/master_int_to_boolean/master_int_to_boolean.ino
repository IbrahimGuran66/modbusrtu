// Bu kodda int türünde slave'den gelen 3 farklı veri slavede boolean
// türü veriye çevrilip tek registerla mastera gönderilmiştir.


#include <ModbusRtu.h>

uint16_t au16data[1];  // Tek register okuyacağız
uint8_t u8state;
Modbus master(0, Serial, 2);  // TX Enable pini = 2

modbus_t telegram;
unsigned long u32wait;

void setup() {
  Serial.begin(19200);
  master.start();
  master.setTimeOut(2000);
  u32wait = millis() + 1000;
  u8state = 0;
}

void loop() {
  switch (u8state) {
    case 0: 
      if (millis() > u32wait) u8state++;  
      break;

    case 1: 
      telegram.u8id = 1; // Slave ID = 1
      telegram.u8fct = 3; // Read Holding Registers
      telegram.u16RegAdd = 0; // Başlangıç adresi
      telegram.u16CoilsNo = 1; // Tek register oku
      telegram.au16reg = au16data; // Gelen veriyi kaydet

      master.query(telegram);
      u8state++;
      break;

    case 2:
      master.poll();
      if (master.getState() == COM_IDLE) {
        Serial.print("Master Alınan Veri (Register): ");
        Serial.println(au16data[0], BIN); // Binary formatta yazdır

        // Register'daki 3 boolean değeri çıkar
        bool boolValues[3];
        for (int i = 0; i < 3; i++) {
          boolValues[i] = (au16data[0] >> i) & 1;  // i. biti al
        }

        Serial.print("Çözülen Boolean Değerler: ");
        for (int i = 0; i < 3; i++) {
          Serial.print(boolValues[i]);
          Serial.print(" ");
        }
        Serial.println();

        u8state = 0;
        u32wait = millis() + 100;
      }
      break;
  }
}
