

#include "webserver_student_attendance.h"

WebServer server(80);
sqlite3 *db;


const char* data = "Callback function called";
char *zErrMsg = 0;
static int callback(void *data, int argc, char **argv, char **azColName);


void handleRoot();
void handleNotFound();
void insertRecord();
void deleteRecord();
void showRecords();
void newRecordTable();

String web_content = "";


void setup() {
  Serial.begin(115200);
  Serial.print("\n Connecting to Wifi");
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }

  Serial.print("\n Connected. IP adress: ");
  Serial.println(WiFi.localIP());

  
  //users
  server.on("/users", HTTP_GET, handleGetUsers);
  server.on("/users", HTTP_POST, handleCreateUser);
  //server.on("/users/:id", HTTP_GET, handleGetUser);
  //server.on("/users/:id", HTTP_PUT, handleUpdateUser);
  server.on("/users", HTTP_DELETE, handleDeleteUser);
  //attendances
  //server.on("/attendances", HTTP_GET, handleGetAttendances);
 // server.on("/attendances", HTTP_DELETE, handleGetAttendances);
  server.on("/attendances", HTTP_PUT, handleUpdateAttendance);
  //server.on("/attendances", HTTP_CREATE, handleGetAttendances);
  //classes
  server.on("/classes", HTTP_GET, handleGetClasses);
  server.on("/classes", HTTP_POST, handleCreateClass);
  server.on("/classes", HTTP_DELETE, handleDeleteClass);

  //classrooms
  server.on("/classrooms", HTTP_POST, handleCreateClassroom);
  server.on("/classrooms", HTTP_GET, handleGetClassrooms);
  server.on("/classrooms", HTTP_DELETE, handleDeleteClassroom);
  //courses
  server.on("/courses", HTTP_POST, handleCreateCourse);
  server.on("/courses", HTTP_GET, handleGetCourses);
  server.on("/courses", HTTP_DELETE, handleDeleteCourse);
  //lessons
  server.on("/lessons", HTTP_GET, handleGetLessons);


  server.begin();
  int rc;

  SPI.begin();
  sqlite3_initialize();
  Serial.print("Trying to print data:\n ");
  Serial.begin(115200);

 // this code lists the files present in the data folder in the SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  File root = SPIFFS.open("/");
  File file = root.openNextFile();
 
  while(file){
      Serial.print("FILE: ");
      Serial.println(file.name());
      file = root.openNextFile();
  }

  // here we initiate the database connection
  if (openDb("/spiffs/attendance_system.db", &db))
      return;
}

void loop() {
  server.handleClient();
}

/*--------------------------------------------------------*/
int openDb(const char *filename, sqlite3 **db) {
   int rc = sqlite3_open(filename, db);
   if (rc) {
       Serial.printf("Can't open database: %s\n", sqlite3_errmsg(*db));
       return rc;
   } else {
       Serial.printf("Opened database successfully\n");
   }
   return rc;
}
/*--------------------------------------------------------*/

int db_exec(sqlite3 *db, const char *sql) {
   Serial.println('\n');
   int rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   if (rc != SQLITE_OK) {
       Serial.printf("SQL error: %s\n", zErrMsg);
       sqlite3_free(zErrMsg);
   }
   return rc;
}
/*--------------------------------------------------------*/

static int callback(void *data, int argc, char **argv, char **azColName){
  web_content += "<tr>";
  for (int i = 1; i<argc; i++){
    web_content +="<td>";
    web_content += argv[i];
    web_content +="</td>";
  }
  web_content +="<td><button onClick =\"del('";
  web_content += argv[1];
  web_content += "')\">Delete</button></td></tr>"; 
  return 0;
}

/*--------------------------------------------------------*/
void handleRoot() {

  server.send(200, "text/html", web_page);

}
/*--------------------------------------------------------*/

void handleNotFound() {

  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
      message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send ( 404, "text/plain", message );

}





/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
