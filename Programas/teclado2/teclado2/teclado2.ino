//LCD
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//KEYPAD
#include <Keypad.h>
//RFID
#include <SPI.h>
#include <MFRC522.h>

#define Password_Length 5

//rele
int signalPin = 13;

//keypad configuration
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

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//rfid configuration
#define SS_PIN 53
#define RST_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

void setup(){
  Serial.begin(9600);
  pinMode(signalPin, OUTPUT);
  //Protocol Configuration
  lcd.init();
  lcd.backlight();  
  //RFID
  SPI.begin();          // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
}

void rfid_function(){
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
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
  if (conteudo.substring(1) == "2B 82 22 1B") //UID 1 - Chaveiro
  {
    Serial.println("lock opened");
    Serial.println();
    digitalWrite(signalPin, HIGH);
    delay(5000);
    digitalWrite(signalPin, LOW);
  }
  
 
  if (conteudo.substring(1) == "27 9B 01 3C") //UID 2 - Cartao
  {
    Serial.println("lock opened");
    Serial.println();
    digitalWrite(signalPin, HIGH);
    delay(5000);
    digitalWrite(signalPin, LOW);
  }  
}

void keypad_function(){
  lcd.setCursor(3,0);
  lcd.print("Enter Password:");

  while(data_count != Password_Length-1){
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
}

void loop(){
  rfid_function();
  customKey = customKeypad.getKey();
  //if (customKey){
  //  keypad_function();
  //}    
}

void clearData(){
  while(data_count !=0){
    Data[data_count--] = 0;
  }
  return;
}
