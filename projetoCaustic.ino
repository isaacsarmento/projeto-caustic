#include <WiFi.h>
#include <FirebaseESP32.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

#define FIREBASE_HOST 
#define FIREBASE_AUTH 
#define WIFI_SSID 
#define WIFI_PASSWORD 

#define MQ2_PIN 32
#define LED_PIN 17
#define BUZZER_PIN 2
#define BUTTON_PIN 4

#define PHONE_NUMBER 
#define API_KEY 

FirebaseData firebaseData;
HTTPClient http;

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando no WIFI...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Wifi Conectado");
    delay(300);
  }
  Serial.println();
  Serial.print("Conectado no IP: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  pinMode(MQ2_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  int mq2Value = analogRead(MQ2_PIN);
  
  if (mq2Value >= 1000) {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    Firebase.setInt(firebaseData, "/statusLed", 1);
    Firebase.setInt(firebaseData, "/statusBuzzer", 1);

    String url = "https://api.callmebot.com/whatsapp.php?phone=" + String(PHONE_NUMBER) + "&text=Gas+detectado!&apikey=" + String(API_KEY);
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);
    } else {
      Serial.println("Erro na Requisição HTTP");
    }
    http.end();
    
  } else {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    Firebase.setInt(firebaseData, "/statusLed", 0);
    Firebase.setInt(firebaseData, "/statusBuzzer", 0);
  }
  
  Firebase.setInt(firebaseData, "/statusSensor", mq2Value);

  delay(1000);
}