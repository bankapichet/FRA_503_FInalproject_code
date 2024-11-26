


// // // Code จ่ายน้ำให้พืช ver 1

// #include <Wire.h>
// #include <DHT.h>

// // กำหนดค่าคงที่และพิน
// #define DHTPIN 26       // พินที่เชื่อมต่อกับ DHT22
// #define DHTTYPE DHT22   // ประเภทเซนเซอร์คือ DHT22
// DHT dht(DHTPIN, DHTTYPE);

// // พินควบคุมปั๊มน้ำ
// #define WATER_PUMP_PIN 14
// // สำหรับเซ็นเซอร์วัดความชื้นในดิน
// int soil_pins[] = {34, 35, 32, 33};  // พิน GPIO สำหรับเซ็นเซอร์ 4 ตัว
// int numSensors = 4;  // จำนวนเซ็นเซอร์วัดความชื้นในดิน

// // ตัวแปรสำหรับเก็บค่าเซ็นเซอร์
// float Sensor1 = 0.0, Sensor2 = 0.0, Sensor3 = 0.0, Sensor4 = 0.0;
// float SoilMoisture = 0.0; // ตัวแปรสำหรับเก็บค่าเฉลี่ยความชื้นในดิน (%)


// float mapToPercentage(float value, float inMin, float inMax, float outMin, float outMax) {
//   if (value < inMax) {
//     return outMax; // ค่าเกิน inMax ให้คืนค่าเป็น outMax
//   } else if (value > inMin) {
//     return outMin; // ค่าน้อยกว่า inMin ให้คืนค่าเป็น outMin
//   } else {
//     return (value - inMin) / (inMax - inMin) * (outMax - outMin) + outMin;
//   }
// }

// // อ่านค่าความชื้นในดินและคำนวณค่าเฉลี่ย
// void readAndCalculateSoilMoisture() {
//   float totalMoisture = 0.0;
//   for (int i = 0; i < numSensors; i++) {
//     int sensorValue = analogRead(soil_pins[i]);
//     float voltage = (sensorValue / 4095.0) * 3.3;

//     if (i == 0) Sensor1 = voltage;
//     else if (i == 1) Sensor2 = voltage;
//     else if (i == 2) Sensor3 = voltage;
//     else if (i == 3) Sensor4 = voltage;

//     Serial.print("Sensor ");
//     Serial.print(i + 1);
//     Serial.print(": ");
//     Serial.print(voltage, 2);
//     Serial.println(" V");
//   }

//   // ปรับค่า inMin และ inMax ตามแรงดันจริง
//   float percentage1 = mapToPercentage(Sensor1, 1.95, 0.84 , 0, 100); 
//   float percentage2 = mapToPercentage(Sensor2, 1.91, 0.79, 0, 100);
//   float percentage3 = mapToPercentage(Sensor3, 1.92, 0.82, 0, 100);
//   float percentage4 = mapToPercentage(Sensor4, 1.91, 0.82, 0, 100);

//   SoilMoisture = (percentage1 + percentage2 + percentage3 + percentage4) / numSensors;

//   Serial.print("ค่าเฉลี่ยความชื้นในดิน: ");
//   Serial.print(SoilMoisture, 2);
//   Serial.println("%");
// }


// // ฟังก์ชันสำหรับรดน้ำ
// void waterPlants(float soilMoisture){
//   if (soilMoisture < 50) {
//     Serial.println("รดน้ำ 30 วินาที...");
//     digitalWrite(WATER_PUMP_PIN, HIGH);
//     delay(1500); 
//     digitalWrite(WATER_PUMP_PIN, LOW);
//     Serial.println("รอน้ำซึม 2 นาที...");
//     delay(20000); 
//   } 
//   else if (soilMoisture >= 60 && soilMoisture <= 65) {
//     Serial.println("รดน้ำ 15 วินาที...");
//     digitalWrite(WATER_PUMP_PIN, HIGH);
//     delay(1000); 
//     digitalWrite(WATER_PUMP_PIN, LOW);
//     Serial.println("รอน้ำซึม 2 นาที ...");
//     delay(20000); 
//   } 
//   else if (soilMoisture > 65) {
//     Serial.println("ความชื้นมากกว่า 65% หยุดรดน้ำ...");
//     digitalWrite(WATER_PUMP_PIN, LOW);
//   }
// }

