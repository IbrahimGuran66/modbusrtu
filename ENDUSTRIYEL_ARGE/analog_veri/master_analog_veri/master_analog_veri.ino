#include <ModbusRtu.h>

uint16_t au16data[1]; // Sadece 1 register kullanacağız
uint8_t u8state;

Modbus master(0, Serial, 2); // TX Enable pini için 2 numaralı pin

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
      telegram.u8id = 1;
      telegram.u8fct = 3; // Read Holding Registers
      telegram.u16RegAdd = 0; // Başlangıç adresi
      telegram.u16CoilsNo = 1; // Sadece 1 register oku
      telegram.au16reg = au16data;

      master.query(telegram);
      u8state++;
      break;
    case 2:
      master.poll();
      if (master.getState() == COM_IDLE) {
        Serial.print("Master Alınan Veri: ");
        Serial.println(au16data[0]);
        u8state = 0;
        u32wait = millis() + 100;
      }
      break;
  }
}
