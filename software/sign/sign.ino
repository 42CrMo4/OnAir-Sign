/*  OnAir-Sign sign software
    https://github.com/42CrMo4/OnAir-Sign

    based on :
        Arduino Long Range Wireless Communication using HC-12: Example 01 by Dejan Nedelkovski, www.HowToMechatronics.com
        FastLED
*/

#include <SoftwareSerial.h>
#include <FastLED.h>

SoftwareSerial HC12(10, 11);                                 // HC-12 TX Pin, HC-12 RX Pin

// How many leds in your strip?
#define NUM_LEDS 20

// Data Pin for the LED Strip
#define DATA_PIN 3

// Define the array of leds
CRGB leds[NUM_LEDS];

// variables
byte incomingByte;
String readBuffer = "";

void setup() {
  HC12.begin(9600);                                         // Serial port to HC12
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // initialize the LED strip
  on_START();                                               // run start sequent of LEDs
}

void loop() {
  while (HC12.available()) {                                // If HC-12 has data
    incomingByte = HC12.read();                             // Store each icoming byte from HC-12
    readBuffer += char(incomingByte);                       // Add each byte to ReadBuffer string variable
  }

  delay(100);

  int i = readBuffer.toInt();                               // convert string of HC-12 buffer to an integer

  // if a signal has arrived decide which state
  if (i == 1) {                                             // message on from button
    on_LED();
    readBuffer = "";                                        // reset readbuffer
  }
  if (i > 1) {                                              // message off from button
    off();
    readBuffer = "";                                        // reset readbuffer
  }
  readBuffer = "";                                          // reset readbuffer
}

void on_START() {
  fill_solid(leds, NUM_LEDS, CRGB(255, 0, 0));              // set all LEDS to full brightness
  FastLED.show();                                           // display LED settings
  delay(500);
  // Now turn the LED off, then pause
  FastLED.clear();                                          // clear all LEDs
  FastLED.show();                                           // display LED settings
}

void on_LED() {
  fill_solid(leds, NUM_LEDS, CRGB(255, 0, 0));              // set all LEDS to full brightness
  FastLED.show();                                           // display LED settings
  HC12.write("1");                                          // transmit on to button
}

void off() {
  FastLED.clear();                                          // clear all LEDs
  FastLED.show();                                           // display LED settings
  HC12.write("2");                                          // transmit off to button
}