// void setup() {
//   Serial.begin(9600);
//   dht.begin();
//   pinMode(WATER_PUMP_PIN, OUTPUT);
//   digitalWrite(WATER_PUMP_PIN, LOW);
//   Serial.println("Starting System...");
// }

// void loop() {
//   readAndCalculateSoilMoisture();
//   // ตรวจสอบว่าความชื้นไม่เกิน 65% ก่อนเรียก waterPlants
//   if (SoilMoisture <= 65) {
//     waterPlants(SoilMoisture);
//   } else {
//     Serial.println("ความชื้นในดินเกิน 65% หยุดการรดน้ำ...");
//     digitalWrite(WATER_PUMP_PIN, LOW); // ปิดปั๊มน้ำหากเปิดอยู่
//   }

//   delay(2000); // หน่วงเวลาเล็กน้อย
// }




// // // Code จ่ายน้ำให้พืช ver 2
// #include <Wire.h>
// #include <DHT.h>

// // กำหนดค่าคงที่และพิน
// #define DHTPIN 26       // พินที่เชื่อมต่อกับ DHT22
// #define DHTTYPE DHT22   // ประเภทเซนเซอร์คือ DHT22
// DHT dht(DHTPIN, DHTTYPE);

// // พินควบคุมปั๊มน้ำ
// #define WATER_PUMP_PIN 25

// // สำหรับเซ็นเซอร์วัดความชื้นในดิน
// int soil_pins[] = {34, 35, 32, 33};  // พิน GPIO สำหรับเซ็นเซอร์ 4 ตัว
// int numSensors = 4;  // จำนวนเซ็นเซอร์วัดความชื้นในดิน

// // ตัวแปรสำหรับเก็บค่าเซ็นเซอร์
// float Sensor1 = 0.0, Sensor2 = 0.0, Sensor3 = 0.0, Sensor4 = 0.0;
// float SoilMoisture = 0.0; // ตัวแปรสำหรับเก็บค่าเฉลี่ยความชื้นในดิน (%)

// // ฟังก์ชันแมปค่า
// float mapToPercentage(float value, float inMin, float inMax, float outMin, float outMax) {
//   if (value < inMax) {
//     return outMax; // ค่าเกิน inMax ให้คืนค่าเป็น outMax
//   } else if (value > inMin) {
//     return outMin; // ค่าน้อยกว่า inMin ให้คืนค่าเป็น outMin
//   } else {
//     return (value - inMin) / (inMax - inMin) * (outMax - outMin) + outMin;
//   }
// }

// // อ่านค่าความชื้นในดินและคำนวณค่าเฉลี่ย
// void readAndCalculateSoilMoisture() {
//   float totalMoisture = 0.0;
//   for (int i = 0; i < numSensors; i++) {
//     int sensorValue = analogRead(soil_pins[i]);
//     float voltage = (sensorValue / 4095.0) * 3.3;

//     if (i == 0) Sensor1 = voltage;
//     else if (i == 1) Sensor2 = voltage;
//     else if (i == 2) Sensor3 = voltage;
//     else if (i == 3) Sensor4 = voltage;

//     Serial.print("Sensor ");
//     Serial.print(i + 1);
//     Serial.print(": ");
//     Serial.print(voltage, 2);
//     Serial.println(" V");
//   }

//   float percentage1 = mapToPercentage(Sensor1, 1.95, 0.84, 0, 100); 
//   float percentage2 = mapToPercentage(Sensor2, 1.91, 0.79, 0, 100);
//   float percentage3 = mapToPercentage(Sensor3, 1.92, 0.82, 0, 100);
//   float percentage4 = mapToPercentage(Sensor4, 1.91, 0.82, 0, 100);

//   SoilMoisture = (percentage1 + percentage2 + percentage3 + percentage4) / numSensors;

