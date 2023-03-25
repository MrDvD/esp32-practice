#define bpin 13
#define lat 19
#define clock 18
#define data 23

byte numberSegments[10] = {
  0b01111101, 0b00100100, 0b01111010, 0b01110110, 0b00100111,
  0b01010111, 0b01011111, 0b01100100, 0b01111111, 0b01110111
};

int click = 0;
boolean butwas = true;

void setup() {
  pinMode(data, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(lat, OUTPUT);
  pinMode(bpin, INPUT_PULLUP);
}

void loop() {
  if (butwas && !digitalRead(bpin)) {
    delay(10);
    if (!digitalRead(bpin)) {
      click = (click + 1) % 10;
    }
  }
  butwas = digitalRead(bpin);
  digitalWrite(lat, LOW);
  shiftOut(data, clock, LSBFIRST, numberSegments[click]);
  digitalWrite(lat, HIGH);
}