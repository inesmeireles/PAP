#include <ESP8266WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <stdlib.h>     /* atol */


byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress server_addr(192, 168, 1, 73); // IP of the MySQL *server* here
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
}