//   Serial.print("ค่าเฉลี่ยความชื้นในดิน: ");
//   Serial.print(SoilMoisture, 2);
//   Serial.println("%");
// }

// // ฟังก์ชันสำหรับรดน้ำ
// void waterPlants(float soilMoisture) {
//   unsigned long startWateringTime = millis(); // เวลาเริ่มต้น
//   unsigned long wateringDuration = 0;

//   if (soilMoisture < 55) {
//     Serial.println("รดน้ำ 30 วินาที...");
//     wateringDuration = 30000; // 30 วินาที
//   } 
//   else if (soilMoisture >= 60 && soilMoisture <= 65) {
//     Serial.println("รดน้ำ 15 วินาที...");
//     wateringDuration = 15000; // 15 วินาที
//   } 
//   else if (soilMoisture > 65) {
//     Serial.println("ความชื้นมากกว่า 65% หยุดรดน้ำ...");
//     digitalWrite(WATER_PUMP_PIN, LOW);
//     return;
//   }

//   digitalWrite(WATER_PUMP_PIN, HIGH);
//   while (millis() - startWateringTime < wateringDuration) {
//     float currentSoilMoisture = SoilMoisture; // ใช้ค่าล่าสุด
//     Serial.print("ความชื้นปัจจุบัน: ");
//     Serial.print(currentSoilMoisture);
//     Serial.println("%");

//     if (currentSoilMoisture > 65) {
//       Serial.println("ความชื้นเกิน 65% หยุดรดน้ำทันที...");
//       break;
//     }
//     delay(500); // ตรวจสอบค่าความชื้นทุก 0.5 วินาที
//   }

//   digitalWrite(WATER_PUMP_PIN, LOW);
//   Serial.println("ปั๊มน้ำหยุดทำงาน");

//   Serial.println("รอน้ำซึม 2 นาที...");
//   delay(120000); // รอน้ำซึม 2 นาที
// }

// void setup() {
//   Serial.begin(9600);
//   dht.begin();
//   pinMode(WATER_PUMP_PIN, OUTPUT);
//   digitalWrite(WATER_PUMP_PIN, LOW);
//   Serial.println("Starting System...");
// }

// void loop() {
//   readAndCalculateSoilMoisture();
//   waterPlants(SoilMoisture);
//   delay(2000); // หน่วงเวลาเล็กน้อย
// }

















// // กดปิดปั้๊มผ่าน node-red
// #include <WiFi.h>         // สำหรับ Wi-Fi
// #include <PubSubClient.h> // สำหรับ MQTT

// // Wi-Fi and MQTT Configuration
// const char* ssid = "Inwkritza007_2.4G";         // ชื่อ Wi-Fi
// const char* password = "0934603281";            // รหัสผ่าน Wi-Fi
// const char* mqtt_server = "broker.hivemq.com";  // ที่อยู่ของ MQTT Broker

// WiFiClient espClient;
// PubSubClient client(espClient);

// // Pump Control Pins
// #define PUMP1_PIN 14 // ปั๊ม 1
// #define PUMP2_PIN 13 // ปั๊ม 2

// String pumpManualMessage = "";      // เก็บข้อความจาก Pump_Manual
// String pumpManualNPKMessage = "";   // เก็บข้อความจาก Pump_Manual_NPK

// // ฟังก์ชันเชื่อมต่อ Wi-Fi
// void setup_wifi() {
//   delay(10);
//   Serial.println();
//   Serial.print("Connecting to ");
//   Serial.println(ssid);

//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println();
//   Serial.println("WiFi connected");
//   Serial.print("IP address: ");
//   Serial.println(WiFi.localIP());
// }

// // ฟังก์ชันเชื่อมต่อ MQTT
// void reconnect_mqtt() {
//   while (!client.connected()) {
//     Serial.print("Attempting MQTT connection...");
//     String client_id = "ESP32Client-";
//     client_id += String(WiFi.macAddress());

//     if (client.connect(client_id.c_str())) {
//       Serial.println("connected");

