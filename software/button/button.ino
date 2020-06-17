/*  OnAir-Sign button software
    https://github.com/42CrMo4/OnAir-Sign
    
    based on :
      * Arduino Long Range Wireless Communication using HC-12: Example 01 by Dejan Nedelkovski, www.HowToMechatronics.com
      * FastLED
*/

#include <SoftwareSerial.h>
#include <FastLED.h>

SoftwareSerial HC12(10, 11); // HC-12 TX Pin, HC-12 RX Pin

// How many leds in your strip?
#define NUM_LEDS 12

// Data Pin for the LED Strip
#define DATA_PIN 3

// Define the array of leds
CRGB leds[NUM_LEDS];

// Button Pin Cherry MX switch
const int b = 2;

// variables
int state = 0;
byte incomingByte;
String readBuffer = "";

void setup() {
  HC12.begin(9600);               // Serial port to HC12
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);  // initialize the LED strip
  pinMode(b, INPUT);              // initialize button als input
  on_START();                     // run start sequent of LEDs
}
void loop() {
  // register messages from sign
  while (HC12.available()) {              // If HC-12 has data
    incomingByte = HC12.read();           // Store each icoming byte from HC-12
    readBuffer += char(incomingByte);     // Add each byte to ReadBuffer string variable
  }
  
  // register button press
  if (digitalRead (b) == HIGH) {
    if (state == 0) {
      HC12.write("1");    // transmit on to sign
      state = 1;          // remenber button state
    }
    else if (state == 1 ) {
      HC12.write("2");    // transmit off to sign
      state = 0;          // remenber button state
    }
    delay(800);
  }
  
  // convert string of HC-12 buffer to an integer
  int i = readBuffer.toInt();
  
  // if a signal has arrived decide which state 
  if (i == 1){          // message on from sign 
    on_LED();
    readBuffer = "";    // reset readbuffer 
  }
  if (i > 1){           // message off from sign
    off();
    readBuffer = "";    // reset readbuffer 
  }
  
  // reset readbuffer 
  readBuffer = "";
}

void on_START() {
  fill_solid(leds, NUM_LEDS, CRGB(255, 0, 0));      // set all LEDS to full brightness
  FastLED.show();                                   // display LED settings
  delay(500);
  // Now turn the LED off, then pause
  FastLED.clear();                                  // clear all LEDs
  FastLED.show();                                   // display LED settings

  for(int n = 0; n < NUM_LEDS; n = n + 1) {         // cicle trough all LED of the strip
      leds[n] = CRGB(255, 0, 0);                    // set the first LED to full
      leds[n-1] = CRGB(150, 0, 0);                  // reduce brightness of the previous LED
      leds[n-2] = CRGB(0, 0, 0);                    // turn off the previous of teh previous LED
      FastLED.show();                               // display LED settings
      FastLED.delay(100);
  }
  FastLED.clear();                                  // clear all LEDs
  FastLED.show();                                   // display LED settings
}

void on_LED() {
  fill_solid(leds, NUM_LEDS, CRGB(255, 0, 0));      // set all LEDS to full brightness  
  FastLED.show();                                   // display LED settings
}

void off() {
  FastLED.clear();                                  // clear all LEDs
  FastLED.show();                                   // display LED settings
}
