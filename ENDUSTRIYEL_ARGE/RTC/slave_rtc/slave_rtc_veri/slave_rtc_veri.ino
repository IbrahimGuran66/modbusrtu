#include <ModbusRtu.h>
#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;  // RTC modülü nesnesi

#define TXEN 2  // RS485 TX Enable pini

uint16_t au16data[8];  // 4 float için 8 register
Modbus slave(1, Serial, TXEN);  // Slave ID = 1

void setup() {
  pinMode(TXEN, OUTPUT);
  Serial.begin(19200); // Baud Rate
  slave.start();

  // RTC modülünü başlat
  if (!rtc.begin()) {
    Serial.println("RTC bulunamadi!");
    while (1);
  }
}

void loop() {
  digitalWrite(TXEN, HIGH);  // Veri gönderirken aç

  DateTime now = rtc.now();  // RTC'den saati al

  // RTC saatini register'lara dönüştür
  au16data[0] = now.year();  // Yıl
  au16data[1] = now.month(); // Ay
  au16data[2] = now.day();   // Gün
  au16data[3] = now.hour();  // Saat
  au16data[4] = now.minute(); // Dakika
  au16data[5] = now.second(); // Saniye

  slave.poll(au16data, 6);  // RTC verilerini gönder

  digitalWrite(TXEN, LOW);   // Gönderme bitince kapat
  delay(1000);  // Her saniye güncelleme yap
}
