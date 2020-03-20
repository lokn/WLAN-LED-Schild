/*
   NeoPixel Webserver
   Copyright (c) 2017, Agis Wichert
   https://www.youtube.com/watch?v=6U4JssgWjr8
   
   ESP Webserver
   Copyright (c) 2015, Majenko Technologies
   
   Edited by Louis

   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

 * * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

 * * Redistributions in binary form must reproduce the above copyright notice, this
     list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.

 * * Neither the name of Majenko Technologies nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#define DEBUG 1

#if DEBUG
#define DEBUGP(str) Serial.println(str)
#else
#define DEBUGP(str)
#endif

// Neopixel
#include <Adafruit_NeoPixel.h>

// ESP8266
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// Webserver Config
const char *ssid = "Festnetz";
const char *password = "EinStuhlHat4Beine";
ESP8266WebServer server ( 80 );

// Neopixel Config
#define NeoPIN D1
#define NUM_LEDS 22
uint8_t brightness = 150;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(22, NeoPIN, NEO_RGB + NEO_KHZ800);


const int led = 13;

// programs
uint16_t duration_on  = 0;
uint16_t duration_off = 0;
uint32_t last_switch  = 0;
bool leds_on          = false;
uint32_t blink_color  = 0;

void setup () {

  Serial.begin(115200);

  // NeoPixel start
  Serial.println();
  strip.setBrightness(brightness);
  strip.begin();
  strip.show();
  delay(50);
  Serial.println("NeoPixel started");

  // Webserver
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);

  WiFi.begin(ssid, password);
  Serial.println("");
  
  // Wait for connection
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if ( MDNS.begin("esp8266")) {
    Serial.println ( "MDNS responder started" );
  }

  // what to do with requests
  server.on ( "/", handleRoot );
  server.onNotFound ( handleNotFound );
  server.begin();



  Serial.println ( "HTTP server started" );
}

void loop () {
  // waiting fo a client
  server.handleClient();

  // flag check
  if (blink_color) {
    blinking();
  }
}

void handleRoot() {
  Serial.print("Start; ");
  Serial.println(millis());
  DEBUGP("handleRoot");
  DEBUGP("Client connected");
  digitalWrite (led,1);

  for (int i = 0; i < server.args(); i++) {

    if (server.argName(i).startsWith("l")) {
      DEBUGP(server.arg(i));

      strip.setPixelColor(
        server.argName(i).substring(1).toInt(), // led position bsp: Parameter l12 -> 12
        (uint32_t) strtol(&(server.arg(i))[1], NULL, 16) // color
      );

    } else if(server.argName(i) == "all") {
      DEBUGP(server.arg(i));
      setAll((uint32_t) strtol(&(server.arg(i))[1], NULL, 16));
      
    } else if (server.argName(i).startsWith("blink")) {
      // blinkXXXXXdelayXXXXX => X in Millis
      blink_color = (uint32_t) strtol(&(server.arg(i))[1], NULL, 16);
      duration_on  = (uint32_t) strtol(&(server.argName(i).substring(5,10))[0], NULL, 16);
      duration_on  = (uint32_t) strtol(&(server.argName(i).substring(15,18))[0], NULL, 16);
    }
    
  }
  
  strip.show();
  DEBUGP("Stop; ");
  DEBUGP(millis());
  
  server.send(200);
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send ( 404, "text/plain", message );
  digitalWrite ( led, 0 );
}

void setAll (uint32_t c) {
  for (int i = 0; i < NUM_LEDS; i++) strip.setPixelColor(i,c);
}

void blinking() {
  if (leds_on && (millis() > last_switch+duration_on)) {
    leds_on = false;
    last_switch = millis();
    setAll(0);
  } else if ( millis() > last_switch+duration_off
  ) {
    leds_on = true;
    last_switch = millis();
    setAll(blink_color);
  }
}
