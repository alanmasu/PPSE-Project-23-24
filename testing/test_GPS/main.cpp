#include <Arduino.h>
#include <Wire.h>

// put function declarations here:
void setup() {
  // put your setup code here, to run once:

  pinMode(20, OUTPUT);

    
}

void loop() {
  digitalWrite(20, HIGH);
     
  // Attendre 1 seconde, pendant ce temps la LED reste allumee
  delay(1000);
     
  // Passer le sortie à l'état BAS pour eteindre la LED
  digitalWrite(20, LOW);    
   
  // Attendre 1 seconde, pendant ce temps la LED reste donc éteinte
  delay(1000);
}
