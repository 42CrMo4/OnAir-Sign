/*    Arduino Long Range Wireless Communication using HC-12
                      Example 01
   by Dejan Nedelkovski, www.HowToMechatronics.com
*/
#include <SoftwareSerial.h>
#include <FastLED.h>

SoftwareSerial HC12(10, 11); // HC-12 TX Pin, HC-12 RX Pin

// How many leds in your strip?
#define NUM_LEDS 12

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 3

// Define the array of leds
CRGB leds[NUM_LEDS];

const int b = 2;

int state = 0;
//int art = 0;
byte incomingByte;
String readBuffer = "";

void setup() {
  Serial.begin(9600);             // Serial port to computer
  HC12.begin(9600);// Serial port to HC12
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  pinMode(b, INPUT);
  on_START();
}
void loop() {
  while (HC12.available()) {        // If HC-12 has data
    incomingByte = HC12.read();          // Store each icoming byte from HC-12
    readBuffer += char(incomingByte);    // Add each byte to ReadBuffer string variable
  }
  while (Serial.available()) {      // If Serial monitor has data
    HC12.write(Serial.read());      // Send that data to HC-12
  }
  if (digitalRead (b) == HIGH) {
    if (state == 0) {
      HC12.write("1");
      state = 1;
    }
    else if (state == 1 ) {
      HC12.write("2");
      state = 0;
    }
    delay(800);
//    art = 1;
  }
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

     for(int n = 0; n < NUM_LEDS; n = n + 1) {
      leds[n] = CRGB(255, 0, 0);
      leds[n-1] = CRGB(150, 0, 0);
      leds[n-2] = CRGB(0, 0, 0);
//      leds[n].maximizeBrightness();
      FastLED.show();
      FastLED.delay(100);
     }
  FastLED.clear();
  FastLED.show();
}

void on_LED() {
  fill_solid(leds, NUM_LEDS, CRGB(255, 0, 0));
  FastLED.show();
}

void off() {
  FastLED.clear();
  FastLED.show();
}
