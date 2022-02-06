#define Te 320

#include <PubSubClient.h>

#include "WiFi.h"
#include <WiFiMulti.h>

#include "WiFiUtils.h"

WiFiClient client;
PubSubClient mqttClient(client);
WiFiUtils wifiUtils;
WiFiMulti wifiMulti;

#include "settings.h"

void setup() {

  Serial.begin(115200);
  pinMode(TRANSMITTER_PIN, OUTPUT);

  Serial.println("Setup started");
  WiFi.mode(WIFI_STA); //be only wifi client, not station

  WiFiUtils::printNetworks();
  WiFi.hostname("Gate_Opener");

  wifiMulti.addAP(WIFI_SSID, WIFI_PASS);
  while (wifiMulti.run() != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(WIFI_SSID);
    delay(3000);
  }

  if (USE_GOOGLE_DNS)
    WiFiUtils::setGoogleDNS();
  WiFiUtils::printWiFiStatus();
  WiFiUtils::printCurrentNet();
  WiFiUtils::printWifiData();

  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCallback(callback);
  Serial.print("Init complete");
}



void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println("Opening gates");

  RfTransmitt(GATES_CODE, 6);

}

void loop()
{

  bool connected = true;
  if (!mqttClient.connected()) {
    connected = false;
    if (mqttClient.connect("Arduino-Gates")) {
      Serial.println("MQTT reconnected");
      connected = true;
      mqttClient.subscribe(GATES_TOPIC);
    } else {
      connected = false;
      Serial.print("MQTT failed, rc=");
      Serial.print(mqttClient.state());
    }
  }

  mqttClient.loop();
}

void SendCameBit(byte b)
{
  delayMicroseconds(Te);
  if (!b) digitalWrite(TRANSMITTER_PIN, HIGH);
  delayMicroseconds(Te);
  digitalWrite(TRANSMITTER_PIN, HIGH);
  delayMicroseconds(Te);
  digitalWrite(TRANSMITTER_PIN, LOW);
}


void RfTransmitt(char *codeString, unsigned int numberOfShipments)
{
  int codeLength = strlen(codeString);
  if (codeLength != 12 && codeLength != 24)
  {
    Serial.println("incorrect code.");
    return;
  }

  byte code[codeLength];
  for (int i = 0; i < codeLength; i++) {
    code[i] = codeString[i] == '0' ? 0 : 1;
  }

  byte codeUpdate[codeLength];

  int number = 0;

  switch (codeLength) {
    case 12:
      //обратный порядок
      for (int i = 5; i >= 0; i--) {
        codeUpdate[number] = code[i];
        //Serial.print(codeUpdate[number]);
        number++;
      }
      //Serial.print(" ");
      //прямой порядок
      for (int i = 6; i < 12; i++) {
        codeUpdate[number] = code[i];
        //Serial.print(codeUpdate[number]);
        number++;
      }
      break;
    case 24:
      //обратный порядок для всех символов
      for (int i = 1; i <= 3; i++) {
        for (int j = i * 8 - 1; j >= 8 * (i - 1); j--) {
          codeUpdate[number] = code[j];
          //Serial.print(codeUpdate[number]);
          number++;
        }
        //Serial.print("-");
      }
      break;
  }


  for (int i = 0; i < numberOfShipments; i++) // посылку посылаем как и брелок - NS раза подряд.
  {
    digitalWrite(TRANSMITTER_PIN, HIGH);
    delayMicroseconds(Te);
    digitalWrite(TRANSMITTER_PIN, LOW); // посылаем стартовый импульс
    for (int j = 0; j < codeLength; j++) {
      SendCameBit(codeUpdate[j]); // побитово перебираем и посылаем код
    }
    delay(16);
  }
}
