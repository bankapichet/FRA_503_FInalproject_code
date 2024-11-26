#include <Wire.h>


#include <DHT.h>

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// เปลี่ยนค่าตัวแปรถัดไปด้วย SSID/รหัสผ่านของคุณ
const char* ssid = "Inwkritza007_2.4G";
const char* password = "0934603281";

// เพิ่มที่อยู่ IP ของ MQTT Broker ของคุณ
const char* mqtt_server = "broker.hivemq.com";




WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;


// ตัวแปรสำหรับส่งค่าไปยัง mqtt
float NPK_array[4] ;
float System_array[3] ;



// กำหนดตัวแปรสถานะ ปั๊มน้ำ ปั๊มปุ๋ย พัดลม
String pumpStatus;
String FanStatus;
String PumpfertilizerStatus;




// ข้อมูลเริ่มต้นการคำนวณปุ๋ย
int N_for_plane = 135 ;//ค่าไนโตรเจนที่กรีนโอ๊คต้องการ
int P_for_plane = 45  ;//ค่าฟอสฟอรัสที่กรีนโอ๊คต้องการ
int K_for_plane = 175 ;//ค่าโพเเทสเซียมที่กรีนโอ๊คต้องการ
float N_current =0 ;         // ค่า N ที่วัดได้ปัจจุบัน
float P_current =0;         // ค่า N ที่วัดได้ปัจจุบัน
float K_current =0 ;         // ค่า N ที่วัดได้ปัจจุบัน

float N_percent = 20.0; // ไนโตรเจนในปุ๋ย (%) เช่น 20% สำหรับสูตร 20-10-10
float P_percent = 10.0; // ฟอสฟอรัสในปุ๋ย (%)
float K_percent = 5.0; // โพแทสเซียมในปุ๋ย (%)
float dilution_rate = 1.0; // อัตราส่วนการเจือจาง (1 cc/ลิตร) จากขวดปุ๋ย
float plantRequirements[3] ; // ค่าที่ต้องใส่ให้พืช (ppm) สำหรับ N, P, K
float solutionConcentrations[3]; // เก็บค่าความเข้มข้นใน ppm
float Fertilizer_Use = 0;



// นับเวลาสำหรับการใส่ปุ๋ย
unsigned long startTime;  // เวลาเริ่มต้น
// const unsigned long interval = 7 * 24 * 60 * 60 * 1000; // 7 วันในมิลลิวินาที
const unsigned long interval = 1000; // 7 วันในมิลลิวินาที



String pumpManualMessage = "OFF";      // สถานะเริ่มต้นสำหรับ Spinger
String pumpManualNPKMessage = "OFF";  // สถานะเริ่มต้นสำหรับ Pump NPK


// JSON Document และ Buffer ใช้ร่วมกัน
StaticJsonDocument<200> doc;
char jsonBuffer[200];


unsigned long pumpStartTime = 0; // เวลาเริ่มต้นของการเปิดปั๊ม
bool pumpRunning = false;        // สถานะปั๊ม (true = กำลังทำงาน)


#define DHTPIN 4       // พินที่เชื่อมต่อกับ DHT22
#define DHTTYPE DHT22   // ประเภทเซนเซอร์คือ DHT22

#define FAN_PIN 15      // พินสำหรับพัดลม
#define NPK_PIN 17     // พินสำหรับระบบรดปุ๋ย
DHT dht(DHTPIN, DHTTYPE);



// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// การตั้งค่าพินควบคุมการรดน้ำ
#define WATER_PUMP_PIN 14
// สำหรับเซ็นเซอร์วัดความชื้นในดิน
int soil_pins[] = {34, 35, 32, 33};  // พิน GPIO สำหรับเซ็นเซอร์ 4 ตัว
int numSensors = 4;  // จำนวนเซ็นเซอร์วัดความชื้นในดิน

// ตัวแปรสำหรับเก็บค่าเซ็นเซอร์
float Sensor1 = 0.0, Sensor2 = 0.0, Sensor3 = 0.0, Sensor4 = 0.0;
float SoilMoisture = 0.0; // ตัวแปรสำหรับเก็บค่าเฉลี่ยความชื้นในดิน (%)


float mapToPercentage(float value, float inMin, float inMax, float outMin, float outMax) {
  if (value < inMax) {
    return outMax; // ค่าเกิน inMax ให้คืนค่าเป็น outMax
  } else if (value > inMin) {
    return outMin; // ค่าน้อยกว่า inMin ให้คืนค่าเป็น outMin
  } else {
    return (value - inMin) / (inMax - inMin) * (outMax - outMin) + outMin;
  }
}



