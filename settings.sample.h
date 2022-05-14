// MQTT settings
#define MQTT_SERVER "192.168.0.5"
#define MQTT_PORT 1883
#define MQTT_USER ""
#define MQTT_PASSWORD ""

#define GATES_TOPIC_OPEN "esp/street/gates/open"
#define GATES_TOPIC_AVAILABLE "esp/street/gates/available"

#define GATES_CODE "110011910110100011101010"

#if defined(ESP32)
#define TRANSMITTER_PIN DAC1
#endif
#if defined(ESP8266)
#define TRANSMITTER_PIN D2
#endif


// WIFI settings
#define WIFI_SSID  "Me"             //  your network SSID (name)
#define WIFI_PASS "secure_password" // your network password
#define USE_GOOGLE_DNS false