//       // Subscribe to MQTT topics
//       client.subscribe("Pump_Manual");
//       client.subscribe("Pump_Manual_NPK");
//       Serial.println("Subscribed to Pump_Manual and Pump_Manual_NPK");
//     } else {
//       Serial.print("failed, rc=");
//       Serial.print(client.state());
//       Serial.println(" try again in 5 seconds");
//       delay(5000);
//     }
//   }
// }

// // ฟังก์ชัน callback สำหรับจัดการข้อความ MQTT
// void callback(char* topic, byte* payload, unsigned int length) {
//   Serial.print("Message arrived on topic: ");
//   Serial.println(topic);

//   // แปลง payload เป็น String
//   String message = "";
//   for (int i = 0; i < length; i++) {
//     message += (char)payload[i];
//   }
//   Serial.print("Message: ");
//   Serial.println(message);

//   // ตรวจสอบว่า Topic ไหนส่งข้อความมา
//   if (String(topic) == "Pump_Manual") {
//     pumpManualMessage = message; // เก็บค่าข้อความในตัวแปร
//     Serial.print("Updated Pump_Manual Message: ");
//     Serial.println(pumpManualMessage);

//     // ควบคุมปั๊ม 1
//     if (pumpManualMessage == "ON") {
//       digitalWrite(PUMP1_PIN, HIGH); // เปิดปั๊ม 1
//       Serial.println("Pump 1 turned ON");
//     } else if (pumpManualMessage == "OFF") {
//       digitalWrite(PUMP1_PIN, LOW); // ปิดปั๊ม 1
//       Serial.println("Pump 1 turned OFF");
//     }
//   } else if (String(topic) == "Pump_Manual_NPK") {
//     pumpManualNPKMessage = message; // เก็บค่าข้อความในตัวแปร
//     Serial.print("Updated Pump_Manual_NPK Message: ");
//     Serial.println(pumpManualNPKMessage);

//     // ควบคุมปั๊ม 2
//     if (pumpManualNPKMessage == "ON") {
//       digitalWrite(PUMP2_PIN, HIGH); // เปิดปั๊ม 2
//       Serial.println("Pump 2 turned ON");
//     } else if (pumpManualNPKMessage == "OFF") {
//       digitalWrite(PUMP2_PIN, LOW); // ปิดปั๊ม 2
//       Serial.println("Pump 2 turned OFF");
//     }
//   }
// }

// // ฟังก์ชัน setup
// void setup() {
//   Serial.begin(115200);
//   setup_wifi(); // เชื่อมต่อ Wi-Fi

//   client.setServer(mqtt_server, 1883); // ตั้งค่า MQTT Broker
//   client.setCallback(callback);       // ตั้งค่า callback

//   // ตั้งค่าขาเอาต์พุตสำหรับปั๊ม
//   pinMode(PUMP1_PIN, OUTPUT);
//   digitalWrite(PUMP1_PIN, LOW); // เริ่มต้นปิดปั๊ม 1
//   pinMode(PUMP2_PIN, OUTPUT);
//   digitalWrite(PUMP2_PIN, LOW); // เริ่มต้นปิดปั๊ม 2
// }

// // ฟังก์ชัน loop
// void loop() {
//   if (!client.connected()) {
//     reconnect_mqtt(); // เชื่อมต่อ MQTT หากหลุด
//   }
//   client.loop(); // ตรวจสอบข้อความ MQTT
// }








// //โค้ดคุมพัดลม
// #include <WiFi.h>
// #include <PubSubClient.h>
// #include <DHT.h>

// // Wi-Fi and MQTT Configuration
// const char* ssid = "Inwkritza007_2.4G";
// const char* password = "0934603281";
// const char* mqtt_server = "broker.hivemq.com";

// WiFiClient espClient;
// PubSubClient client(espClient);

// // DHT Sensor Configuration
// #define DHTPIN 4           // กำหนด PIN ที่เชื่อมต่อกับเซ็นเซอร์ DHT
// #define DHTTYPE DHT22       // ใช้เซ็นเซอร์ DHT22
// DHT dht(DHTPIN, DHTTYPE);

