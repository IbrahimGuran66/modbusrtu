#include <ModbusRtu.h>

#define TXEN 2  // RS485 TX Enable pini

// Modbus veri dizisi
uint16_t au16data[16] = {3, 1415, 9265, 4, 2, 7182, 28182, 8, 0, 0, 0, 0, 0, 0, 1, 65535};

// Modbus Slave nesnesi
Modbus slave(1, Serial, TXEN); // Slave ID = 1, 2. pin TX Enable için

void setup() {
  pinMode(TXEN, OUTPUT);
  Serial.begin(19200); // Baud Rate
  slave.start();
}

void loop() {
  digitalWrite(TXEN, HIGH);  // Veri gönderirken aç
  slave.poll(au16data, 16);
  digitalWrite(TXEN, LOW);   // Gönderme bitince kapat

  // Slave’in hangi veriyi gönderdiğini görmek için
  Serial.print("Slave Gönderilen Veri: ");
  for (int i = 0; i < 4; i++) {
    Serial.print(au16data[i]);
    Serial.print(" ");
  }
  Serial.println();
  delay(500);
}
