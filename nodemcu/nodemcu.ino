#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

WiFiClient client;

const char *ssid = "iot";  
const char *password = "wland217e8";

String thingSpeakAddress = "api.thingspeak.com";
String writeAPIKey;
String tsfield1Name;
String request_string;

float ph, suhu, kekeruhan, ketinggianAir;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  connectToWifi();
}

void connectToWifi(){
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

String splitString(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void loop() {
  if(WiFi.status() != WL_CONNECTED) {
    connectToWifi();
  }
  // put your main code here, to run repeatedly:
  if(Serial.available()) {
    String msg = "";
    while(Serial.available()) {
      msg += char(Serial.read());
      delay(50);
    }

    ph = splitString(msg, ';', 0).toFloat();
    suhu = splitString(msg, ';', 1).toFloat();
    kekeruhan = splitString(msg, ';', 2).toFloat();
    ketinggianAir = splitString(msg, ';', 3).toFloat();

    kirim_thingspeak(ph, suhu, kekeruhan, ketinggianAir);

    if (isnan(ph) || isnan(suhu) || isnan(kekeruhan) || isnan(ketinggianAir)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    
//    Serial.print(msg);
  }
}

void kirim_thingspeak(float ph, float suhu, float kekeruhan, float ketinggianAir) {
  if (client.connect("api.thingspeak.com", 80)) {
    request_string = "/update?";
    request_string += "key=";
    request_string += "JISZ721FTUEUM5G5";
    request_string += "&";
    request_string += "field1";
    request_string += "=";
    request_string += ph;
    request_string += "&";
    request_string += "field2";
    request_string += "=";
    request_string += suhu;
    request_string += "&";
    request_string += "field3";
    request_string += "=";
    request_string += kekeruhan;
    request_string += "&";
    request_string += "field4";
    request_string += "=";
    request_string += ketinggianAir;
    Serial.println(String("GET ") + request_string + " HTTP/1.1\r\n" +
                 "Host: " + thingSpeakAddress + "\r\n" +
                 "Connection: close\r\n\r\n");
                 
    client.print(String("GET ") + request_string + " HTTP/1.1\r\n" +
                 "Host: " + thingSpeakAddress + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 10000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }

    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }

    Serial.println();
    Serial.println("closing connection");

  }
}
