#include "max6675.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"

#include "ESPAsyncWebServer.h"
#include "LittleFS.h"
#include <Preferences.h>
#include <ArduinoJson.h>

// ---------------------------- ICONS ----------------------------
// 'upload', 20x20px
const unsigned char uploadIcon[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0xf0, 0x00, 0x01, 0xf8, 0x00, 0x03, 0xfc, 0x00, 0x07,
  0xfe, 0x00, 0x0f, 0x6f, 0x00, 0x0e, 0x67, 0x00, 0x0c, 0x63, 0x00, 0x00, 0x60, 0x00, 0x00, 0x60,
  0x00, 0x00, 0x60, 0x00, 0x00, 0x60, 0x00, 0x00, 0x60, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00,
  0x3f, 0xff, 0xc0, 0x3f, 0xff, 0xc0, 0x3f, 0xff, 0xc0, 0x00, 0x00, 0x00
};
// 'check', 20x20px
const unsigned char checkIcon[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x00,
  0x03, 0xc0, 0x00, 0x07, 0xc0, 0x00, 0x0f, 0x80, 0x00, 0x1f, 0x00, 0x38, 0x3e, 0x00, 0x3c, 0x7c,
  0x00, 0x3e, 0xf8, 0x00, 0x1f, 0xf0, 0x00, 0x0f, 0xe0, 0x00, 0x07, 0xc0, 0x00, 0x03, 0x80, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'x', 20x20px
const unsigned char xIcon[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x07, 0x00, 0x1f,
  0x0f, 0x00, 0x0f, 0x9f, 0x00, 0x07, 0xfe, 0x00, 0x03, 0xfc, 0x00, 0x01, 0xf8, 0x00, 0x01, 0xf8,
  0x00, 0x03, 0xfc, 0x00, 0x07, 0xfe, 0x00, 0x0f, 0x9f, 0x00, 0x0f, 0x0f, 0x00, 0x0e, 0x07, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'wifi', 20x20px
const unsigned char wifiIcon[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x1f, 0xff, 0x80, 0x3f,
  0xff, 0xc0, 0x7c, 0x03, 0xe0, 0xf0, 0xf0, 0xf0, 0x67, 0xfe, 0x60, 0x1f, 0xff, 0x80, 0x1e, 0x07,
  0x80, 0x18, 0xf1, 0x80, 0x03, 0xfc, 0x00, 0x03, 0xfc, 0x00, 0x03, 0x0c, 0x00, 0x00, 0x60, 0x00,
  0x00, 0x60, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'wifi-zero', 20x20px
const unsigned char wifiZeroIcon[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00,
  0x00, 0x60, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'wifi-low', 20x20px
const unsigned char wifiLowIcon[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xf0, 0x00, 0x03, 0xfc, 0x00, 0x03, 0xfc, 0x00, 0x03, 0x0c, 0x00, 0x00, 0x60, 0x00,
  0x00, 0x60, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'wifi-high', 20x20px
const unsigned char wifiHighIcon[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x07, 0xfe, 0x00, 0x1f, 0xff, 0x80, 0x1e, 0x07,
  0x80, 0x18, 0xf1, 0x80, 0x03, 0xfc, 0x00, 0x03, 0xfc, 0x00, 0x03, 0x0c, 0x00, 0x00, 0x60, 0x00,
  0x00, 0x60, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'wifi-off', 20x20px
const unsigned char wifiOffIcon[] PROGMEM = {
  0x60, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x78, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x3f, 0xff, 0x80, 0x3f,
  0xff, 0xc0, 0x7f, 0x83, 0xe0, 0xf7, 0xf0, 0xf0, 0x67, 0xfe, 0x60, 0x1f, 0xff, 0x80, 0x1e, 0xff,
  0x80, 0x18, 0xfd, 0x80, 0x03, 0xfe, 0x00, 0x03, 0xff, 0x00, 0x03, 0x0f, 0x80, 0x00, 0x67, 0xc0,
  0x00, 0x63, 0xe0, 0x00, 0x61, 0xe0, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x60
};


// ---------------------------- PIN DEFINITIONS ----------------------------

// SSD1306 OLED SPI pins
#define OLED_SCK 27
#define OLED_SDA 26
#define OLED_RES 25
#define OLED_DC 33
#define OLED_CS 32

// MAX6675 SPI pins
#define MAX6675_SCK 19
#define MAX6675_CS 5
#define MAX6675_SO 18

// Jumper Pin for debug mode
#define TOUCH_PIN 12


// ------------------------------ PREFERENCES ------------------------------
#define PREFERENCES_NAMESPACE "brewthermo"


// ---------------------------- DISPLAY CONFIGURATION ---------------------------

// Refresh delay of the display in ms
#define DISPLAY_REFRESH_DELAY_DEFAULT 500
unsigned int DISPLAY_REFRESH_DELAY = DISPLAY_REFRESH_DELAY_DEFAULT;

// How often the Wifi connectivity and RSSI should be checked in ms
#define WIFI_CHECK_DELAY_DEFAULT 1000
unsigned int WIFI_CHECK_DELAY = WIFI_CHECK_DELAY_DEFAULT;


// ---------------------------- WIFI CREDENTIALS ----------------------------

// WiFi Credentials
#define WIFI_HOSTNAME "BeerbrewingThermometer_ESP32"
#define WIFI_SSID_DEFAULT ""
String WIFI_SSID = WIFI_SSID_DEFAULT;
#define WIFI_PASSWORD_DEFAULT ""
String WIFI_PASSWORD = WIFI_PASSWORD_DEFAULT;

// Debug Mode Access Point
#define AP_SSID "BeerbrewingThermo"
String AP_PASSWORD = "";


// ---------------------------- TIME CONFIGURATION ---------------------------

#define NTP_SERVER_DEFAULT "pool.ntp.org"
String NTP_SERVER = NTP_SERVER_DEFAULT;
#define TIMESYNC_DELAY_DEFAULT 108000
unsigned int TIMESYNC_DELAY = TIMESYNC_DELAY_DEFAULT;


// ---------------------------- FIREBASE CREDENTIALS ----------------------------

#define USER_EMAIL_DEFAULT ""
String USER_EMAIL = USER_EMAIL_DEFAULT;

#define USER_PASSWORD_DEFAULT ""
String USER_PASSWORD = USER_PASSWORD_DEFAULT;

#define API_KEY_DEFAULT ""
String API_KEY = API_KEY_DEFAULT;

#define DATABASE_URL_DEFAULT ""
String DATABASE_URL = DATABASE_URL_DEFAULT;

// Firestore Collection
#define DATABASE_PATH_DEFAULT "readings"
String DATABASE_PATH = DATABASE_PATH_DEFAULT;

// Upload delay to firebase
#define UPLOAD_DELAY_DEFAULT 5000
unsigned int UPLOAD_DELAY = UPLOAD_DELAY_DEFAULT;


// ---------------------------- GLOBAL VARIABLES ----------------------------

// Task Handles
TaskHandle_t readSensorTaskHandle;
TaskHandle_t uploadTaskHandle;
TaskHandle_t checkWifiTaskHandle;
TaskHandle_t timesyncTaskHandle;

float currentTemperature = 0.0;
unsigned int delayedTime = 0;
bool hasDataUploaded = false;
bool isUploading = false;
bool isUploadSuccessful = false;

bool isWifiConnected = false;
int wifiSignalStrength = 0;

// Firebase Objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;


// ---------------------------- OBJECT INITIALIZATIONS ----------------------------
// Initialize MAX6675 sensor
MAX6675 thermocouple(MAX6675_SCK, MAX6675_CS, MAX6675_SO);

// Initialize OLED display
Adafruit_SSD1306 display(128, 64, &SPI, OLED_DC, OLED_RES, OLED_CS);

AsyncWebServer server(80);

Preferences preferences;


// ---------------------------- FUNCTION DEFINITIONS ----------------------------

void enableOled() {
  digitalWrite(OLED_CS, LOW);
}

void disableOled() {
  digitalWrite(OLED_CS, HIGH);
}

void enableMax6675() {
  digitalWrite(MAX6675_CS, LOW);
}

void disableMax6675() {
  digitalWrite(MAX6675_CS, HIGH);
}

void readSensorTask(void *param) {
  for (;;) {
    // Disable OLED and enable MAX6675
    disableOled();
    enableMax6675();
    vTaskDelay(pdMS_TO_TICKS(10));

    // Read temperature from MAX6675
    currentTemperature = thermocouple.readCelsius();

    // Disable MAX6675
    disableMax6675();

    // Display WiFi status and upload status on OLED
    display.clearDisplay();

    // WiFi status and icon logic
    if (isWifiConnected) {
      const unsigned char *currentWifiIcon;
      if (wifiSignalStrength >= -55) {
        currentWifiIcon = wifiIcon;
      } else if (wifiSignalStrength >= -70) {
        currentWifiIcon = wifiHighIcon;
      } else if (wifiSignalStrength >= -85) {
        currentWifiIcon = wifiLowIcon;
      } else {
        currentWifiIcon = wifiZeroIcon;
      }

      display.drawBitmap(108, 0, currentWifiIcon, 20, 20, SSD1306_WHITE);
    } else {
      display.drawBitmap(108, 0, wifiOffIcon, 20, 20, SSD1306_WHITE);
    }

    if (hasDataUploaded) {
      display.drawBitmap(108, 22, isUploadSuccessful ? checkIcon : xIcon, 20, 20, SSD1306_WHITE);
    }

    if (isUploading) {
      display.drawBitmap(108, 44, uploadIcon, 20, 20, SSD1306_WHITE);
    } else {
      int progressBarWidthCurrent = map(delayedTime, 0, UPLOAD_DELAY, 0, 16);
      display.fillRect(110, 60, progressBarWidthCurrent, 3, SSD1306_WHITE);
    }

    display.setCursor(0, 0);
    int temp = (int)round(currentTemperature) % 100;
    if (temp < 10) display.print("0");
    display.print(temp);
    display.display();

    disableOled();
    vTaskDelay(pdMS_TO_TICKS(DISPLAY_REFRESH_DELAY));
  }
}


void initWiFi() {
  WiFi.setHostname(WIFI_HOSTNAME);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println("\nConnected to WiFi: " + WiFi.localIP().toString());
}

void initAP() {
  AP_PASSWORD = generateRandomNumberString(8);

  WiFi.softAP(AP_SSID, AP_PASSWORD);
  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
}

String convertToFirestoreTimestamp(time_t rawTime) {
  struct tm timeinfo;
  gmtime_r(&rawTime, &timeinfo);

  char timestamp[30];
  // Format time to Firestore timestamp format (ISO 8601)
  strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
  return String(timestamp);
}

void uploadTask(void *param) {
  for (;;) {
    delayedTime = 0;

    if (!isWifiConnected) {
      hasDataUploaded = false;
      vTaskDelay(pdMS_TO_TICKS(WIFI_CHECK_DELAY));
      continue;
    }

    while (delayedTime < UPLOAD_DELAY) {
      delayedTime += DISPLAY_REFRESH_DELAY;
      vTaskDelay(pdMS_TO_TICKS(DISPLAY_REFRESH_DELAY));
    }

    isUploading = true;

    time_t timestamp;
    time(&timestamp);

    // Newer than 1st January 2025
    if (timestamp >= 1735686000) {
      Serial.printf("Logging Temperature: %.2f°C at %ld\n", currentTemperature, timestamp);

      // Upload data to Firebase RTDB
      String dataPath = DATABASE_PATH + "/" + String(timestamp);
      if (Firebase.RTDB.setFloat(&fbdo, dataPath.c_str(), currentTemperature)) {
        Serial.println("✅ Data uploaded successfully!");
        isUploadSuccessful = true;
      } else {
        Serial.println("❌ RTDB error: " + fbdo.errorReason());
        isUploadSuccessful = false;
      }
    } else {
      Serial.println("Failed to get time, skipping data upload.");
    }

    isUploading = false;
    hasDataUploaded = true;
  }
}

void checkWifiTask(void *param) {
  for (;;) {
    isWifiConnected = WiFi.status() == WL_CONNECTED;
    wifiSignalStrength = WiFi.RSSI();
    vTaskDelay(pdMS_TO_TICKS(WIFI_CHECK_DELAY));
  }
}

void timesyncTask(void *param) {
  for (;;) {
    vTaskDelay(pdMS_TO_TICKS(TIMESYNC_DELAY));
    configTime(0, 0, NTP_SERVER.c_str());
    Serial.println("Synchronized time with NTP Server");
  }
}

void initMax6675() {
  pinMode(MAX6675_CS, OUTPUT);
  disableMax6675();
}

void initOled() {
  pinMode(OLED_CS, OUTPUT);
  disableOled();

  // Initialize SPI and Serial
  SPI.begin(OLED_SCK, -1, OLED_SDA, OLED_CS);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println("SSD1306 allocation failed");
    for (;;)
      ;
  }

  display.ssd1306_command(SSD1306_SETCONTRAST);
  display.ssd1306_command(1);  // Dim setting
  display.ssd1306_command(SSD1306_SETVCOMDETECT);
  display.ssd1306_command(0x00);  // Low Vcomh for dim display

  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);
  display.clearDisplay();
  display.setCursor(0, 0);

  display.print("Booting up...");
  display.display();
}

String getContentType(String filename) {
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".json")) return "application/json";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".jpg") || filename.endsWith(".jpeg")) return "image/jpeg";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".svg")) return "image/svg+xml";
  else if (filename.endsWith(".ttf")) return "font/ttf";
  else if (filename.endsWith(".woff")) return "font/woff";
  else if (filename.endsWith(".woff2")) return "font/woff2";
  else if (filename.endsWith(".mp4")) return "video/mp4";
  else if (filename.endsWith(".webp")) return "image/webp";
  return "text/plain";  // Default to plain text
}

