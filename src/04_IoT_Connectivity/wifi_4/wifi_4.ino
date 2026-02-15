#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* WIFI_SSID = ".....";
const char* WIFI_PASSWORD = "......";
const char* apiKey = "YOUR API KEY";
void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
      Serial.print('.');
      delay(1000);
  }
  Serial.println(WiFi.localIP());

  String inputText = "你知道中央大學嗎?";
  Serial.println(inputText);

  String apiUrl = "https://api.openai.com/v1/chat/completions";
  String payload = "{\"model\": \"gpt-3.5-turbo\", \"messages\": [{\"role\": \"user\", \"content\": \"" + inputText + "\"}], \"max_tokens\": 100}";

  WiFiClientSecure client;
  client.setInsecure();  // 測試階段使用，正式環境建議導入憑證

  HTTPClient http;
  http.begin(client, apiUrl);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", "Bearer " + String(apiKey));

  int httpResponseCode = http.POST(payload);
  if (httpResponseCode == 200) {
    String response = http.getString();
    Serial.println(response);

    DynamicJsonDocument jsonDoc(2048);
    deserializeJson(jsonDoc, response);
    String outputText = jsonDoc["choices"][0]["message"]["content"];
    // Serial.println(outputText); // 這邊可以印出回傳的Json資訊
  } else {
    Serial.printf("Error %i\n", httpResponseCode);
    Serial.println(http.getString());  // 印出錯誤訊息方便除錯
  }
}

void loop() {
}
