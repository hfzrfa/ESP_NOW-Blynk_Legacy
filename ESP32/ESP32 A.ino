#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BlynkSimpleEsp32.h>

// OLED I2C setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Blynk configuration
char auth[] = ""; // Replace with your Blynk auth token
char blynkServer[] = "0.0.0.0"; // Local Blynk server IP
int blynkPort = 8080;
char ssid[] = ""; // Replace with your WiFi SSID
char pass[] = ""; // Replace with your WiFi password

// Struct for incoming ESP-NOW data
typedef struct struct_message {
  int id;
  float temperature;
  float humidity;
  int distance;
} struct_message;

struct_message latestData;
bool dataReceived = false;
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 10000; // 10 seconds

// Function to print MAC
void printMacAddress(const uint8_t *mac) {
  for (int i = 0; i < 6; i++) {
    Serial.print(mac[i], HEX);
    if (i < 5) Serial.print(":");
  }
  Serial.println();
}

// ESP-NOW callback
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  memcpy(&latestData, data, sizeof(latestData));
  dataReceived = true;

  Serial.println("[ESP-NOW] Data received:");
  Serial.print("MAC: ");
  printMacAddress(info->src_addr);
  Serial.print("ID: "); Serial.println(latestData.id);
  Serial.print("Temp: "); Serial.print(latestData.temperature); Serial.println(" C");
  Serial.print("Humidity: "); Serial.print(latestData.humidity); Serial.println(" %");
  Serial.print("Distance: "); Serial.print(latestData.distance); Serial.println(" cm");

  // Display data
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("ESP32 Receiver");
  display.print("ID: "); display.println(latestData.id);
  display.print("Temp: "); display.print(latestData.temperature); display.println(" C");
  display.print("Humid: "); display.print(latestData.humidity); display.println(" %");
  display.print("Dist: "); display.print(latestData.distance); display.println(" cm");
  display.display();
}

void initEspNow() {
  if (esp_now_init() == ESP_OK) {
    esp_now_register_recv_cb(OnDataRecv);
    Serial.println("[ESP-NOW] Initialized");
  } else {
    Serial.println("[ESP-NOW] Init failed");
  }
}

void deinitEspNow() {
  esp_now_deinit();
  Serial.println("[ESP-NOW] Deinitialized");
}

void connectToWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.print("[WIFI] Connecting...");
  unsigned long startAttempt = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 10000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("[WIFI] Connected");
  } else {
    Serial.println("[WIFI] Failed to connect");
  }
}

void disconnectWiFi() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  Serial.println("[WIFI] Disconnected");
}

void sendToBlynk() {
  if (!dataReceived) {
    Serial.println("[BLYNK] No data to send");
    return;
  }

  Serial.println("[BLYNK] Sending data...");

  connectToWiFi();
  if (WiFi.status() == WL_CONNECTED) {
    Blynk.config(auth, blynkServer, blynkPort);
    if (Blynk.connect(5000)) {
      Blynk.virtualWrite(V1, latestData.temperature);
      Blynk.virtualWrite(V2, latestData.humidity);
      Blynk.virtualWrite(V3, latestData.distance);
      Serial.println("[BLYNK] Data sent successfully");
    } else {
      Serial.println("[BLYNK] Failed to connect");
    }
  }

  disconnectWiFi();
}

void setup() {
  Serial.begin(115200);
  Serial.println("[SETUP] Starting...");

  // OLED init
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("[OLED] Init failed");
    while (true);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Waiting for data...");
  display.display();

  WiFi.mode(WIFI_AP_STA); // Allows ESP-NOW while supporting future WiFi
  initEspNow();
}

void loop() {
  unsigned long currentMillis = millis();

  // Every 10s, stop ESP-NOW, send to Blynk, then resume ESP-NOW
  if (currentMillis - lastSendTime >= sendInterval) {
    lastSendTime = currentMillis;

    deinitEspNow();
    delay(50); // Ensure ESP-NOW fully stopped

    sendToBlynk();

    WiFi.mode(WIFI_AP_STA); // Reset mode
    initEspNow();
  }

  delay(2); // Let CPU breathe
}
