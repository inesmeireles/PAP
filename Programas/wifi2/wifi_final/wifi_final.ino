#include <ESP8266WiFi.h>

byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

void setup()
{
  Serial.begin(115200);
  Serial.println();

  //WiFi.begin("Casa_Santa_Teresinha", "teresinha"); //Seixas
  WiFi.begin("Vodafone-DE3AFC", "95J4VrbAUA"); //Codessos

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial1.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}


void loop() {

}
