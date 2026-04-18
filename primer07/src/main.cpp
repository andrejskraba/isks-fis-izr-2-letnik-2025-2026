#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiAP.h>

const char* ssid = "esp32_1"; // ime esp32 modula kot WiFi dostopne točke (SSID - Service Set Identifier) 
const char* password = "12345678";

// ustvarimo objekt razreda WebServer, port 80 je prednastavljeni port za HTTP strežnike
WebServer server(80); // objekt server razreda WebServer (http port 80)

// spletno stran spravimo v spremenljivko HTML, ki je shranjena v flash pomnilniku (PROGMEM) in ne v RAM-u, da prihranimo dragoceni RAM pomnilnik na Esp32 modulu
const char HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="sl">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>esp32</title>
</head>
<body>
    <h1>Esp32 kot WiFi dostopna točka - Access Point AP!</h1>
</body>
</html>
)rawliteral"; // spletno stran spravimo v spremenljivko HTML

// tu določimo, kaj se dogodi, če uporabnik vpiše
// korenski ("root") naslov našega strežnika na esp32 moudlu, (npr. http://192.168.1.223/) - request "/" pomeni korenski naslov
void handle_root() {
  server.send(200, "text/html; charset=UTF-8", HTML); // pošljemo HTTP odgovor s statusno kodo 200 (OK), vsebino tipa "text/html" in besedilom "Pozdravljen svet...
  // strežnik klientu (Chrome) pošlje kot odziv, t.j. "response"
  // String "Pozdravljen svet...", koda 200 predstavlja standardni odziv na uspošno HTTP zahtevo ("request")
}

void setup() {
  // put your setup code here, to run once:
  // tu zapišemo kodo za nastavitve, koda bo izvedena le enkrat, ko se naprava zažene:

  Serial.begin(115200); // nastavimo hitrost serijske komunikacije na 115200 bitov/s
  
  // Vzpostavitev programske dostopne točke - "Soft Acces Point AP"
  // "software enabled access point", npr. računalnik, ki ni bil zasnovan kot
  // WiFi dostopna točka postane z ustreznim programjem dostopna točka
  // esp32 se spremeni v dostopno točko, na katero se lahko povežejo drugi WiFi klienti
  // (npr. prenosni računalnik, pametni telefon, tablica...)
  WiFi.softAP(ssid, password); // vzpostavimo WiFi dostopno točko z imenom "esp32_1" in geslom "12345678"
 
  // izpišemo IP naslov
  Serial.println("Vzpostavitev programske dostopne točke na esp32 modulu.");
  Serial.print("IP naslov esp32 dostopne točke: ");
  Serial.println(WiFi.softAPIP()); // prednastavljeni IP naslov esp32 dostopne točke je 192.168.4.1

  server.on("/", handle_root); // ko vpišemo IP naslov, na koncu "/", v brskalnik in pritisnemo Enter, tedaj strežnik na esp32 modulu dobi zahtevo "request" "/" in izvede se funkcija handle_root(), ki smo jo definirali zgoraj

  server.begin(); // zaženemo strežnik na esp32 modulu, ki posluša na portu 80 in čaka na zahteve ("requests") od klientov (npr. brskalnika Chrome)

  // ta del kode uporabimo kot indikator, da se je program prenesel na esp32 modul, modra LED dioda bo 2x utripnila
  pinMode(2, OUTPUT); // pin (nožica) št. 2 bo uporabljen kot digitalni izhod (na nožic 2 je povezana vgrajena LED dioda na ESP32)
  digitalWrite(2, HIGH); // na pin 2 zapišemo visoko vrednost (LED dioda se prižge)
  delay(250); // počakamo 250ms
  digitalWrite(2, LOW); // na pin 2 zapišemo nizko vrednost (LED dioda se ugasne)
  delay(250); // počakamo 250ms
  digitalWrite(2, HIGH); // na pin 2 zapišemo visoko vrednost (LED dioda se prižge)
  delay(250); // počakamo 250ms
  digitalWrite(2, LOW); // na pin 2 zapišemo nizko vrednost (LED dioda se ugasne)
  delay(250); // počakamo 250ms
  
}

void loop() {
  // put your main code here, to run repeatedly:
  // tu zapišemo kodo, ki se ponavljajoče izvaja, dokler je esp32 vklopljen:
  server.handleClient(); // preverimo, če je prišla kakšna zahteva ("request") od klienta (npr. brskalnika Chrome) in če je, tedaj jo obdelamo in pošljemo ustrezen odgovor ("response")
}