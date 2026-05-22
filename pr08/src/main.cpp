#include <Arduino.h>
#include "WiFi.h"
#include "WebSocketsClient.h" // Sockets - množina (s na koncu)

const char* ssid = "kresilnik";
const char* password = "bled2024";

int portNumber = 8811; // preko vrat 8811 se bo esp32 povezal na node.js strežnik

WebSocketsClient webSocket; // kreiramo objekt razreda WebSocketsClient 

uint8_t LED1pin = 2; // 8-bitna celoštevilska spremenljivka


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
  webSocket.begin("192.168.1.198", portNumber); // priključimo se na node.js strežnik na IP naslovu 192... port 8811

  // del, ki služi kot indikator, de je koda prenešena na modul
  pinMode(LED1pin, OUTPUT); // nožica št. 2 bo delovala kot izhod
  digitalWrite(LED1pin, HIGH); // na nožici 2 zapišemo visoko vrednost - LED zasveti
  delay(500); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
  digitalWrite(LED1pin, LOW); // na nožici 2 zapišemo nizko vrednost - LED ugasne
  delay(500); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
  digitalWrite(LED1pin, HIGH); // na nožici 2 zapišemo visoko vrednost - LED zasveti
  delay(500); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
  digitalWrite(LED1pin, LOW); // na nožici 2 zapišemo nizko vrednost - LED ugasne

  Serial.println(WiFi.localIP());
  Serial.println(WiFi.getMode());

}

void loop() {
  // put your main code here, to run repeatedly:
  // tu zapišemo kodo, ki se ponavljajoče izvaja
  webSocket.loop(); // spremlja komunikacijo na webSocket-u 

}