#include "DHT.h"
#include "LedControl.h"
LedControl lc = LedControl(8, 4, 7, 1); //3เลขแรก เรียง din,clk,cs
#define DHTPIN 2     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
int relay1 = 11; // ต่อกับขา in1 ของบอร์ดรีเลย์
int relay2 = 12; // ต่อกับขา in2 ของบอร์ดรีเลย์
int relay3 = 13; // ต่อกับขา in3 ของบอร์ดรีเลย์

int state = 0;
char msg[50];
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  dht.begin();
  //LCD MAX 7219
  lc.shutdown(0, false);
  lc.setIntensity(0, 15);
  lc.clearDisplay(0);
  lc.setRow(0, 7, B01011011);
  lc.setRow(0, 6, B00001111);
  lc.setRow(0, 5, B01110111);
  lc.setRow(0, 4, B00000101);
  lc.setRow(0, 3, B00001111);
  lc.setRow(0, 2, B00000000);
  lc.setRow(0, 1, B00111110);
  lc.setRow(0, 0, B01100111);
  delay(2000);

}

void loop() {
  delay(500);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  delay(2000);
  MaxDisplay(t, h);
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    Error();
    delay(3000);
    Please();
    delay(3000);
    RESET();
    delay(3000);
    return;
  }
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);
  if (t >= 37.50 ) {
    TempSerial(h, t, f, hif, hic);
    digitalWrite(relay1, 0);
    digitalWrite(relay2, 0);
    digitalWrite(relay3, 0);
    state = digitalRead(relay1);
    Serial.println(state);
    delay(3000);
  }
  else {
    TempSerial(h, t, f, hif, hic);
    digitalWrite(relay1, 1);
    digitalWrite(relay2, 1);
    digitalWrite(relay3, 1);
    delay(3000);
    state = digitalRead(relay1);
    Serial.println(state);
  }

}
void MaxDisplay(float TT, float HH) {
  int XX = (int)(TT * 10);
  int Tempp0 = XX % 10; XX /= 10;
  int Tempp1 = XX % 10; XX /= 10;
  int Tempp2 = XX % 10;
  int YY = (int)(HH * 10);
  int Humid0 = YY % 10; YY /= 10;
  int Humid1 = YY % 10; YY /= 10;
  int Humid2 = YY % 10;
  lc.setDigit(0, 7, Tempp2, false);
  lc.setDigit(0, 6, Tempp1, true);
  lc.setDigit(0, 5, Tempp0, false);
  lc.setRow(0, 4, B00001101); // Code=tabcdefgh
  lc.setDigit(0, 3, Humid2, false);
  lc.setDigit(0, 2, Humid1, true);
  lc.setDigit(0, 1, Humid0, false);
  lc.setRow(0, 0, B00010111); // Code=tabcdefgh
}
void Error() {
  lc.setRow(0, 7, B01001111);
  lc.setRow(0, 6, B00000101);
  lc.setRow(0, 5, B00000101);
  lc.setRow(0, 4, B00011101);
  lc.setRow(0, 3, B10000101);
  lc.setRow(0, 2, B10000000);
  lc.setRow(0, 1, B10000000);
  lc.setRow(0, 0, B10000000);
}

void Please() {
  lc.setRow(0, 7, B01100111);
  lc.setRow(0, 6, B00001110);
  lc.setRow(0, 5, B01001111);
  lc.setRow(0, 4, B01110111);
  lc.setRow(0, 3, B01011011);
  lc.setRow(0, 2, B11001111);
  lc.setRow(0, 1, B10000000);
  lc.setRow(0, 0, B10000000);
}
void RESET() {
  lc.setRow(0, 7, B00000101);
  lc.setRow(0, 6, B01001111);
  lc.setRow(0, 5, B01011011);
  lc.setRow(0, 4, B01001111);
  lc.setRow(0, 3, B10001111);
  lc.setRow(0, 2, B10000000);
  lc.setRow(0, 1, B10000000);
  lc.setRow(0, 0, B10000000);
}
void TempSerial(float h, float t, float f, float hif, float hic) {
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\n");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\n");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
  Serial.println("========================");
}
