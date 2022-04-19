#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "led_effect.cpp"

const char *SSID = "your_wifi_ssid";
const char *PWD = "your_wifi_pwd";

#define DATA_PIN 6
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 60
CRGB leds[NUM_LEDS];

#define BRIGHTNESS 96
#define FRAMES_PER_SECOND 120

// Web server running on port 80
WebServer server(80);

LED_EFFECT current_effect(LED_EFFECT_TYPE::STATIC, CRGB::White);

void connect_to_wifi()
{
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, PWD);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
    // we can even make the ESP32 to sleep
  }

  Serial.print("Connected. IP: ");
  Serial.println(WiFi.localIP());
}
void setup_routing()
{
  server.on("/rainbow", rainbow);
  server.on("/static", staticCol);
  server.on("/off", LedOff);

  // start server
  server.begin();
}

void rainbow()
{
  current_effect.type = LED_EFFECT_TYPE::RAINBOW;
}
void staticCol()
{
  current_effect.type = LED_EFFECT_TYPE::STATIC;
  current_effect.color = CRGB::White;
}
void LedOff()
{
  current_effect.type = LED_EFFECT_TYPE::NONE;
}

void setup()
{
  Serial.begin(9600);
  delay(3000); // 3 second delay for recovery
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  connect_to_wifi();
  setup_routing();
}

void loop()
{
  server.handleClient();

  if (current_effect.type == LED_EFFECT_TYPE::NONE)
  {
    fadeToBlackBy(leds, NUM_LEDS, 10);
  }
  else if (current_effect.type == LED_EFFECT_TYPE::RAINBOW)
  {
    fill_rainbow(leds, NUM_LEDS, 128, 7);
  }
  else if (current_effect.type == LED_EFFECT_TYPE::STATIC)
  {
    fill_solid(leds, NUM_LEDS, current_effect.color);
  }

  FastLED.show();
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}