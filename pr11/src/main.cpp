#include <Arduino.h>
#include "WiFi.h"
#include "WebSocketsClient.h" // Sockets - množina (s na koncu)

#include <ArduinoJson.h>

const char* ssid = "kresilnik";
const char* password = "bled2024";

int portNumber = 8811; // preko vrat 8811 se bo esp32 povezal na node.js strežnik

bool trenutnoStanje; // trenutno stanje na našem vhodnem pinu (18), ki je povezan s tipko
const int BUTTON_PIN = 18; // številka nožice, ki je povezana s tipko

String dataString; // spremenljivka, v katero bomo shranili sporočilo, ki ga bomo poslali preko spletnega vtičnika 8811 (WebSocket-a) na node.js strežnik

WebSocketsClient webSocket; // objekt razreda WebSocketsClient, ki nam omogoča komunikacijo preko protokola WebSocket s strežnikom, ki teče na računalniku (node.js strežnik)

// koda, ki se izvede ob prejemu sproročila preko spletnega vtičnika (WebSocket-a) 8811
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length){
  switch(type) {
    case WStype_TEXT:
    {
    // v tem delu prenešeni string z JSON sporočilom pretvorimo v JSON objekt
    // <256> določa kapaciteto objekta doc - StaticJsonDocument doc(256); - 256 bajtov, kar je dovolj za naše potrebe
    JsonDocument doc;
    // deserializacija JSON sporočila - prenešeni string z JSON sporočilom pretvorimo v JSON objekt doc
    // sporočilo bo shranjeno v objektu doc
    DeserializationError error = deserializeJson(doc, payload, length);
    // preverimo, ali je prišlo do napake pri deserializaciji JSON sporočila
    if (error) {
      Serial.print("Napaka pri deserializaciji JSON sporočila: ");
      Serial.println(error.c_str());
      return;
    }

    char tip[40]; // spremenljivka za besedo iz JSON strukture
    memset(tip, 0, sizeof(tip)); // izbrišemo vse vrednosti iz znakovnega niza tip, da bo prazen
    strcpy(tip, doc["tipSporočila"]); // iz JSON strukture v spremenljivko tip kopiramo vrednost pod ključem "tipSporočila" | v našem primeru je tip="LED"

    if(strcmp(tip, "LED") == 0) { // če je tip sporočila "LED", potem izvedemo naslednje
      int vrednost = doc["vrednost"]; // v spremenljivko vrednost zapišemo vrednost iz JSON strukture pod ključem "vrednost" | v našem primeru je vrednost=1 ali 0
      int pin = doc["pin"]; // v spremenljivko pin zapišemo vrednost iz JSON strukture pod ključem "pin" | v našem primeru je pin=2
      if(pin==2) { // če je pin 2, potem izvedemo naslednje
        if(vrednost == 0) {
          digitalWrite(2, LOW); // na nožici 2 zapišemo nizko vrednost - LED ugasne
        } else if(vrednost == 1) {
          digitalWrite(2, HIGH); // na nožici 2 zapišemo visoko vrednost - LED zasveti
        }
      }
    }
    } // konec if stavka, ki preverja tip sporočila

      Serial.print("Prejeto sporočilo: ");
      Serial.println((char*)payload);

      default:
        break;
    
  } // konec switch stavka
} // konec funkcije webSocketEvent

void setup() {
  // put your setup code here, to run once:
  // tu zapišemo kodo, ki se bo izvedla le enkrat - nastavitve

  Serial.begin(115200); // hitrost komunikacije preko serijskega protokola

  // WiFi.mode(WIFI_STA); // nastavitev WiFi načina delovanja na "Station" (STA) - esp32 se bo obnašal kot odjemalec, ki se povezuje na WiFi omrežje
  WiFi.begin(ssid, password); // poženemo WiFi omrežje - se priklopimo

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Povezovanje z WiFi omrežjem...");
  }
  
  Serial.println("Povezava z WiFi omrežjem je vzpostavljena.");
  Serial.print("IP naslov esp32 modula je: ");
  Serial.println(WiFi.localIP());

  // poženemo WebSocket strežnik in določimo "callback" funkcijo, ki se izvede ob vsakem dogodku na spletnem vtičniku (WebSocket-u)
  webSocket.begin("192.168.1.198", 8811); // povezava na node.js strežnik, ki teče na IP naslovu 192.168.1.198, preko vrat 8811

  // del, ki služi kot indikator, de je koda prenešena na modul
  pinMode(2, OUTPUT); // nožica št. 2 bo delovala kot izhod
  pinMode(BUTTON_PIN, INPUT); // nožica št. 18 bo delovala kot vhod
  digitalWrite(2, HIGH); // na nožici 2 zapišemo visoko vrednost - LED zasveti
  delay(500); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
  digitalWrite(2, LOW); // na nožici 2 zapišemo nizko vrednost - LED ugasne
  delay(500); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
  digitalWrite(2, HIGH); // na nožici 2 zapišemo visoko vrednost - LED zasveti
  delay(500); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
  digitalWrite(2, LOW); // na nožici 2 zapišemo nizko vrednost - LED ugasne

  Serial.println(WiFi.localIP());
  Serial.println(WiFi.getMode());

  webSocket.onEvent(webSocketEvent); // določimo funkcijo, ki se izvede ob vsakem dogodku na spletnem vtičniku (WebSocket-u) - ob prejemu sporočila, ob povezavi, ob prekinitvi povezave, ...

}

void loop() {
  // put your main code here, to run repeatedly:
  // tu zapišemo kodo, ki se ponavljajoče izvaja

  webSocket.loop(); // spremlja prisotnost klientov in posluša njihove WebSocket zahteve ("request") 

  trenutnoStanje = digitalRead(BUTTON_PIN); // preberemo stanje na nožici 18 in ga shranimo v spremenljivko
  //Serial.println(trenutnoStanje); // izpišemo trenutno stanje na nožici 18 v serijski monitor

  if (trenutnoStanje == HIGH) { // če je stanje na nožici 18 visoko, to pomeni, da je tipka pritisnjena
    dataString = R"({"tipSporočila":"tipka","pin":18,"vrednost":1})"; // R"()" - raw string literal, ki nam omogoča enostaven vnos
    webSocket.sendTXT(dataString); // pošljemo sporočilo preko spletnega vtičnika (WebSocket-a) na node.js strežnik
  } else { // sicer je stanje na nožici 18 nizko, kar pomeni, da tipka ni pritisnjena
    dataString = R"({"tipSporočila":"tipka","pin":18,"vrednost":0})"; // R"()" - raw string literal, ki nam omogoča enostaven vnos
    webSocket.sendTXT(dataString); // pošljemo sporočilo preko spletnega vtičnika (WebSocket-a) na node.js strežnik
  }

  delay(100);

}