#include <Arduino.h>

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

  // del, ki služi kot indikator, de je koda prenešena na modul
  pinMode(2, OUTPUT); // nožica št. 2 bo delovala kot izhod
  digitalWrite(2, HIGH); // na nožici 2 zapišemo visoko vrednost - LED zasveti
  delay(500); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
  digitalWrite(2, LOW); // na nožici 2 zapišemo nizko vrednost - LED ugasne
  delay(500); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
  digitalWrite(2, HIGH); // na nožici 2 zapišemo visoko vrednost - LED zasveti
  delay(500); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
  digitalWrite(2, LOW); // na nožici 2 zapišemo nizko vrednost - LED ugasne

}

void loop() {
  // put your main code here, to run repeatedly:
  // tu zapišemo kodo, ki se ponavljajoče izvaja

  vrednostPotenciometra = analogRead(nozicaPotenciometra); // preberemo vrednost s potenciometra in jo shranimo v spremenljivko
  Serial.println(vrednostPotenciometra); // izpišemo vrednost potenciometra na serijski monitor oz. v terminal

  delay(50); // na 50ms izpišemo vrednost potenciometra 

}