#include <fstream>
#include <LittleFS.h>
#include <iostream>

// START MEM: 0x290000
//   SIZE FS: 0x160000 --- written in mklittlefs --size

// COMMANDS:
// ./mklittlefs --create ~/Documents/Programming/esp32/labs/littlefs/data file.bin --size 0x160000
// esptool.py --port=/dev/ttyUSB0 --chip=esp32 --baud=921600 write_flash 0x290000 file.bin

void setup() {
   Serial.begin(115200);
   
   LittleFS.begin(false);
   
//   File root = LittleFS.open("/");
//   File file1 = root.openNextFile();
//   while (file1.available()) {
//      char line = file1.read();
//      Serial.print(line);
//   }
   
   
   // WRITE
   // File webpage_file = LittleFS.open("/webpage.html", "w");
   // File script_file = LittleFS.open("/script.js", "w");
   
   // Serial.write(webpage.c_str());
   // Serial.write(script.c_str());
   
   // std::cout << webpage << '\n';
   // std::cout << script << '\n';
   
   // webpage_file.write(*webpage.c_str());
   // script_file.write(*script.c_str());
   
   // webpage_file.close();
   // script_file.close();
   
   // READ HTML
   if (LittleFS.exists("/webpage.html")) {
      Serial.println("[HTML_EXISTS]");
      File webpage_file = LittleFS.open("/webpage.html", "r");
      while (webpage_file.available()) {
         char line = webpage_file.read();
         Serial.print(line);
      }
      webpage_file.close();
   } else {
      Serial.println("[HTML_DOESNT_EXIST]");
   }
   
   // READ JS
   if (LittleFS.exists("/script.js")) {
      Serial.println("[JS_EXISTS]");
      File script_file = LittleFS.open("/script.js", "r");
      while (script_file.available()) {
         char line = script_file.read();
         Serial.print(line);
      }
      script_file.close();
   } else {
      Serial.println("[JS_DOESNT_EXIST]");
   }
   
   // REMOVE
   // LittleFS.remove("/webpage.html");
   // LittleFS.remove("/script.js");
   
   LittleFS.end();
}

void loop() {
}