// อ่านค่าความชื้นในดินและคำนวณค่าเฉลี่ย
void readAndCalculateSoilMoisture() {
  float totalMoisture = 0.0;
  for (int i = 0; i < numSensors; i++) {
    int sensorValue = analogRead(soil_pins[i]);
    float voltage = (sensorValue / 4095.0) * 3.3;

    if (i == 0) Sensor1 = voltage;
    else if (i == 1) Sensor2 = voltage;
    else if (i == 2) Sensor3 = voltage;
    else if (i == 3) Sensor4 = voltage;

    // Serial.print("Sensor ");
    // Serial.print(i + 1);
    // Serial.print(": ");
    // Serial.print(voltage, 2);
    // Serial.println(" V");
  }

  // ปรับค่า inMin และ inMax ตามแรงดันจริง
  float percentage1 = mapToPercentage(Sensor1, 1.95, 0.84 , 0, 100); 
  float percentage2 = mapToPercentage(Sensor2, 1.91, 0.79, 0, 100);
  float percentage3 = mapToPercentage(Sensor3, 1.92, 0.82, 0, 100);
  float percentage4 = mapToPercentage(Sensor4, 1.91, 0.82, 0, 100);

  SoilMoisture = (percentage1 + percentage2 + percentage3 + percentage4) / numSensors;
  System_array[2] = SoilMoisture;
  Serial.print("ค่าเฉลี่ยความชื้นในดิน: ");
  Serial.print(SoilMoisture, 2);
  Serial.println("%");
}


// ฟังก์ชันสำหรับรดน้ำ
void waterPlants(float soilMoisture){
  if (soilMoisture < 50) {
    Serial.println("รดน้ำ 30 วินาที...");
    digitalWrite(WATER_PUMP_PIN, HIGH);
    pumpStatus = "ON"; 
    sendMQTTData();
    delay(30000); 

    digitalWrite(WATER_PUMP_PIN, LOW);
    Serial.println("รอน้ำซึม 2 นาที...");
    pumpStatus = "OFF"; 
    sendMQTTData();
    delay(120000); 
  } 
  else if (soilMoisture >= 60 && soilMoisture <= 65) {
    Serial.println("รดน้ำ 15 วินาที...");
    digitalWrite(WATER_PUMP_PIN, HIGH);
    pumpStatus = "ON";
    sendMQTTData();
    delay(15000); 

    digitalWrite(WATER_PUMP_PIN, LOW);
    Serial.println("รอน้ำซึม 2 นาที ...");
    pumpStatus = "OFF"; 
    sendMQTTData();
    delay(120000); 
  } 
  else if (soilMoisture > 65) {
    Serial.println("ความชื้นมากกว่า 65% หยุดรดน้ำ...");
    digitalWrite(WATER_PUMP_PIN, LOW);
  }
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// สำหรับเซ็นเซอร์วัดระดับน้ำ
int analogPin = 36;
int val = 0;

#define RE 25
#define DE 39


void setup() { 
  Serial.begin(9600);  // ตั้งค่าพอร์ตอนุกรมหลักที่ 9600
  startTime = millis(); // บันทึกเวลาเริ่มต้น
  // ตั้งค่า pin ต่างๆ
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
  
  pinMode(FAN_PIN, OUTPUT);
  pinMode(NPK_PIN, OUTPUT);
  digitalWrite(NPK_PIN, LOW); // ตั้งค่าเริ่มต้นให้ปั๊มปิด
  digitalWrite(FAN_PIN, LOW); // ตั้งค่าเริ่มต้นให้ปั๊มปิด
  

  // รดน้ำต้นไม้
  pinMode(WATER_PUMP_PIN, OUTPUT);
  digitalWrite(WATER_PUMP_PIN, LOW);
  Serial.println("Starting System...");
  
 
  
  // เชื่อมต่อกับ Wi-Fi
  setup_wifi();
  
  // กำหนดเซิร์ฟเวอร์ MQTT
  client.setServer(mqtt_server, 1883);

  // รับค่า node-red
  client.setCallback(callback);
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP32_Client")) {
      Serial.println("Connected to MQTT");
      client.subscribe("Pump_Manual");
      client.subscribe("Pump_Manual_NPK");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds...");
      delay(5000);
    }
  }
  

  // ตั้งค่าขาของเซ็นเซอร์และปุ่มเป็นอินพุต
  dht.begin();  // เริ่มต้นเซ็นเซอร์ DHT22
  // Serial2.begin(4800, SERIAL_8N1, RS485_RX, RS485_TX);  // Serial2 สำหรับ RS485 ที่ 4800
  
 
  
  delay(500);
  Serial.println("Initializing NPK Sensor...");
}