void loadPreferences() {
  DISPLAY_REFRESH_DELAY = getPreference("refreshDelay", DISPLAY_REFRESH_DELAY_DEFAULT);
  WIFI_CHECK_DELAY = getPreference("wifiCheckDelay", WIFI_CHECK_DELAY_DEFAULT);

  WIFI_SSID = getPreference("ssid", WIFI_SSID_DEFAULT);
  WIFI_PASSWORD = getPreference("password", WIFI_PASSWORD_DEFAULT);

  NTP_SERVER = getPreference("ntpServer", NTP_SERVER_DEFAULT);
  TIMESYNC_DELAY = getPreference("timesyncDelay", TIMESYNC_DELAY_DEFAULT);

  USER_EMAIL = getPreference("userEmail", USER_EMAIL_DEFAULT);
  USER_PASSWORD = getPreference("userPassword", USER_PASSWORD_DEFAULT);
  API_KEY = getPreference("apiKey", API_KEY_DEFAULT);
  DATABASE_URL = getPreference("databaseUrl", DATABASE_URL_DEFAULT);
  DATABASE_PATH = getPreference("databasePath", DATABASE_PATH_DEFAULT);
  UPLOAD_DELAY = getPreference("uploadDelay", UPLOAD_DELAY_DEFAULT);
}

