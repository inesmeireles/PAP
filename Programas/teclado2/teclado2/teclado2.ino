#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

#define Password_Length 5

int signalPin = 12;

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
  Serial.begin(9600);
  lcd.backlight();
  pinMode(signalPin, OUTPUT);
}

void loop(){

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

void clearData(){
  while(data_count !=0){
    Data[data_count--] = 0;
  }
  return;
}
