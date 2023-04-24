

#include "webserver_student_attendance.h"

WebServer server(80);
sqlite3 *db;


const char* data = "Callback function called";
char *zErrMsg = 0;
static int callback(void *data, int argc, char **argv, char **azColName);


bool mountSPIFFS();
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
  server.on("/users", HTTP_PUT, handleUpdateUser);
  server.on("/users", HTTP_DELETE, handleDeleteUser);
  //server.on("/users", HTTP_GET, handleGetUser);

  //students
  server.on("/students", HTTP_GET, handleGetStudents);
  server.on("/students", HTTP_POST, handleCreateStudent);
  server.on("/students", HTTP_PUT, handleUpdateStudent);
  server.on("/students", HTTP_DELETE, handleDeleteStudent);
  //server.on("/students", HTTP_GET, handleGetStudent);

  //teachers
  server.on("/teachers", HTTP_GET, handleGetTeachers);
  server.on("/teachers", HTTP_POST, handleCreateTeacher);
  server.on("/teachers", HTTP_PUT, handleUpdateTeacher);
  server.on("/teachers", HTTP_DELETE, handleDeleteTeacher);
  //server.on("/teachers", HTTP_GET, handleGetTeacher);

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

  server.on("/filesystem/listFiles", HTTP_GET, handleListFiles);
  server.on("/filesystem/deleteFile", HTTP_DELETE, handleDeleteFile);

  //server.on("/", handleRoot);

  server.on("/", HTTP_GET, [](){
    File indexFile = SPIFFS.open("/index.html", "r");
    if (!indexFile) {
      server.send(404, "text/plain", "File not found");
      return;
    }
    server.streamFile(indexFile, "text/html");
    indexFile.close();
  });
  
  mountSPIFFS();

  File rootOne = SPIFFS.open("/");
  File file = rootOne.openNextFile();

  while(file){
      String fileName = file.path();
      if (fileName.endsWith(".html") || fileName.endsWith(".css") || fileName.endsWith(".js")) {
          Serial.println(fileName +  " preparing to serve");
          String contentType = "text/plain";
          if (fileName.endsWith(".html")) {
              contentType = "text/html";
          } else if (fileName.endsWith(".css")) {
              contentType = "text/css";
          } else if (fileName.endsWith(".js")) {
              contentType = "application/javascript";
          }

          // Check if file is in the 'assets' subdirectory
          if (fileName.startsWith("/assets/")) {
              Serial.println(fileName + "assets");
              server.serveStatic(fileName.c_str(), SPIFFS, fileName.c_str());
          } else if (fileName == "/index.html") {
              Serial.println(fileName + "html");
              server.serveStatic(fileName.c_str(), SPIFFS, fileName.c_str());
          }
      }

      file = rootOne.openNextFile();
  }
  

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
  File fileTwo = root.openNextFile();
 
  while(fileTwo){
      Serial.print("FILE: ");
      Serial.println(fileTwo.path());

      fileTwo = root.openNextFile();
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

bool mountSPIFFS() {
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount SPIFFS file system");
    return false;
  }
  Serial.println("SPIFFS file system mounted");
  return true;
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
