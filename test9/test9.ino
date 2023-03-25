#include <regex>

char html[1000];
const char* query = R"""(
    GET / HTTP/1.1
    Host: 192.168.180.242:7005
    User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/110.0
    Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/;q=0.8
    Accept-Language: en-US,en;q=0.5
    Accept-Encoding: gzip, deflate
    DNT: 1
    Connection: keep-alive
    Upgrade-Insecure-Requests: 1

)""";

std::regex typ_get("GET \\/ ");
std::regex value_get("POST \\/led_pwm_ajax");
std::regex typ_end("\n\n");
std::regex value_end("\n\n\\w*=(\\d*)");
std::cmatch output;

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 1000; ++i) {
    char buf[1];
    sprintf(buf, "%c", query[i]);
    strncat(html, buf, 1);
    Serial.write(html);
    Serial.write('\n');
    if (std::regex_search(html, output, typ_get)) {
      Serial.println("[TYP_GOT]");
      for (int j = i + 1; j < 1000; ++j) {
        char buf[1];
        sprintf(buf, "%c", query[j]);
        strncat(html, buf, 1);
        Serial.write(html);
        Serial.write('\n');
        
        if (std::regex_search(html, output, typ_end)) {
          Serial.println("[END_GOT]");
          break;
        } else {
          Serial.println("[END_NOT_GOT]");
        }
      }
      break;
      } else {
        Serial.println("[TYP_NOT_GOT]");
      }
  }
}

void loop() {
}
