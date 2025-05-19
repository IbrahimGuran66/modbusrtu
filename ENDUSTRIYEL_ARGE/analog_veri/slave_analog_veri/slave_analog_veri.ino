#include <ModbusRtu.h>

#define TXEN 2  // RS485 TX Enable pini

uint16_t au16data[1]; // Sadece 1 register göndereceğiz

Modbus slave(1, Serial, TXEN); // Slave ID = 1, TX Enable için 2 numaralı pin

void setup() {
  pinMode(TXEN, OUTPUT);
  Serial.begin(19200);
  slave.start();
}

void loop() {
  // Analog pin okuma (örneğin A0 pininden)
  au16data[0] = analogRead(A0); // A0 pinindeki değeri oku ve register'a aktar

  digitalWrite(TXEN, HIGH); // Veri gönderirken aç
  slave.poll(au16data, 1); // Veriyi gönder
  digitalWrite(TXEN, LOW);  // Gönderme bitince kapat

  Serial.print("Slave Gönderilen Veri: ");
  Serial.println(au16data[0]);
  delay(500); // 500 ms bekle
}
