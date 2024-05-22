#include <Wire.h>
#include "RTClib.h"
#include <Adafruit_SSD1306.h>


#define OLED_RESET 4


Adafruit_SSD1306 display(OLED_RESET);


RTC_DS1307 RTC;


const int NUM_SLIDERS = 6;
const int analogInputs[NUM_SLIDERS] = { A0, A1, A2, A3, A6, A7 };
const int NUM_BUTTONS = 5;
const int buttonInputs[NUM_BUTTONS] = { 2, 3, 4, 5, 6 };

int analogSliderValues[NUM_SLIDERS];
int buttonValues[NUM_BUTTONS];

void setup() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
  }

  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttonInputs[i], INPUT_PULLUP);
  }

  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(100);
  display.clearDisplay();
  Wire.begin();
  RTC.begin();

  if (!RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  updateSliderValues();
  sendSliderValues();  // Actually send data (all the time)
                       //   printSliderValues(); // For debug
  delay(10);
  DateTime now = RTC.now();
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  delay(50);



  display.setTextSize(2);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(20, 4);
  display.print(now.hour(), DEC);
  display.print(':');
  display.print(now.minute(), DEC);
  display.print(':');
  display.print(now.second(), DEC);

  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(35, 22);
  display.print(now.year(), DEC);
  display.print('/');
  display.print(now.month(), DEC);
  display.print('/');
  display.print(now.day(), DEC);
  display.display();
  display.clearDisplay();
}

void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    analogSliderValues[i] = analogRead(analogInputs[i]);
  }
  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttonValues[i] = digitalRead(buttonInputs[i]);
  }
}

void sendSliderValues() {
  String builtString = String("");

  for (int i = 0; i < NUM_SLIDERS; i++) {
    builtString += "s";
    builtString += String((int)analogSliderValues[i]);

    if (i < NUM_SLIDERS - 1) {
      builtString += String("|");
    }
  }

  if (NUM_BUTTONS > 0) {
    builtString += String("|");
  }

  for (int i = 0; i < NUM_BUTTONS; i++) {
    builtString += "b";
    builtString += String((int)buttonValues[i]);

    if (i < NUM_BUTTONS - 1) {
      builtString += String("|");
    }
  }

  Serial.println(builtString);
}

void printSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    String printedString = String("Slider #") + String(i + 1) + String(": ") + String(analogSliderValues[i]) + String(" mV");
    Serial.write(printedString.c_str());

    if (i < NUM_SLIDERS - 1) {
      Serial.write(" | ");
    } else {
      Serial.write("\n");
    }
  }
}