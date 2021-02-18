#include <FastLED.h>

// these constants won't change:
const int ledPin = 5;
const int micPin = A1;

//Light Source  R G B Values
//Warm Fluorescent  255, 244, 229
//Standard Fluorescent  244, 255, 250
//Cool White Fluorescent  212, 235, 255
//Full Spectrum Fluorescent 255, 244, 242

const int numLeds = 64;
const int brightness[3] = {255, 244, 229};

// set up the led array
CRGB leds[numLeds];

const int micThreshold = 5;
const int numModes = 4;

const int changeTimeout = 100;
const int updateTimeout = 200;
int changeTimeoutCount = 0;
int updateTimeoutCount = updateTimeout;

// these variables will change:
int currentMode = 0;

void ledModeHigh() {
    for( int i = 0; i < numLeds; i++) {
      leds[i] = CRGB(brightness[0], brightness[1], brightness[2]); 
    }
    FastLED.show();
}

void ledModeLow() {
    for( int i = 0; i < numLeds; i++) {
      leds[i] = CRGB(brightness[0]/5, brightness[1]/5, brightness[2]/5); 
    }
    FastLED.show();
}


// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
void ledModeParty() {
    static uint8_t colorIndex = 0;
    for( int i = 0; i < numLeds; i++) {
        leds[i] = ColorFromPalette( PartyColors_p, colorIndex, brightness[0], LINEARBLEND);
        colorIndex += 3;
    }
    FastLED.show();
    
}

void ledModeOff() {
    for( int i = 0; i < numLeds; i++) {
      leds[i] = CRGB::Black; 
    }
    FastLED.show();
}

void changeMode() {
    // cycle through the modes
    currentMode++;
    if (currentMode == numModes) {
      currentMode = 0;
    }
}

void setup() {
    // add the leds
    FastLED.addLeds<WS2812, ledPin, GRB>(leds, numLeds);
    // turn on the mic pin
    pinMode(micPin, INPUT);
    // and let's keep track
    Serial.begin(9600);       // use the serial port
    // start off
    ledModeOff();
}


void loop() {
  int current = analogRead(micPin);
  // if the volume maxes out we see big spikes at the top and bottom of the wave - change mode
  //  Serial.print(current);
  //  Serial.print("\n");
  if (current <= micThreshold && changeTimeoutCount >= changeTimeout) {
    changeMode();
    // make sure we don't change modes again for a bit - to let the sound 
    // quiet down
    changeTimeoutCount = 0;
    // and make sure the leds update next round
    updateTimeoutCount = 0;
  } else {
    changeTimeoutCount++;
  }

  if (updateTimeoutCount <= 0) {
    switch (currentMode) {
        case 1:
           ledModeParty();
        break;
        case 2:
           ledModeHigh();
        break;
        case 3:
           ledModeLow();
        break;
        default:
           ledModeOff();
        break;
    }
    // reset update timer - so we don't update again for a bit
    updateTimeoutCount = updateTimeout;
    
  } else {
    updateTimeoutCount--;
  }
  delay(1);        // delay in between reads for stability
}
