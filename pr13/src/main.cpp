#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

const char* ssid = "kresilnik";
const char* password = "bled2024";

int portNumber = 8811; // preko vrat 8811 se bo esp32 povezal na node.js strežnik
WebSocketsClient webSocket; // ustvarimo objekt za delo s spletno vtičnico

String dataString; // za pošiljanje json sporočil

// na nožico 34 je priključen potenciometer 10kOhm
// ADC pretvornik (Analog to Digital Converter) na esp32
// vrednosti med 0V in 3.3V pretvorimo v vrednosti med 0 in 4095
// imamo 4096 vrednost 2^12=4096 12-bitna natančnost ADC pretvornika
const int nozicaPotenciometra = 34; // številka nožice, ki je povezana s potenciometrom

int vrednostPotenciometra = 0; // spremenljivka, v katero bomo shranili vrednost, ki jo preberemo s potenciometra

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

  // del, ki služi kot indikator, de je koda prenešena na modul
  pinMode(2, OUTPUT); // nožica št. 2 bo delovala kot izhod
  digitalWrite(2, HIGH); // na nožici 2 zapišemo visoko vrednost - LED zasveti
  delay(500); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
  digitalWrite(2, LOW); // na nožici 2 zapišemo nizko vrednost - LED ugasne
  delay(500); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
  digitalWrite(2, HIGH); // na nožici 2 zapišemo visoko vrednost - LED zasveti
  delay(500); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
  digitalWrite(2, LOW); // na nožici 2 zapišemo nizko vrednost - LED ugasne

  webSocket.begin("192.168.1.198", 8811); // priključimo se na node.js strežnik na IP naslovu 192...

}

void loop() {
  // put your main code here, to run repeatedly:
  // tu zapišemo kodo, ki se ponavljajoče izvaja

  webSocket.loop(); // vzdržujemo povezavo s strežnikom, preverjamo, če so npr. prišla sporočila, itd.

  vrednostPotenciometra = analogRead(nozicaPotenciometra); // preberemo vrednost s potenciometra in jo shranimo v spremenljivko
  //Serial.println(vrednostPotenciometra); // izpišemo vrednost potenciometra na serijski monitor oz. v terminal

  dataString = R"({"tipSporočila": "potenciometer", "pin": 34, "vrednost":)"; // R"() nam omogoča enostaven vnos niza znakov "literal string" brez potrebe po uporabi escape znakov, npr. za narekovaje
  dataString = dataString + String(vrednostPotenciometra) + "}"; // sestavimo JSON niz, ki ga bomo poslali na strežnik, vključno z vrednostjo potenciometra

  webSocket.sendTXT(dataString); // preko spletnega vtičnika pošljemo vrednost potenciometra v json obliki na stežnik

  // Serial.println(dataString); 

  delay(50); // na 50ms izpišemo vrednost potenciometra 

}