#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "kresilnik";
const char* password = "bled2024";

// ustvarimo objekt razreda WebServer, port 80 je prednastavljeni port za HTTP strežnike
WebServer server(80); // objekt server razreda WebServer (http port 80)

uint8_t LED1pin = 2; // 8-bitna celoštevilska spremenljivka
bool LED1status = LOW; // logična spremenljivka, ki hrani stanje LED1, LOW pomeni, da je LED ugasnjena, HIGH bi pomenilo, da je LED prižgana

const char HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="sl">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>esp32</title>
</head>
<body>
    <h1>Vpišite http://192.168.1.163/1 za vklop LED diode ali http://192.168.1.163/0 za izklop, t.j. /1 ali /0 na koncu.</h1>
</body>
</html>
)rawliteral"; // spletno stran spravimo v spremenljivko HTML

const char HTML1[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="sl">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>esp32</title>
</head>
<body>
    <h1>Prejet ukaz za vklop LED diode.</h1>
</body>
</html>
)rawliteral"; // spletno stran spravimo v spremenljivko HTML1

const char HTML0[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="sl">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>esp32</title>
</head>
<body>
    <h1>Prejet ukaz za izklop LED diode.</h1>
</body>
</html>
)rawliteral"; // spletno stran spravimo v spremenljivko HTML0

// tu določimo, kaj se dogodi, če uporabnik vpiše
// korenski ("root") naslov našega strežnika na esp32 moudlu, (npr. http://192.168.1.223/) - request "/" pomeni korenski naslov
void handle_root() {
  server.send(200, "text/html; charset=UTF-8", HTML); // pošljemo HTTP odgovor s statusno kodo 200 (OK), vsebino tipa "text/html" in besedilom "Pozdravljen svet...
  // strežnik klientu (Chrome) pošlje kot odziv, t.j. "response"
  // String "Pozdravljen svet...", koda 200 predstavlja standardni odziv na uspošno HTTP zahtevo ("request")
}

// tu določimo, kaj se dogodi, če uporabnik vpiše
// zahtevo "/1" (npr. http://192.168.1.163/1) - request "/1"
void handle_1() {
  LED1status = HIGH; // spremenljivki LED1status dodelimo vrednost HIGH, kar pomeni, da bo LED prižgana, v zanki loop() ves čas preverjamo vrednost te spremenljivke in glede na to prižigamo ali ugašamo LED diodo
  server.send(200, "text/html; charset=UTF-8", HTML1); // pošljemo HTTP odgovor s statusno kodo 200 (OK), vsebino tipa "text/html" in besedilom v spremenljivki HTML1
}

// tu določimo, kaj se dogodi, če uporabnik vpiše
// zahtevo "/0" (npr. http://192.168.1.163/0) - request "/0"
void handle_0() {
  LED1status = LOW; // spremenljivki LED1status dodelimo vrednost LOW, kar pomeni, da bo LED ugasnjena, v zanki loop() ves čas preverjamo vrednost te spremenljivke in glede na to prižigamo ali ugašamo LED diodo
  server.send(200, "text/html; charset=UTF-8", HTML0); // pošljemo HTTP odgovor s statusno kodo 200 (OK), vsebino tipa "text/html" in besedilom v spremenljivki HTML0
}

void setup() {
  // put your setup code here, to run once:
  // tu zapišemo kodo za nastavitve, koda bo izvedena le enkrat, ko se naprava zažene:

  Serial.begin(115200); // nastavimo hitrost serijske komunikacije na 115200 bitov/s
  WiFi.begin(ssid, password); // povežemo se na WiFi omrežje z imenom "kresilnik" in geslom "bled2024"

  while (WiFi.status() != WL_CONNECTED) { // dokler se modul ne poveže na WiFi omrežje, ponavljamo zanko
    delay(500); // počakamo 500ms preden preverimo stanje povezave znova
    Serial.println("Povezovanje z WiFi omrežjem..."); // izpišemo sporočilo na serijski monitor, da vidimo, da se še vedno poskuša povezati
  }

  Serial.println("Povezava z WiFi omrežjem je vzpostavljena."); // izpišemo sporočilo, ko je povezava uspešna
  Serial.print("IP naslov esp32 modula je: ");
  Serial.println(WiFi.localIP()); // izpišemo IP naslov, ki ga je modul dobil od WiFi omrežja

  server.on("/", handle_root); // ko vpišemo IP naslov, na koncu "/", v brskalnik in pritisnemo Enter, tedaj strežnik na esp32 modulu dobi zahtevo "request" "/" in izvede se funkcija handle_root(), ki smo jo definirali zgoraj
  server.on("/1", handle_1); // ko vpišemo IP naslov, na koncu "/1", v brskalnik in pritisnemo Enter, tedaj strežnik na esp32 modulu dobi zahtevo "request" "/1" in izvede se funkcija handle_1(), ki smo jo definirali zgoraj
  server.on("/0", handle_0); // ko vpišemo IP naslov, na koncu "/0", v brskalnik in pritisnemo Enter, tedaj strežnik na esp32 modulu dobi zahtevo "request" "/0" in izvede se funkcija handle_0(), ki smo jo definirali zgoraj

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

  if(LED1status == HIGH) { // če je spremenljivka LED1status enaka HIGH, tedaj prižgemo LED diodo
    digitalWrite(LED1pin, HIGH); // na pin 2 zapišemo visoko vrednost (LED dioda se prižge)
  } else { // sicer, če je spremenljivka LED1status enaka LOW, tedaj ugasnemo LED diodo
    digitalWrite(LED1pin, LOW); // na pin 2 zapišemo nizko vrednost (LED dioda se ugasne)
  }
}