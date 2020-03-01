#include <ESP8266WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <stdlib.h>     /* atol */


byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress server_addr(192, 168, 1, 90); // IP of the MySQL *server* here
char user[] = "arduino";              // MySQL user login username
char password[] = "12345678";        // MySQL user login password
int pin = 2;

char query[] = "select CONVERT(CASE WHEN a.pin is null THEN 0 ELSE 1 END ,UNSIGNED INTEGER) AS pin FROM  arduino.customer_login b left join arduino.registos a on a.pin = b.pin AND b.date_in between a.start_date and a.end_date WHERE b.new = 1 LIMIT 1";
char queryUPD[] = "UPDATE arduino.customer_login SET new = 0 WHERE new = 1";

WiFiClient client;
MySQL_Connection conn((Client *)&client);

int k = 0;
void setup()
{
  Serial.begin(115200);
  Serial.println();

  WiFi.begin("Casa_Santa_Teresinha", "teresinha"); //Seixas
  //WiFi.begin("Vodafone-DE3AFC", "95J4VrbAUA"); //Codessos

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  delay(10000);
  if (conn.connect(server_addr, 3306, user, password)) {
    delay(1000);
    Serial.println("MySQL Connected.");
  }
  else
    Serial.println("Connection failed.");
}


void loop() {
  // Initiate pin variable
  pin = 2;
  // Initiate the query class instance
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  // Execute the query
  cur_mem->execute(query);
  // Fetch the columns and print them
  column_names *cols = cur_mem->get_columns();
  Serial.println();
  // Read mysql value
  row_values *row = NULL;
  do {
    row = cur_mem->get_next_row();
    if (row != NULL) {
      Serial.println(row->values[0]);
      pin = atol(row->values[0]);
      Serial.println(pin);
      if (pin == 1) {
        Serial.write("9");
      }
      else
        Serial.write("7");
    }
  } while (row != NULL);

  // Deleting the cursor also frees up memory used
  delete cur_mem;

  //Update MySQL QRcode login
  if (pin != 2) {
    // Initiate the query class instance
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    // Update
    cur_mem->execute(queryUPD);
    delete cur_mem;
  }

  empregadas();
  //clientes();
}

void clientes() {
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
}

void empregadas() {
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
  if (i == 3) {
    char emp[] = "INSERT INTO arduino.user_login VALUES ('2B 82 22 1B',CURRENT_TIMESTAMP,'Patricia')";
    // Initiate the query class instance
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    // Update
    cur_mem->execute(emp);
    delete cur_mem;    
  }
  //Serial.print(i);
  if (i == 4) {
    char emp[] = "INSERT INTO arduino.user_login VALUES ('27 9B 01 3C',CURRENT_TIMESTAMP,'Marta')";
    // Initiate the query class instance
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    // Update
    cur_mem->execute(emp);
    delete cur_mem;    
  }  
}
