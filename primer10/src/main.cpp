#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

const char* ssid = "kresilnik";
const char* password = "bled2024";

// ustvarimo objekt razreda WebServer, port 80 je prednastavljeni port za HTTP strežnike
WebServer server(80); // objekt server razreda WebServer (http port 80)

// na vratih 81 ustvarimo strežnik spletnih vtičnikov - WebSocketsServer, ki zagotavlja permanetno dvosmerno komunikacijo med klientom (npr. brskalnikom Chrome) in strežnikom na esp32 modulu, kar omogoča, da lahko strežnik pošilja podatke klientu tudi brez zahteve ("request") od klienta
WebSocketsServer webSocket = WebSocketsServer(81); // objekt webSocket razreda WebSocketsServer (websocket port 81)

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
    <h2>Primer s spletnim vtičnikom WebSocket.</h2>
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

// ***************************************************************************************************
// definicija funkcije onWebSocketEvent, ki se izvede kadarkoli dobimo sporočilo preko spletnega
// vtičnika, t.j. preko WebSocket-a (oz. se dogodi dogodek - "event" na spletnem vtičniku WebSocket-u)
// ***************************************************************************************************
void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  // določimo funkcionalnost glede na tip dogodka na spletnem vtičniku (WebSocket-u)
  switch(type) {

    case WStype_CONNECTED: // če je tip dogodka "WStype_CONNECTED", tedaj se je klient (npr. brskalnik Chrome) povezal na spletni vtičnik WebSocket na esp32 modulu
      {
        IPAddress ip = webSocket.remoteIP(num); // dobimo IP naslov klienta, ki se je povezal
        Serial.printf("[%u] Klient povezan z IP: %d.%d.%d.%d\n", num, ip[0], ip[1], ip[2], ip[3]); // izpišemo sporočilo na serijski monitor, da vidimo, da se je klient povezal in njegov IP naslov
      }
      break;    

    case WStype_DISCONNECTED: // če je tip dogodka "WStype_DISCONNECTED", tedaj se je klient (npr. brskalnik Chrome) odklopil od spletnega vtičnika WebSocket na esp32 modulu
      {
        Serial.printf("[%u] Klient odklopljen!\n", num); // izpišemo sporočilo na serijski monitor, da vidimo, da se je klient odklopil
      }
      break;

    case WStype_TEXT: // če je tip dogodka "WStype_TEXT", tedaj smo preko spletnega vtičnika WebSocket na esp32 modulu prejeli besedilno sporočilo od klienta (npr. brskalnika Chrome)
      {
        Serial.printf("[%u] Prejeto besedilno sporočilo: %s\n", num, payload); // izpišemo sporočilo na serijski monitor, da vidimo, da smo prejeli besedilno sporočilo in kakšno je njegovo vsebino
        String besedilo = String((char) payload[0]); // prvi znak prejetega sporočila spravimo v spremenljivko besedilo, ki je tipa String

        Serial.println("Prejeti prvi znak sporočila je: " + besedilo);

        if (besedilo == "1") { // če je prejeti prvi znak sporočila "1", tedaj vklopimo LED diodo
          digitalWrite(LED1pin, HIGH); // na pin 2 zapišemo visoko vrednost (LED dioda se prižge)
        }
        else if (besedilo == "0") {
          digitalWrite(LED1pin, LOW); // na pin 2 zapišemo nizko vrednost (LED dioda se ugasne)
        }  
    }

      default:
        break;

    } // KONEC switch stavka
  } // KONEC funkcije onWebSocketEvent


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

  server.begin(); // zaženemo strežnik na esp32 modulu, ki posluša na portu 80 in čaka na zahteve ("requests") od klientov (npr. brskalnika Chrome)
  Serial.println("HTTP strežnik je zagnan."); // izpišemo sporočilo, da je strežnik zagnan

  // poženemo WebSocket strežnik in določimo funkcijo onWebSocketEvent, ki se izvede kadarkoli dobimo sporočilo preko spletnega vtičnika WebSocket-a
  webSocket.begin(); // zaženemo WebSocket spletni strežnik, ki posluša na portu 81
  Serial.println("WebSocket strežnik je zagnan."); // izpišemo sporočilo, da je WebSocket strežnik zagnan
  webSocket.onEvent(onWebSocketEvent); // kadarkoli se bo dogodil določen dogodek preko spletnega vtičnika WebSocket-a, bomo izvedli funkcijo onWebSocketEvent, ki je definirano zgoraj

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

  webSocket.loop(); // poslušamo na spletnem vtičniku 81 (WebSocket-u) za podatke in dogodke
}