#include <WiFi.h>
#include <math.h>
#include <iostream>
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
</b></p>
  </body>
</html> 
)""";
const char *web1 = R"""(
<html>
  <head>
    <title>ESP32 Web Page</title>
  </head>
  <body>
    <p>Write LED State</p>
    <p>Read<p>
   <p><b>
)""";
void setup() {
  Serial.begin(115200);
  //pinMode(36, ANALOG);
  adcAttachPin(36);
  analogReadResolution(10);
  analogSetPinAttenuation(36, ADC_0db);

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
  WiFiClient client = server.available();
  
  float volt = analogRead(36)*3.3/1023.0;
  float v2 = 3.3-volt;
  float res=volt/(v2/10000.0);
  float in = log(res/10000.0);
  float t3=1.0/((in/3977.0)+(1.0/(25+273.15)));
  t3-=273.15;
  float temp=1.0/(log(volt/2.5)/4300.0+1.0/298.0)-273.0;
  Serial.println(t3);
  int t2 = (int)temp;
  string s;
  s += to_string(t2);
  
  if (client) {
    //Serial.println("Получили клиента");
    boolean cLIB = true;
    String value;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //Serial.write(c);
        if (c == '\n' && cLIB) {
          client.print(httpResponseHeader);
          client.print(web1);
          client.print(std::stoi(s.c_str()));
          client.print(webPage);
          //Serial.printf("[HTTP RESP SENT]\n");
          break;
        }
        if (c == '\n') {
          cLIB = true;
        }
        else if (c != '\r') {
          cLIB = false;
                     
        }
      }
    }
    
    client.stop();
    //Serial.println("Disconnected client\n");
  }
}
