#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define WAIT 400          // Time between a new set of animations
#define STATE_BASE 0      //The starting state
#define STATE_ONE 1       //The first state
#define STATE_TWO 2       //The second state
#define STATE_THREE 3     //The third state
#define NUMPIXELS 8       //Amount of NeoPixel attached to the Strip
#define XPOS 0            // Indexes into the 'icons' array in function below
#define YPOS 1
#define DELTAX 2
#define PIN 6                //The pin connected to the NeoPixel
#define BUTTONPIN 7          //The pin connected to the button
#define DELAYVAL 300         //Time (in milliseconds) to pause between things
#define PAUSE 2000           //Time between OLED display changes
#define REST 800             //Amount time between the inner display changes
#define Scan 250             //Amount of time between the lines changing
#define MODE 3               //The amount of different cases and modes
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D  ///< See datasheet for Address; 0x3D for 128x64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int PRESSCOUNTER = 0;  //The amount of time the button is pressed
#define NUMFLAKES 8    // Number of snowflakes in the animation example

#define LOGO_HEIGHT 20
#define LOGO_WIDTH 20
static const unsigned char PROGMEM logo_bmp[] = {};

void setup() {
  Serial.begin(9600);
  pinMode(PIN, OUTPUT);
  pinMode(BUTTONPIN, INPUT);
  pixels.begin();  //INITIALIZE the NeoPixel (REQUIRED)
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }

  display.display();
}

void loop() {
  pixels.clear();  //sets all the pixels to 'off'

  if (digitalRead(7) == HIGH) {
    PRESSCOUNTER++;
    delay(DELAYVAL);
  }

  if (PRESSCOUNTER > MODE) {
    PRESSCOUNTER = 0;
  }

  switch (PRESSCOUNTER) {  //choose what to display based on the PRESSCOUNTER value
    case STATE_BASE:
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(13, 228, 20));  //colors go from 0,0,0 up to 255,255,255
        pixels.show();                                       //Sends the updated data to the Strip
      }
      Scanner();
      break;
    case STATE_ONE:
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 71, 71));
        pixels.show();
      }
      Drawcircle();
      delay(REST);
      Fillcircle();
      delay(REST);
      break;
    case STATE_TWO:
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(191, 64, 191));
        pixels.show();
      }
      Drawtriangle();
      delay(REST);
      Filltriangle();
      delay(REST);
      break;
    case STATE_THREE:
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(248, 131, 121));
        pixels.show();
      }
      Drawrect();
      delay(REST);
      Fillrect();
      delay(REST);
      break;
  }
}

void Drawcircle(void) {
  display.clearDisplay();

  for (int16_t i = 0; i < max(display.width(), display.height()) / 2; i += 2) {
    display.drawCircle(display.width() / 2, display.height() / 2, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }

  delay(PAUSE);
}

void Fillcircle(void) {
  display.clearDisplay();

  for (int16_t i = max(display.width(), display.height()) / 2; i > 0; i -= 3) {
    // The INVERSE color is used so circles alternate white/black
    display.fillCircle(display.width() / 2, display.height() / 2, i, SSD1306_INVERSE);
    display.display();  // Update screen with each newly-drawn circle
    delay(1);
  }
}

void Drawtriangle(void) {
  display.clearDisplay();

  for (int16_t i = 0; i < max(display.width(), display.height()) / 2; i += 5) {
    display.drawTriangle(
      display.width() / 2, display.height() / 2 - i,
      display.width() / 2 - i, display.height() / 2 + i,
      display.width() / 2 + i, display.height() / 2 + i, SSD1306_WHITE);
    display.display();
    delay(1);
  }

  delay(PAUSE);
}

void Filltriangle(void) {
  display.clearDisplay();

  for (int16_t i = max(display.width(), display.height()) / 2; i > 0; i -= 5) {
    // The INVERSE color is used so triangles alternate white/black
    display.fillTriangle(
      display.width() / 2, display.height() / 2 - i,
      display.width() / 2 - i, display.height() / 2 + i,
      display.width() / 2 + i, display.height() / 2 + i, SSD1306_INVERSE);
    display.display();
    delay(1);
  }
}

void Drawrect(void) {
  display.clearDisplay();

  for (int16_t i = 0; i < display.height() / 2; i += 2) {
    display.drawRect(i, i, display.width() - 2 * i, display.height() - 2 * i, SSD1306_WHITE);
    display.display();  // Update screen with each newly-drawn rectangle
    delay(1);
  }
  delay(PAUSE);
}

void Fillrect(void) {
  display.clearDisplay();

  for (int16_t i = 0; i < display.height() / 2; i += 3) {
    // The INVERSE color is used so rectangles alternate white/black
    display.fillRect(i, i, display.width() - i * 2, display.height() - i * 2, SSD1306_INVERSE);
    display.display();  // Update screen with each newly-drawn rectangle
    delay(1);
  }
}

void Scanner() {
  int16_t i;
  display.clearDisplay();  // Clear display buffer

  for (i = 0; i < display.width(); i += 4) {
    display.drawLine(0, 0, i, display.height() - 1, SSD1306_WHITE);
    display.display();  // Update screen with each newly-drawn line
    delay(1);
  }
  for (i = 0; i < display.height(); i += 4) {
    display.drawLine(0, 0, display.width() - 1, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  delay(Scan);

  display.clearDisplay();

  for (i = 0; i < display.width(); i += 4) {
    display.drawLine(0, display.height() - 1, i, 0, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  for (i = display.height() - 1; i >= 0; i -= 4) {
    display.drawLine(0, display.height() - 1, display.width() - 1, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  delay(Scan);

  display.clearDisplay();

  for (i = display.width() - 1; i >= 0; i -= 4) {
    display.drawLine(display.width() - 1, display.height() - 1, i, 0, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  for (i = display.height() - 1; i >= 0; i -= 4) {
    display.drawLine(display.width() - 1, display.height() - 1, 0, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  delay(Scan);

  display.clearDisplay();

  for (i = 0; i < display.height(); i += 4) {
    display.drawLine(display.width() - 1, 0, 0, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  for (i = 0; i < display.width(); i += 4) {
    display.drawLine(display.width() - 1, 0, i, display.height() - 1, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  delay(Scan);

  // for (i = 0; i < display.height(); i += 3) {                       //For([Start at zero] x=0; [Stop at 9] x<10: [Count by one] x=x+1) {}
  //   display.drawLine(display.width() - 1, 0, 0, i, SSD1306_WHITE);  //display.drawline(StartX, StartY. Endx, EndY, WHITE);
  //   display.display();                                              //Height = 64; Width = 128
  //   delay(1);
  // }
  // for (i = 0; i < display.width(); i += 3) {  //upper right corner(second part)
  //   display.drawLine(display.width() - 1, 0, i, display.height() - 1, SSD1306_WHITE);
  //   display.display();
  //   delay(1);
  // }
  // display.clearDisplay();

  // for (i = 0; i < display.height(); i += 3) {
  //   display.drawLine(display.height() - 1, 0, 0, i, SSD1306_WHITE);
  //   display.display();
  //   delay(1);
  // }
  // for (i = 0; i < display.width(); i += 3) {
  //   display.drawLine(display.height() - 1, 0, i, display.height() - 1, SSD1306_WHITE);
  //   display.display();
  //   delay(1);
  // }
}