// Function to write preference (supports int and string)
void writePreference(const char *key, const char *value) {
  preferences.begin(PREFERENCES_NAMESPACE, false);
  preferences.putString(key, value);
  preferences.end();
}

void writePreference(const char *key, int value) {
  preferences.begin(PREFERENCES_NAMESPACE, false);
  preferences.putInt(key, value);
  preferences.end();
}

// Function to get preference (supports int and string)
String getPreference(const char *key, const char *defaultValue) {
  preferences.begin(PREFERENCES_NAMESPACE, true);
  String value = preferences.getString(key, defaultValue);
  preferences.end();
  return value;
}

int getPreference(const char *key, int defaultValue) {
  preferences.begin(PREFERENCES_NAMESPACE, true);
  int value = preferences.getInt(key, defaultValue);
  preferences.end();
  return value;
}

void clearPreferences() {
  preferences.begin(PREFERENCES_NAMESPACE, false);
  preferences.clear();
  preferences.end();
}

String generateRandomNumberString(int length) {
  String randomString = "";
  for (int i = 0; i < length; i++) {
    randomString += String(random(0, 10));  // Append a random digit (0-9)
  }
  return randomString;
}

void initDebugWebserver() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", "text/html");
  });

  // Catch-all to serve files or fallback to index.html (for Vue SPA)
  server.onNotFound([](AsyncWebServerRequest *request) {
    String path = request->url();
    if (LittleFS.exists(path)) {
      request->send(LittleFS, path, getContentType(path));
    } else {
      request->send(LittleFS, "/index.html", "text/html");
    }
  });

  // API endpoint example
  server.on("/api/display", HTTP_GET, [](AsyncWebServerRequest *request) {
    StaticJsonDocument<200> doc;
    doc["refreshDelay"] = getPreference("refreshDelay", DISPLAY_REFRESH_DELAY_DEFAULT);
    doc["wifiCheckDelay"] = getPreference("wifiCheckDelay", WIFI_CHECK_DELAY_DEFAULT);

    char jsonString[200];
    serializeJson(doc, jsonString);

    request->send(200, "application/json", jsonString);
  });

  server.on(
    "/api/display", HTTP_PUT, [](AsyncWebServerRequest *request) {}, NULL,
    [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, data, len);

      if (error) {
        request->send(400, "application/json", "{\"error\": \"Invalid JSON\"}");
        return;
      }

      bool updated = false;

      // Update preferences if values are present in the request
      if (doc.containsKey("refreshDelay")) {
        const unsigned int refreshDelay = doc["refreshDelay"];
        writePreference("refreshDelay", refreshDelay);
        updated = true;
      }

      if (doc.containsKey("wifiCheckDelay")) {
        const unsigned int wifiCheckDelay = doc["wifiCheckDelay"];
        writePreference("wifiCheckDelay", wifiCheckDelay);
        updated = true;
      }

      // Send 204 No Content if updated, otherwise 400 Bad Request
      if (updated) {
        request->send(204);  // No Content
      } else {
        request->send(400, "application/json", "{\"error\": \"No valid keys provided\"}");
      }
    });

  server.on("/api/wifi", HTTP_GET, [](AsyncWebServerRequest *request) {
    StaticJsonDocument<200> doc;
    doc["ssid"] = getPreference("ssid", WIFI_SSID_DEFAULT);
    doc["password"] = getPreference("password", WIFI_PASSWORD_DEFAULT);

    char jsonString[200];
    serializeJson(doc, jsonString);

    request->send(200, "application/json", jsonString);
  });

  server.on(
    "/api/wifi", HTTP_PUT, [](AsyncWebServerRequest *request) {}, NULL,
    [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, data, len);

      if (error) {
        request->send(400, "application/json", "{\"error\": \"Invalid JSON\"}");
        return;
      }

      bool updated = false;

      if (doc.containsKey("ssid")) {
        const char *ssid = doc["ssid"];
        writePreference("ssid", ssid);
        updated = true;
      }

      if (doc.containsKey("password")) {
        const char *password = doc["password"];
        writePreference("password", password);
        updated = true;
      }

      if (updated) {
        request->send(204);
      } else {
        request->send(400, "application/json", "{\"error\": \"No valid keys provided\"}");
      }
    });

  server.on("/api/time", HTTP_GET, [](AsyncWebServerRequest *request) {
    StaticJsonDocument<200> doc;
    doc["ntpServer"] = getPreference("ntpServer", NTP_SERVER_DEFAULT);
    doc["timesyncDelay"] = getPreference("timesyncDelay", TIMESYNC_DELAY_DEFAULT);

    char jsonString[200];
    serializeJson(doc, jsonString);

    request->send(200, "application/json", jsonString);
  });

  server.on(
    "/api/time", HTTP_PUT, [](AsyncWebServerRequest *request) {}, NULL,
    [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, data, len);

      if (error) {
        request->send(400, "application/json", "{\"error\": \"Invalid JSON\"}");
        return;
      }

      bool updated = false;

      if (doc.containsKey("ntpServer")) {
        const char *ntpServer = doc["ntpServer"];
        writePreference("ntpServer", ntpServer);
        updated = true;
      }
      if (doc.containsKey("timesyncDelay")) {
        const unsigned int timesyncDelay = doc["timesyncDelay"];
        writePreference("timesyncDelay", timesyncDelay);
        updated = true;
      }

      if (updated) {
        request->send(204);
      } else {
        request->send(400, "application/json", "{\"error\": \"No valid keys provided\"}");
      }
    });

  server.on("/api/firebase", HTTP_GET, [](AsyncWebServerRequest *request) {
    StaticJsonDocument<400> doc;
    doc["userEmail"] = getPreference("userEmail", USER_EMAIL_DEFAULT);
    doc["userPassword"] = getPreference("userPassword", USER_PASSWORD_DEFAULT);
    doc["apiKey"] = getPreference("apiKey", API_KEY_DEFAULT);
    doc["databaseUrl"] = getPreference("databaseUrl", DATABASE_URL_DEFAULT);
    doc["databasePath"] = getPreference("databasePath", DATABASE_PATH_DEFAULT);
    doc["uploadDelay"] = getPreference("uploadDelay", UPLOAD_DELAY_DEFAULT);

    char jsonString[400];
    serializeJson(doc, jsonString);

    request->send(200, "application/json", jsonString);
  });

  server.on(
    "/api/firebase", HTTP_PUT, [](AsyncWebServerRequest *request) {}, NULL,
    [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
      StaticJsonDocument<400> doc;
      DeserializationError error = deserializeJson(doc, data, len);

      if (error) {
        request->send(400, "application/json", "{\"error\": \"Invalid JSON\"}");
        return;
      }

      bool updated = false;

      if (doc.containsKey("userEmail")) {
        const char *userEmail = doc["userEmail"];
        writePreference("userEmail", userEmail);
        updated = true;
      }
      if (doc.containsKey("userPassword")) {
        const char *userPassword = doc["userPassword"];
        writePreference("userPassword", userPassword);
        updated = true;
      }
      if (doc.containsKey("apiKey")) {
        const char *apiKey = doc["apiKey"];
        writePreference("apiKey", apiKey);
        updated = true;
      }
      if (doc.containsKey("databaseUrl")) {
        const char *databaseUrl = doc["databaseUrl"];
        writePreference("databaseUrl", databaseUrl);
        updated = true;
      }
      if (doc.containsKey("databasePath")) {
        const char *databasePath = doc["databasePath"];
        writePreference("databasePath", databasePath);
        updated = true;
      }
      if (doc.containsKey("uploadDelay")) {
        const unsigned int uploadDelay = doc["uploadDelay"];
        writePreference("uploadDelay", uploadDelay);
        updated = true;
      }

      if (updated) {
        request->send(204);
      } else {
        request->send(400, "application/json", "{\"error\": \"No valid keys provided\"}");
      }
    });

  server.on("/api/reset", HTTP_POST, [](AsyncWebServerRequest *request) {
    clearPreferences();
    request->send(204);
  });

  server.begin();
}


