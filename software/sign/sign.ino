/*    Arduino Long Range Wireless Communication using HC-12
                      Example 01
   by Dejan Nedelkovski, www.HowToMechatronics.com
*/
#include <SoftwareSerial.h>
#include <FastLED.h>

SoftwareSerial HC12(10, 11); // HC-12 TX Pin, HC-12 RX Pin

// How many leds in your strip?
#define NUM_LEDS 20

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 3

// Define the array of leds
CRGB leds[NUM_LEDS];
byte incomingByte;
String readBuffer = "";

void setup() {
  Serial.begin(9600);             // Serial port to computer
  HC12.begin(9600);               // Serial port to HC12
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  on_START();
}
void loop() {
  while (HC12.available()) {             // If HC-12 has data
    incomingByte = HC12.read();          // Store each icoming byte from HC-12
    readBuffer += char(incomingByte);    // Add each byte to ReadBuffer string variable
  }
  delay(100);
  Serial.print(readBuffer);
  int i = readBuffer.toInt();
  if (i == 1){
    on_LED();
    readBuffer = "";
  }
  if (i > 1){
    off();
    readBuffer = "";
  }
  readBuffer = "";
}

void on_START() {
  fill_solid(leds, NUM_LEDS, CRGB(255, 0, 0));
  FastLED.show();
  delay(500);
  // Now turn the LED off, then pause
  FastLED.clear();
  FastLED.show();
}

void on_LED() {
  fill_solid(leds, NUM_LEDS, CRGB(255, 0, 0));
  FastLED.show();
  HC12.write("1");
}

void off() {
  FastLED.clear();
  FastLED.show();
  HC12.write("2");
}