// ตั้งค่าการเชื่อมต่อ wifi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  // รอการเชื่อมต่อ Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to the WiFi network");
}

// เช็คการเชื่อมต่อกับ mqtt
void reconnect_mqtt() {
  while (!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
    
    if (client.connect(client_id.c_str())) {
      Serial.println("MQTT connected");
    } else {
      Serial.print("Failed to connect, state ");
      Serial.println(client.state());
      delay(2000); 
    }
  }
}


// ฟังก์ชัน callback สำหรับจัดการข้อความ MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.println(topic);

  // แปลง payload เป็น String
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.print("Message: ");
  Serial.println(message);

  // ตรวจสอบว่า Topic ไหนส่งข้อความมา
  if (String(topic) == "Pump_Manual") {
    pumpManualMessage = message; // เก็บค่าข้อความในตัวแปร
    Serial.print("Updated Pump_Manual Message: ");
    Serial.println(pumpManualMessage);
  } else if (String(topic) == "Pump_Manual_NPK") {
    pumpManualNPKMessage = message; // เก็บค่าข้อความในตัวแปร
    Serial.print("Updated Pump_Manual_NPK Message: ");
    Serial.println(pumpManualNPKMessage);
  }
}




 // ฟังก์ชันสำหรับควบคุมพัดลมดูดอากาศ
void controlFan(float airHumidity) {
    if (airHumidity > 65) {
      digitalWrite(FAN_PIN, HIGH);
      Serial.println("Start dehumidifying");
      FanStatus = "ON";
    } else {
      digitalWrite(FAN_PIN, LOW);
      Serial.println("Stop dehumidifying");
      FanStatus = "OFF";
    }
}





void sendMQTTData() {
    StaticJsonDocument<200> doc; // สำหรับ JSON
    char jsonBuffer[200]; // บัฟเฟอร์สำหรับ JSON ข้อความ
    char tempMsg[10];     // บัฟเฟอร์สำหรับข้อความทั่วไป

  

    // *** ส่ง System_array ***
    doc.clear();
    doc["AirTemp"] = System_array[0];
    doc["AirHumidity"] = System_array[1];
    doc["SoilHumidity"] = System_array[2];
    serializeJson(doc, jsonBuffer);
    Serial.print("Publishing System Array: ");
    Serial.println(jsonBuffer);
    if (client.publish("System_Data", jsonBuffer)) {
      Serial.println("Publish System Array successful");
    } else {
      Serial.println("Publish System Array failed, reconnecting...");
      reconnect_mqtt();
    }

    delay(1000); // หน่วงเวลา 1 วินาที












void loop() { 
   // ตรวจสอบให้แน่ใจว่า MQTT ยังเชื่อมต่ออยู่
  if (!client.connected()) {
    reconnect_mqtt();
  }
  client.loop();  // รักษาการเชื่อมต่อกับ MQTT
  long now = millis();
  if (now - lastMsg > 1000) {  // ส่งข้อมูลทุก 1 วินาที
    lastMsg = now;
  
  
  
  }
  
  
  
  
  
  



  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  //ควบคุมปั๊มน้ำ
  readAndCalculateSoilMoisture();
  // ตรวจสอบว่าความชื้นไม่เกิน 65% ก่อนเรียก waterPlants
  if (SoilMoisture <= 65) {
    waterPlants(SoilMoisture);
   
  } else {
    Serial.println("ความชื้นในดินเกิน 65% หยุดการรดน้ำ...");
    digitalWrite(WATER_PUMP_PIN, LOW); // ปิดปั๊มน้ำหากเปิดอยู่
    pumpStatus = "OFF" ;
    sendMQTTData();
    
  }
  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  Serial.print(SoilMoisture, 2);




  // อ่านค่าเซ็นเซอร์วัดอากาศ (อุณหภูมิและความชื้น)

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  System_array[0]=temperature;
  System_array[1]=humidity;
  Serial.print("อุณหภูมิอากาศ : ");
  Serial.print(temperature, 1);
  Serial.println(" °C");
  
  Serial.print("ความชื้นอากาศ : ");
  Serial.print(humidity, 1);
  Serial.println(" %");

  

  delay(2000);  // หน่วงเวลาเล็กน้อยระหว่างการอ่าน
}