// // Fan Control Configuration
// #define FAN_PIN 0          // กำหนด PIN ที่เชื่อมต่อกับพัดลม
// String FanStatus = "OFF";   // สถานะเริ่มต้นของพัดลม

// // Array สำหรับจัดเก็บค่าระบบ
// float System_array[2]; // [0]: Temperature, [1]: Humidity

// // ฟังก์ชันควบคุมพัดลม
// void controlFan(float airHumidity) {
//   if (airHumidity > 65) {  // หากความชื้น > 65% ให้เปิดพัดลม
//     digitalWrite(FAN_PIN, HIGH);
//     Serial.println("Start dehumidifying");
//     FanStatus = "ON";
//   } else {  // หากความชื้น <= 65% ให้ปิดพัดลม
//     digitalWrite(FAN_PIN, LOW);
//     Serial.println("Stop dehumidifying");
//     FanStatus = "OFF";
//   }
// }

// // ฟังก์ชันส่งสถานะพัดลมไปยัง MQTT
// void publishFanStatus() {
//   if (client.publish("fan/status", FanStatus.c_str())) {
//     Serial.print("MQTT Publish Fan Status: ");
//     Serial.println(FanStatus);
//   } else {
//     Serial.println("MQTT Publish Fan Status Failed!");
//   }
// }

// // ฟังก์ชันเชื่อมต่อ Wi-Fi
// void setup_wifi() {
//   delay(10);
//   Serial.println();
//   Serial.print("Connecting to ");
//   Serial.println(ssid);

//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println();
//   Serial.println("WiFi connected");
//   Serial.print("IP address: ");
//   Serial.println(WiFi.localIP());
// }

// // ฟังก์ชันเชื่อมต่อ MQTT
// void reconnect_mqtt() {
//   while (!client.connected()) {
//     Serial.print("Attempting MQTT connection...");
//     String client_id = "ESP32Client-";
//     client_id += String(WiFi.macAddress());

//     if (client.connect(client_id.c_str())) {
//       Serial.println("connected");

//       // Subscribe to MQTT topics (หากมีการ subscribe อื่นให้เพิ่มที่นี่)
//     } else {
//       Serial.print("failed, rc=");
//       Serial.print(client.state());
//       Serial.println(" try again in 5 seconds");
//       delay(5000);
//     }
//   }
// }

// // ฟังก์ชัน setup
// void setup() {
//   Serial.begin(115200);
//   setup_wifi(); // เชื่อมต่อ Wi-Fi

//   client.setServer(mqtt_server, 1883); // ตั้งค่า MQTT Broker
//   pinMode(FAN_PIN, OUTPUT);
//   digitalWrite(FAN_PIN, LOW); // ตั้งค่าพัดลมเริ่มต้นเป็นปิด

//   dht.begin(); // เริ่มต้นเซ็นเซอร์ DHT
// }

// // ฟังก์ชัน loop
// void loop() {
//   if (!client.connected()) {
//     reconnect_mqtt(); // เชื่อมต่อ MQTT หากหลุด
//   }
//   client.loop(); // ตรวจสอบข้อความ MQTT

//   // อ่านค่าเซ็นเซอร์วัดอุณหภูมิและความชื้น
//   float humidity = dht.readHumidity();
//   float temperature = dht.readTemperature();

//   if (isnan(humidity) || isnan(temperature)) {
//     Serial.println("Failed to read from DHT sensor!");
//     return;
//   }

//   // เก็บค่าใน System_array
//   System_array[0] = temperature;
//   System_array[1] = humidity;

//   // แสดงผลบน Serial Monitor
//   Serial.print("อุณหภูมิ: ");
//   Serial.print(temperature, 1);
//   Serial.println(" °C");

//   Serial.print("ความชื้น: ");
//   Serial.print(humidity, 1);
//   Serial.println(" %");

//   // ควบคุมพัดลมตามค่าความชื้น
//   controlFan(humidity);

//   // ส่งสถานะพัดลมไปยัง MQTT
//   publishFanStatus();

//   // หน่วงเวลาสำหรับการอ่านค่าครั้งถัดไป
//   delay(2000);
// }
