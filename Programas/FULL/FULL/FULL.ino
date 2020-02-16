
 
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
 
#define SS_PIN 53
#define RST_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
  
//rele
int signalPin = 13;

byte data_count = 0, master_count = 0;

char st[20];

void setup() 
{
  Serial.begin(9600);   // Inicia a serial
  SPI.begin();      // Inicia  SPI bus
  pinMode(signalPin, OUTPUT);
  mfrc522.PCD_Init();   // Inicia MFRC522
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();  
}
 
void loop() 
{
  rfid();
}

int rfid() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return 0;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return 0;
  }
  //Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();
  if (conteudo.substring(1) == "2B 82 22 1B") //UID 1 - Chaveiro
  {
    Serial.println("lock opened");
    digitalWrite(signalPin, HIGH);
    delay(1000);
    digitalWrite(signalPin, LOW);
    Serial.println();  
    loop();
  }
  
 
  if (conteudo.substring(1) == "27 9B 01 3C") //UID 2 - Cartao
  {
    Serial.println("lock opened");
    //digitalWrite(signalPin, HIGH);
    delay(1000);
    //digitalWrite(signalPin, LOW);
    Serial.println(); 
    loop();
  }  
}
