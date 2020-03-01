//LCD
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//KEYPAD
#include <Keypad.h>
//RFID
#include <SPI.h>
#include <MFRC522.h>
//Finger print
#include <Adafruit_Fingerprint.h>

#define Password_Length 6 //são 5 carateres mas temos de carregar em # para ativar o keypad

byte data; //Inicialização da variavel para ler o array dos dados enviados por serie do WIFI (recebe dados)

static const char xyz[] PROGMEM = "This is a string stored in flash";

//Finger print config
// pin #10 is IN from sensor (Yellow wire)
// pin #11 is OUT from arduino  (WHITE wire)
SoftwareSerial mySerial(10, 11);// Tx, Rx

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

//rele
int signalPin = 13;

//keypad configuration
char Data[Password_Length];
char Master[Password_Length] = "12345";
byte data_count = 0, master_count = 0;
bool Pass_is_good;
char customKey;
boolean key_pad = true;

const byte ROWS = 4;
const byte COLS = 3;

char hexaKeys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte colPins[COLS] = {5, 4, 3}; //pinos das colunas conectados na placa
byte rowPins[ROWS] = {9, 8, 7, 6}; //pinos das linhas conectados na placa

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

//display
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//rfid configuration
boolean RFIDMode = true; // boolean to change modes
#define SS_PIN 53
#define RST_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

//QR Code
boolean qrcode = true;

void setup(){
  Serial.begin(115200); //frequencia de trabalho dos componentes do projeto /keypad, rfid, lcd, monitor, etc.
  Serial1.begin(115200); //frequencia de transmissão entre o wifi modulo e o arduino

  while (!Serial); //loop para carregar todas as comunicações serie
  delay(100);  

  pinMode(signalPin, OUTPUT);
  //Protocol Configuration
  lcd.init();
  lcd.backlight();  
  //RFID
  SPI.begin();          // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  digitalWrite(signalPin, LOW);

  // set the data rate for the sensor serial port
  finger.begin(57600); //frequencia especifica para transmitir dados com a impresssao digital 
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor");
  } else {
    Serial.println("Did not find fingerprint sensor");
    while (1) { delay(1); }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" template(s)");
  Serial.println("Waiting for valid finger..."); 
}

int fingerprint_function() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)
    return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) {
    Serial.println("Try again...");
    lcd.clear();
    lcd.print("Leitura invalida,");
    lcd.setCursor(0,1);
    lcd.print("tente de novo...");  
    delay(2000);    
    return -1;
  }
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  digitalWrite(signalPin, HIGH);
  lcd.clear();
  lcd.print("Bem vindo, chefe");
  lcd.setCursor(0,1);
  lcd.print("boa visita...");  
  delay(5000);                        //Change the door lock delay from here
  digitalWrite(signalPin, LOW);
  Serial.println("Unlocked");
  return finger.fingerID;   
}

void qrcode_function(){
  int i=0;
  String str;

  if (Serial.available() > 0) 
  {
    byte index = 0; // Index into array; where to store the character     
    char inChar = Serial.read();
    if(index < 19) // One less than the size of the array
    {
        str = Serial.readStringUntil('\n');
        i = Serial.parseInt();
    }
  }
  //Serial.print(i);
  if (i == 9)
  {
    lcd.clear();
    lcd.print("Bem vindo,");
    lcd.setCursor(0,1);
    lcd.print("boa estadia...");      
    Serial.println("lock opened");
    Serial.println();
    digitalWrite(signalPin, HIGH);
    delay(5000);
    digitalWrite(signalPin, LOW);
  }
  if (i == 7)
  {
    lcd.clear();
    lcd.print("QR Code invalido,");
    lcd.setCursor(0,1);
    lcd.print("Verifique reserva...");      
    Serial.println("QR Code invalido");
    Serial.println();
    delay(5000);
  }  
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
    lcd.clear();
    lcd.print("Ola Patricia,");
    lcd.setCursor(0,1);
    lcd.print("bom trabalho...");   
    Serial.write("3");
    delay(5000);
    digitalWrite(signalPin, LOW);
  }
  
 
  if (conteudo.substring(1) == "27 9B 01 3C") //UID 2 - Cartao
  {
    Serial.println("lock opened");
    Serial.println();
    digitalWrite(signalPin, HIGH);
    lcd.clear();
    lcd.print("Ola Marta,");
    lcd.setCursor(0,1);
    lcd.print("bom trabalho...");    
    Serial.write("4"); 
    delay(5000);
    digitalWrite(signalPin, LOW);
  }  
}

void clearData(){
  while(data_count !=0){
    Data[data_count--] = 0;
  }
  return;
}

void keypad_function(){
  lcd.setCursor(3,0);
  lcd.clear();
  lcd.print("Introduza codigo:");

  //Envia codigo para Modulo WIFI
  Serial.write("#"); //envia inicio de codigo para o modulo wifi para validar na base de dados
  while(data_count != Password_Length-1){
    customKey = customKeypad.getKey();
    if (customKey){
      Serial.print ("Key pressed: ");
      Serial.println(customKey);
      Serial.write(customKey); //envia numero para o WIFI
      Data[data_count] = customKey;
      lcd.setCursor(data_count,1);
      lcd.print(Data[data_count]);
      data_count++;
      }
  
    if(data_count == Password_Length-1){
      lcd.clear();
  
      if(!strcmp(Data, Master)){        
        lcd.print("Bem vindo,");
        lcd.setCursor(0,1);
        lcd.print("boa estadia...");
        Serial.print("Correct");
        digitalWrite(signalPin, HIGH);
        delay(5000);
        digitalWrite(signalPin, LOW);
        }
      else{
        lcd.print("Codigo invalido!!!");
        Serial.print("Incorrect");
        delay(1000);
        }

      SPI.begin();          // Inicia  SPI bus
      mfrc522.PCD_Init();   // Inicia MFRC522 
      RFIDMode = true; //altera o modo
      lcd.clear();
      clearData(); 
      return; 
    }  
  }  
}

void key_pad_ini() {
  customKey = customKeypad.getKey();
  if (customKey){ //verifica se foi carregada uma tecla qualquer
    keypad_function();
  }
  SPI.begin();          // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522    
  key_pad = false; //altera o modo   
  RFIDMode = true;   
}

void loop(){
  lcd.clear();
  lcd.print("Alojamento local!!!");
  if (key_pad == true) {
    key_pad_ini();
  }  
  if (qrcode == true) {
    key_pad_ini();
    qrcode_function();
    qrcode = false;
    key_pad = true;
    key_pad_ini();
  }
  if (RFIDMode == true) {
    key_pad_ini();
    rfid_function();
    RFIDMode = false; //altera o modo
    qrcode=true;
    key_pad=true;
    key_pad_ini();
  }  
  key_pad_ini();
  fingerprint_function();
  key_pad_ini();
}
