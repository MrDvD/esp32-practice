#include <WiFi.h>
#include <regex>
#include <cstring> //strcat

const char ssid[] = "Redmi Note 10S";
const char pass[] = "11111111";

std::regex typ_get("GET \\/ ");
std::regex value_get("POST \\/led_pwm_ajax");
std::regex typ_end("\r\n\r\n$"); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
std::regex value_end("\r\n\r\n\\w*=(\\d*)%26");
std::cmatch output;

String httpResponseHeader =
"HTTP/1.1 200 OK\r\n" \
"Content-Type: text/html\r\n" \
"Connection: close\r\n" \
"\r\n";

const char *webPage = R"""(
<!DOCTYPE html>
<html>
   <meta charset="UTF-8"></meta>
   <head>
      <title>Some name</title>
   </head>
   <body>
      <p>Write LED Value using AJAX</p>
      <input type="range" id="range_pwm" min="0" max="1023" value="0"></input>
      <button id="pressed">POST</button>

      <!-- Загружает последнюю версию JQuery -->
      <script src="https://code.jquery.com/jquery-3.6.4.min.js"></script>
      <script>
         $('#pressed').on('click', function() {
            $.ajax({
               type: "post",
               url: '/led_pwm_ajax',
               data: {range_pwm: $('#range_pwm').val() + '&'
               }
            });
         });
      </script>
   </body>
</html>
)""";

WiFiServer server(80);

void setup() {
  // Serial.begin(115200);
  
  // PINS
  ledcSetup(0, 2048, 10);
  ledcAttachPin(26, 0);

  // WIFI
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    // Serial.printf("WiFi connect failed! Rebooting ...\n");
  }
  // Serial.printf("Connected to Network\n");
  // Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());

  // TCP SERVER
  server.begin();
  // Serial.print("TCP server started\n");
  
  delay(2000);
}

void loop() {
  delay(400);
  // Serial.println("[WAIT FOR A CLIENT]");
  WiFiClient client = server.available();

  
  if (client) {
    // Serial.println("[GOT A CLIENT]");
    delay(1000);
    std::cmatch output;
    char html[1000];
    html[0] = '\0';

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        strncat(html, &c, 1);
        if (std::regex_search(html, output, typ_get)) {
          // Serial.println("[TYP_GOT]");
          while (client.connected()) {
            c = client.read();
            strncat(html, &c, 1);
            
            if (std::regex_search(html, output, typ_end)) {
              // Serial.println("[END_GOT]");
              client.print(httpResponseHeader);
              client.print(webPage);
              break;
            } else {
              // Serial.println("[END_NOT_GOT]");
            }
          }
          break;
        } else if (std::regex_search(html, output, value_get)) {
            // Serial.println("[VALUE_GOT]");
            while (true) {
              c = client.read();
              strncat(html, &c, 1);
              if (std::regex_search(html, output, value_end)) {
                // Serial.println("[END_VAL_GOT]");f
                std::string value = output[1].str();
                ledcWrite(0, std::stoi(value));
                break;
              }
            }
          break;
        }
      }
    }
  
      client.stop();
      // Serial.println("[CLIENT DISCONNECTED]");
    }
  }
