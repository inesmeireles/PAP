//teclado 
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

//Programa: Sistema de trava eletrica com RFID
#include <SPI.h>
#include <MFRC522.h>

#define Password_Length 5

//RFID
#define SS_PIN 53
#define RST_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

int signalPin = 13;

char Data[Password_Length];
char Master[Password_Length] = "1234";
byte data_count = 0, master_count = 0;
bool Pass_is_good;
char customKey;


const byte ROWS = 4;
const byte COLS = 3;

char hexaKeys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte colPins[COLS] = {5, 4, 3}; //connect col pins
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect row pins

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

LiquidCrystal_I2C lcd(0x20, 16, 2);  

void setup(){
  //teclado
  Serial.begin(9600);
  lcd.backlight();

  //rfid
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();
 
  pinMode(signalPin, OUTPUT);
}

void loop(){
  rfid();
  teclado();
}

void teclado()
{
  lcd.setCursor(3,0);
  lcd.print("Enter Password:");

  customKey = customKeypad.getKey();
  if (customKey){
    Serial.print ("Key pressed: ");
    Serial.println(customKey);
    Data[data_count] = customKey;
    lcd.setCursor(data_count,1);
    lcd.print(Data[data_count]);
    data_count++;
    }

  if(data_count == Password_Length-1){
    lcd.clear();

    if(!strcmp(Data, Master)){
      lcd.print("Correct");
      Serial.print("Correct");
      digitalWrite(signalPin, HIGH);
      delay(5000);
      digitalWrite(signalPin, LOW);
      }
    else{
      lcd.print("Incorrect");
      Serial.print("Incorrect");
      delay(1000);
      }
   
    lcd.clear();
    clearData();  
  }  
}
void rfid()
{
  // Procura por cartao RFID
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Seleciona o cartao RFID
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
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
 
  if (conteudo.substring(1) == "2B 82 22 1B") //UID 1 - Cartao
  {
    Serial.println("Ola FILIPEFLOP !");
    Serial.println();
    digitalWrite(13, HIGH); // ativa rele, abre a trava solenoide
    delay(3000);           // espera 3 segundos
    digitalWrite(13, LOW);  // desativa rele, fecha a trava solenoide
  }   
}
void clearData(){
  while(data_count !=0){
    Data[data_count--] = 0;
  }
  return;
}
