#include <WiFi.h>
#include <iostream>
#include <regex>
using namespace std;

const char ssid[] = "Redmi Note 10S";
const char pass[] = "11111111";

WiFiServer server(80);

String httpResponseHeader =
"HTTP/1.1 200 OK\r\n" \
"Content-Type: text/html\r\n" \
"Connection: close\r\n" \
"\r\n";

const char *webPage = R"""(
<html>
  <head>
    <title>ESP32 Web Page</title>
  </head>
  <body>
    <p>Write LED State</p>
    <input type="range" id="range_pwm"
      min="0" max="1023" value="0">
    <script>
      document.getElementById("range_pwm").oninput = function() {
        var post_request = "pwm_value=" + document.getElementById("range_pwm").value;
        var xhttp = new XMLHttpRequest();
        xhttp.open("POST", post_request, true);
        xhttp.send(post_request);
      };
    </script>
  </body>
</html> 
)""";

std::regex typ_get("GET \\/ ");
std::regex value_get("POST \\/led_pwm_ajax");
std::regex typ_end("\n\n");
std::regex value_end("\n\n\\w*=(\\d*)");

void setup() {
  Serial.begin(115200);

  // PINS
  ledcSetup(0, 2048, 10);
  ledcAttachPin(26, 0);

  // WIFI
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi connect failed! Rebooting ...\n");
  }
  Serial.printf("Connected to Network\n");
  Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());

  // TCP SERVER
  server.begin();
  Serial.print("TCP server started\n");
}

void loop() {
  delay(400);
  Serial.println("[WAIT FOR A CLIENT]");
  WiFiClient client = server.available();

  if (client) {
    Serial.println("[GOT A CLIENT]");
    
    char query[1000];
    std::cmatch output;
    
    while (client.connected()) {
      if (client.available()) {
        char buf[1];
        char r = client.read();
        sprintf(buf, "%c", r);
        strncat(query, buf, 1);
        Serial.write("[CHAR_READ]");
        Serial.write('\n');
        Serial.write(r);
        Serial.write('\n');
        Serial.write(buf);
        Serial.write('\n');
        Serial.write(query);
        Serial.write('\n');

        if (std::regex_search(query, output, typ_get)) {
          while (true) {
            Serial.println("[TYP_GET]");
            char buff[1];
            char d = client.read();
            sprintf(buff, "%c", d);
            strncat(query, buff, 1);
            Serial.write('\n');
            Serial.write(d);
            Serial.write('\n');
            Serial.write(buf);
            Serial.write('\n');
            Serial.write(query);
            Serial.write('\n');

            delay(8);
            if (std::regex_search(query, output, typ_end)) {
              Serial.println("[TYP_END]");
              client.print(httpResponseHeader);
              client.print(webPage);
              client.stop();
              Serial.println("Disconnected client\n");
              break;
            }
          }
          break;
        } else if (std::regex_search(query, output, value_get)) {
          Serial.println("[VALUE_GET]");
          while (true) {
            char f = client.read();
            sprintf(buf, "%c", f);
            strncat(query, buf, 1);
            Serial.write('\n');
            Serial.write(r);
            Serial.write('\n');
            Serial.write(buf);
            Serial.write('\n');
            Serial.write(query);
            Serial.write('\n');

            if (std::regex_search(query, output, value_end)) {
              Serial.println("[TYP_END]");
              std::string value = output[1].str();
              ledcWrite(0, std::stoi(value));
              client.stop();
              Serial.println("Disconnected client\n");
              break;
            }
          }
        }
        Serial.println("==========");
      }
    }
  }
}
