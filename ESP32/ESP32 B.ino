#include <esp_now.h>
#include <WiFi.h>
#include <DHT.h>

#define DHTPIN 14        // Pin data DHT22
#define DHTTYPE DHT22
#define TRIG_PIN 13      // Trigger pin for HC-SR04
#define ECHO_PIN 32      // Echo pin for HC-SR04

DHT dht(DHTPIN, DHTTYPE);

long duration;
int distance;

// Ganti MAC address ini dengan MAC dari ESP penerima
uint8_t allowedAddress[] = {0xA0, 0xB7, 0x65, 0x37, 0x0D, 0x70}; 

// Struct yang dikirim ke ESP penerima
typedef struct struct_message {
  int id;
  float temperature;
  float humidity;
  int distance;
} struct_message;

struct_message myData;

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, allowedAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);

  esp_now_register_send_cb([](const uint8_t *mac, esp_now_send_status_t status) {
    Serial.print("Send Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
  });
}

void loop() {
  // Read temperature and humidity from DHT22
  float temp = dht.readTemperature();
  float humid = dht.readHumidity();

  if (!isnan(temp) && !isnan(humid)) {
    myData.id = 1;
    myData.temperature = temp;
    myData.humidity = humid;

    Serial.print("Temperature = ");
    Serial.print(temp);
    Serial.print(" °C | Humidity = ");
    Serial.print(humid);
    Serial.println(" %");
  } else {
    Serial.println("Failed to read from DHT sensor!");
  }

  // Read distance from HC-SR04
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;
  myData.distance = distance;

  Serial.print("Distance = ");
  Serial.print(distance);
  Serial.println(" cm");

  // Send data via ESP-NOW
  esp_now_send(allowedAddress, (uint8_t *) &myData, sizeof(myData));

  delay(2000); // Send every 2 seconds
}