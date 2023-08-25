#include <WiFi.h>
#include <FirebaseESP32.h>
#include <HTTPClient.h>
#include <WiFiManager.h>
#include <time.h>
#include <TimeLib.h>

#define FIREBASE_HOST 
#define FIREBASE_AUTH 

#define MQ2_PIN 32
#define LED_PIN 17
#define BUZZER_PIN 2
#define RESET_PIN 4 // 

#define API_KEY "5098649"

FirebaseData firebaseData;
HTTPClient http;

const char* customCSS = "<style>"
                        "body { background-color: orange; }"
                        "input[type=submit] { background-color: white; }"
                        "<h1>Configuração do Dispositivo CAUSTIC</h1>"
                        "</style>"; // Crie uma string contendo o código CSS e HTML personalizado

WiFiManagerParameter custom_phone_number("phone", "Número de telefone", "", 20); // Crie um parâmetro personalizado para o número de telefone

void setup() {
  Serial.begin(115200);

  pinMode(MQ2_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  const char* ntpServer = "a.st1.ntp.br";
  const long  gmtOffset_sec = -3 * 60 * 60;
  const int   daylightOffset_sec = 0;


  pinMode(RESET_PIN, INPUT_PULLUP); // Defina o pino do botão de redefinição como entrada com pull-up
  if (digitalRead(RESET_PIN) == LOW) { // Verifique se o botão de redefinição está pressionado
    WiFiManager wifiManager; // Crie uma instância do WiFiManager
    wifiManager.resetSettings(); // Redefina as configurações do WiFiManager
    ESP.restart(); // Reinicie o ESP32
  }
  
  WiFiManager wifiManager; // Crie uma instância do WiFiManager
  wifiManager.setCustomHeadElement(customCSS); // Adicione o estilo CSS e HTML personalizado ao portal cativo
  wifiManager.addParameter(&custom_phone_number); // Adicione o parâmetro personalizado ao WiFiManager
  wifiManager.startConfigPortal("projetoCaustic", "caustic123"); // Inicie o portal cativo para configurar as credenciais de WiFi
  
  Serial.println();
  Serial.print("Conectado com esse IP: ");
  Serial.println(WiFi.localIP());
  digitalWrite(LED_PIN, HIGH);
  delay(1500);
  digitalWrite(LED_PIN, LOW);
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  digitalWrite(BUZZER_PIN, LOW); 
  digitalWrite(LED_PIN, LOW); 

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {
  
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Erro ao obter a hora atual");
    return;
  }

  char buffer[64];
  strftime(buffer, sizeof(buffer), "%A, %d de %B de %Y %H:%M:%S", &timeinfo); 

  int mq2Value = analogRead(MQ2_PIN);

 char timestamp[20];
  strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &timeinfo);

if (Firebase.getInt(firebaseData, "/statusBuzzer")) {
  int statusBuzzer = firebaseData.intData();
  if (mq2Value >= 6000 && statusBuzzer == 1) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
    digitalWrite(BUZZER_PIN, HIGH);
    Firebase.setInt(firebaseData, "/statusLed", 1);

    String message_n3 = "Gás detectado! Muito cuidado! Nessa faixa, a concentração de gás atinge um nível perigoso, representando um alto risco de explosão, incêndio ou intoxicação grave. Recalibração acionada. Reativação do sistema acionada.\n\nNÍVEL CRÍTICO:\nData e hora: " + String(buffer) + "\nAcesse para mais informações: https://caustic.web.app/sobre.html";
    String encodedMessage_n3 = encodeURIComponent(message_n3);
    String phoneNumber = custom_phone_number.getValue(); // Obtenha o valor do parâmetro personalizado
    String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&text=" + encodedMessage_n3 + "&apikey=" + String(API_KEY);
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);
    } else {
      Serial.println("Erro na requisição HTTP!");
    }
    http.end();
    Firebase.setString(firebaseData, "/statusMensagem", "Mensagem enviada!");
    
  } else if (mq2Value >= 2001 && mq2Value <= 5999 && statusBuzzer == 1) {
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
    delay(500);
    digitalWrite(BUZZER_PIN, HIGH);
    Firebase.setInt(firebaseData, "/statusLed", 1);

    String message_n2 = "Gás detectado! Fique atento! Tomamos as medidas preventivas e iniciamos ações para mitigar o vazamento.\n\nNÍVEL MÉDIO:\nData e hora: " + String(buffer) + "\nAcesse para mais informações: https://caustic.web.app/sobre.html";
    String encodedMessage_n2 = encodeURIComponent(message_n2);
    String phoneNumber = custom_phone_number.getValue(); // Obtenha o valor do parâmetro personalizado
    String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&text=" + encodedMessage_n2 + "&apikey=" + String(API_KEY);
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);
    } else {
      Serial.println("Erro na requisição HTTP!");
    }
    http.end();
    Firebase.setString(firebaseData, "/statusMensagem", "Mensagem enviada!");
  } else if (mq2Value >= 1500 && mq2Value <= 2000 && statusBuzzer == 1) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
    digitalWrite(BUZZER_PIN, HIGH);
    Firebase.setInt(firebaseData, "/statusLed", 1);

    String message_n1 = "Gás detectado! Fique tranquilo! Esse nível de gás é comum. Nenhum sistema de segurança foi acionado.\n\nNÍVEL BAIXO:\nData e hora: " + String(buffer) + "\nAcesse para mais informações: https://caustic.web.app/sobre.html";
    String encodedMessage_n1 = encodeURIComponent(message_n1);
    String phoneNumber = custom_phone_number.getValue(); // Obtenha o valor do parâmetro personalizado
    String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&text=" + encodedMessage_n1 + "&apikey=" + String(API_KEY);
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);
    } else {
      Serial.println("Erro na requisição HTTP!");
    }
    http.end();
    Firebase.setString(firebaseData, "/statusMensagem", "Mensagem enviada!");
  } else {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    Firebase.setInt(firebaseData, "/statusLed", 0);
    Firebase.setString(firebaseData, "/statusMensagem", "Nível de gás normal!");
  }
}
  FirebaseJson json;
  json.set("value", mq2Value);
  json.set("timestamp", timestamp);

  Firebase.pushJSON(firebaseData, "/statusSensor", json);
  delay(1000);
}

String encodeURIComponent(const String &str) {
  String result = "";
  for (unsigned int i = 0; i < str.length(); i++) {
    char c = str.charAt(i);
    if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '-' || c == '_' || c == '.' || c == '~') {
      result += c;
    } else {
      result += '%';
      result += "0123456789ABCDEF"[c >> 4];
      result += "0123456789ABCDEF"[c & 15];
    }
  }
  return result;
}

