// Bu kodda int türünde slave'den gelen 3 farklı veri slavede boolean
// türü veriye çevrilip tek registerla mastera gönderilmiştir.


#include <ModbusRtu.h>

#define TXEN 2  // RS485 TX Enable pini

uint16_t au16data[1];  // Tek bir register kullanacağız
Modbus slave(1, Serial, TXEN);  // Slave ID = 1

void setup() {
  pinMode(TXEN, OUTPUT);
  Serial.begin(19200);
  slave.start();
}

void loop() {
  int values[3] = {1, 0, 1};  // Gönderilecek 3 değer

  // Bit manipülasyonu ile 3 boolean değeri tek register'a sıkıştır
  uint16_t registerData = 0;
  for (int i = 0; i < 3; i++) {
    if (values[i]) {
      registerData |= (1 << i);  // i. biti 1 yap
    }
  }

  au16data[0] = registerData;  // Modbus register'a kaydet

  digitalWrite(TXEN, HIGH);
  slave.poll(au16data, 1);  // Tek register gönderiliyor
  digitalWrite(TXEN, LOW);

  Serial.print("Slave Gönderilen Veri (Register): ");
  Serial.println(registerData, BIN);  // Binary formatta yazdır
  delay(500);
}