// ---------------------------- SETUP AND LOOP ----------------------------

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing system...");

  initOled();

  if (touchRead(TOUCH_PIN) > 40) {
    // not touched
    initMax6675();
    mainSetup();
  } else {
    // touched
    debugSetup();
  }
}

void mainSetup() {
  display.setTextSize(9);
  loadPreferences();

  // Initialize important tasks on core 0
  xTaskCreatePinnedToCore(readSensorTask, "Read Sensor Task", 4096, NULL, 1, &readSensorTaskHandle, 0);

  // Initialize WiFi and Firebase
  initWiFi();
  xTaskCreatePinnedToCore(checkWifiTask, "Check Wifi Task", 4096, NULL, 3, &checkWifiTaskHandle, 1);
  configTime(0, 0, NTP_SERVER.c_str());
  xTaskCreatePinnedToCore(timesyncTask, "Timesync Task", 4096, NULL, 2, &timesyncTaskHandle, 1);

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  Firebase.reconnectWiFi(true);
  Firebase.begin(&config, &auth);

  unsigned long startMillis = millis();
  while (auth.token.uid.length() == 0) {
    Serial.println("Waiting for authentication...");
    delay(500);

    // Optional timeout to prevent infinite loop (10 seconds)
    if (millis() - startMillis > 10000) {
      Serial.println("Authentication timeout. Proceeding without Firebase.");
      break;
    }
  }

  if (auth.token.uid.length() > 0) {
    Serial.println("Firebase login successful. Token received. Waiting for time to sync...");

    // Wait until newer than 1st January 2025
    time_t timestamp;
    time(&timestamp);
    do {
      delay(500);
      time(&timestamp);
    } while (timestamp < 1735686000);

    Serial.println("Starting Upload Task...");
    xTaskCreatePinnedToCore(uploadTask, "Upload Task", 8192, NULL, 1, &uploadTaskHandle, 1);
  } else {
    Serial.println("Firebase authentication failed. Continuing without uploading.");
  }
}


void debugSetup() {
  Serial.println("Starting debug mode...");
  if (!LittleFS.begin()) {
    Serial.println("LittleFS Mount Failed");
    return;
  }

  initAP();

  initDebugWebserver();

  display.clearDisplay();
  display.setCursor(0, 0);

  display.println("Debug Mode");
  display.println();

  display.println("WiFi Hotspot:");
  display.println(AP_SSID);
  display.print("PWD: ");
  display.println(AP_PASSWORD);
  display.print("IP: ");
  display.println(WiFi.softAPIP());

  display.display();

  Serial.println("Debug mode ready");
}

void loop() {
}
