#include <ModbusRtu.h>
#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;  // RTC modülü nesnesi

#define TXEN 2  // RS485 TX Enable pini

uint16_t au16data[6];  // RTC verileri için 6 register
Modbus master(0, Serial, TXEN);  // Master ID = 0, TX Enable pini 2

void setup() {
  Serial.begin(19200); // Baud Rate
  master.start();
  master.setTimeOut(2000); // Timeout süresi (2 saniye)

  // RTC modülünü başlat
  if (!rtc.begin()) {
    Serial.println("RTC bulunamadi!");
    while (1);
  }
}

void loop() {
  // Modbus isteği yapılandırması
  modbus_t telegram;
  telegram.u8id = 1;  // Slave ID = 1
  telegram.u8fct = 3;  // Read Holding Registers (3: read, 6: write)
  telegram.u16RegAdd = 0;  // Register başlangıç adresi
  telegram.u16CoilsNo = 6;  // 6 register oku (yıl, ay, gün, saat, dakika, saniye)
  telegram.au16reg = au16data;  // Gelen veriyi kaydetmek için

  // Modbus sorgusunu gönder
  master.query(telegram);

  // Gelen verileri işleme
  master.poll();
  if (master.getState() == COM_IDLE) {
    // Gelen verileri yazdırma
    Serial.print("Master Alınan Veri: ");
    Serial.print("Yıl: ");
    Serial.print(au16data[0]);
    Serial.print(", Ay: ");
    Serial.print(au16data[1]);
    Serial.print(", Gün: ");
    Serial.print(au16data[2]);
    Serial.print(", Saat: ");
    Serial.print(au16data[3]);
    Serial.print(", Dakika: ");
    Serial.print(au16data[4]);
    Serial.print(", Saniye: ");
    Serial.println(au16data[5]);
  }

  delay(1000);  // 1 saniye bekle
}
