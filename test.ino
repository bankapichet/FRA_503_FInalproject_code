// #include <Wire.h>

// กำหนดพินสำหรับ RS485
#define RE 5  // พินสำหรับ Read Enable
#define DE 4  // พินสำหรับ Data Enable

// คำสั่ง Modbus สำหรับ N, P, K
const byte nitro[] = {0x01, 0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const byte phos[] = {0x01, 0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte pota[] = {0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};

byte values[11];

void setup() {
  Serial.begin(9600);         // สำหรับ Debugging
  Serial2.begin(4800, SERIAL_8N1, 16, 17); // Serial2: RX = GPIO16, TX = GPIO17
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);

  Serial.println("NPK Sensor Initializing...");
  delay(3000);
}

void loop() {
  byte val1, val2, val3;
  val1 = nitrogen();
  delay(250);
  val2 = phosphorous();
  delay(250);
  val3 = potassium();
  delay(250);

  // แสดงค่าที่อ่านได้
  Serial.print("Nitrogen: ");
  Serial.print(val1);
  Serial.println(" mg/kg");

  Serial.print("Phosphorous: ");
  Serial.print(val2);
  Serial.println(" mg/kg");

  Serial.print("Potassium: ");
  Serial.print(val3);
  Serial.println(" mg/kg");

  delay(2000);
}

// ฟังก์ชันอ่านค่าไนโตรเจน (N)
byte nitrogen() {
  digitalWrite(DE, HIGH); // เปิดการส่งข้อมูล
  digitalWrite(RE, HIGH);
  delay(10);

  if (Serial2.write(nitro, sizeof(nitro)) == 8) { // ส่งคำสั่ง
    digitalWrite(DE, LOW); // สลับไปอ่านข้อมูล
    digitalWrite(RE, LOW);
    delay(10);

    for (byte i = 0; i < 7; i++) {
      if (Serial2.available()) {
        values[i] = Serial2.read();
        Serial.print(values[i], HEX); // แสดงข้อมูลเป็น HEX
      }
    }
    Serial.println();
  }
  return values[4]; // ค่าผลลัพธ์ที่ต้องการ
}

// ฟังก์ชันอ่านค่าฟอสฟอรัส (P)
byte phosphorous() {
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);

  if (Serial2.write(phos, sizeof(phos)) == 8) {
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    delay(10);

    for (byte i = 0; i < 7; i++) {
      if (Serial2.available()) {
        values[i] = Serial2.read();
        Serial.print(values[i], HEX);
      }
    }
    Serial.println();
  }
  return values[4];
}

// ฟังก์ชันอ่านค่าโพแทสเซียม (K)
byte potassium() {
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);

  if (Serial2.write(pota, sizeof(pota)) == 8) {
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    delay(10);

    for (byte i = 0; i < 7; i++) {
      if (Serial2.available()) {
        values[i] = Serial2.read();
        Serial.print(values[i], HEX);
      }
    }
    Serial.println();
  }
  return values[4];
}
