#include <WiFi.h>
#include <regex>
#include <cstring>  //strcat

const char ssid[] = "Redmi Note 10S";
const char pass[] = "11111111";

std::regex get_q("^GET");
std::regex post_q("\\w*=(\\d*)j$");
std::cmatch output;

String header =
  "HTTP/1.1 200 OK\r\n"
  "Content-Type: text/html\r\n"
  "Connection: close\r\n"
  "\r\n";

const char *postreq = R"""(
POST /pwm_value HTTP/1.1
Host: 192.168.180.242:8000
User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/110.0
Accept: */
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate
Content-Type: application/x-www-form-urlencoded; charset=UTF-8
X-Requested-With: XMLHttpRequest
Content-Length: 21
Origin: http://192.168.180.242:8000
DNT: 1
Connection: keep-alive
Referer: http://192.168.180.242:8000/

range_pwm=753j
)""";

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
               data: {range_pwm: $('#range_pwm').val() + 'j'
               }
            });
         });
      </script>
   </body>
</html>
)""";

WiFiServer server(80);
char html[2000];

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
  server.on();
  Serial.print("TCP server started\n");

  delay(10000);
  boolean newline = false;
  for (int i = 0; i < 2000; ++i) {
    char c = postreq[i];
    strncat(html, &c, 1);
    while (true) {
      for (int j = i + 1; j < 2000; ++j) {
        char c = postreq[j];
        strncat(html, &c, 1);
        Serial.write(html);
        Serial.write('\n');
        if (c == '\n' && newline) {
          while (true) {
            for (int k = j + 1; k < 2000; ++k) {
              char c = postreq[k];
              Serial.write(c);
              Serial.write('\n');
              strncat(html, &c, 1);
              if (c == 'j') {
                break;
              }
            }
            Serial.println("[FOR_EXITED]");
            break;
          }
          break;
        } else if (c == '\n') {
          newline = true;
        } else if (c != '\r') {
          newline = false;
        }
      }
      break;
    }
    break;
  }
  Serial.write(html);
  if (std::regex_search(html, output, post_q)) {
    Serial.println("[WORKS]");
  } else {
    Serial.println("[DOESN'T WORK]");
  }
}

void loop() {
}
