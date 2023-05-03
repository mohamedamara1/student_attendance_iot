

#include "webserver_student_attendance.h"

#define mqttPort 1883
#define mqttServer "127.0.0.1"

WebServer server(80);
sqlite3 *db;
sMQTTBroker broker;
WiFiClient espClient;
PubSubClient mqttClient(espClient);

const char *data = "Callback function called";
char *zErrMsg = 0;
static int callback(void *data, int argc, char **argv, char **azColName);

bool mountSPIFFS();
void handleNotFound();

String web_content = "";

TaskHandle_t brokerTask;

void brokerTaskCode(void *pvParameters)
{
  Serial.print("broker running on core ");
  Serial.println(xPortGetCoreID());

  for (;;)
  {
    delay(1000);
    broker.update();
  }
}

void sendCrossOriginHeader()
{
  Serial.println(F("sendCORSHeader"));
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  server.sendHeader("Access-Control-Allow-Methods", "GET,POST,PUT,DELETE,OPTIONS");
}
void handleOptions()
{
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.send(204);
}
void setup()
{
  Serial.begin(115200);
  Serial.print("\n Connecting to Wifi");

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(250);
    Serial.print(".");
  }

  Serial.print("\n Connected. IP adress: ");
  Serial.println(WiFi.localIP());

  // users
  server.on("/users", HTTP_GET, handleGetUsers);
  server.on("/users", HTTP_POST, handleCreateUser);
  server.on("/users", HTTP_PUT, handleUpdateUser);
  server.on("/users", HTTP_DELETE, handleDeleteUser);
  server.on("/users/findById", HTTP_GET, handleGetUser);

  // students
  server.on("/students", HTTP_GET, handleGetStudents);
  server.on("/students", HTTP_POST, handleCreateStudent);
  server.on("/students", HTTP_PUT, handleUpdateStudent);
  server.on("/students", HTTP_DELETE, handleDeleteStudent);
  server.on("/students/findById", HTTP_GET, handleGetStudentById);
  server.on("/students/findByUserId", HTTP_GET, handleGetStudentByUserId);
  server.on("/students/findByUserId", HTTP_OPTIONS, handleOptions); // Add this line to handle OPTIONS requests

  // teachers
  server.on("/teachers", HTTP_GET, handleGetTeachers);
  server.on("/teachers", HTTP_POST, handleCreateTeacher);
  server.on("/teachers", HTTP_PUT, handleUpdateTeacher);
  server.on("/teachers", HTTP_DELETE, handleDeleteTeacher);
  // server.on("/teachers", HTTP_GET, handleGetTeacher);

  // attendances
  server.on("/attendances", HTTP_GET, handleGetAttendances);
  server.on("/attendances", HTTP_DELETE, handleDeleteAttendance);
  server.on("/attendances", HTTP_PUT, handleUpdateAttendance);
  // server.on("/attendances", HTTP_CREATE, handleGetAttendances);
  server.on("/attendances/markPresence", HTTP_POST, handleCreateAttendance);
  server.on("/attendances/markPresence", HTTP_OPTIONS, handleOptions); // Add this line to handle OPTIONS requests

  // classes
  server.on("/classes", HTTP_GET, handleGetClasses);
  server.on("/classes", HTTP_POST, handleCreateClass);
  server.on("/classes", HTTP_DELETE, handleDeleteClass);

  // classrooms
  server.on("/classrooms", HTTP_POST, handleCreateClassroom);
  server.on("/classrooms", HTTP_GET, handleGetClassrooms);
  server.on("/classrooms", HTTP_DELETE, handleDeleteClassroom);

  // courses
  server.on("/courses", HTTP_POST, handleCreateCourse);
  server.on("/courses", HTTP_GET, handleGetCourses);
  server.on("/courses", HTTP_DELETE, handleDeleteCourse);

  // lessons
  server.on("/lessons", HTTP_GET, handleGetLessons);
  server.on("/lesson/newestLesson", HTTP_GET, handleGetNewestLesson);
  server.on("/lessons", HTTP_POST, handleCreateLesson);
  server.on("/lessons", HTTP_PUT, handleUpdateLesson);
  server.on("/lessons", HTTP_DELETE, handleDeleteLesson);

  // filesystem
  server.on("/filesystem/listFiles", HTTP_GET, handleListFiles);
  server.on("/filesystem/deleteFile", HTTP_DELETE, handleDeleteFile);

  server.on("/", HTTP_GET, []()
            {
    File indexFile = SPIFFS.open("/index.html", "r");
    if (!indexFile) {
      server.send(404, "text/plain", "File not found");
      return;
    }
    server.streamFile(indexFile, "text/html");
    indexFile.close(); });

  mountSPIFFS();
  Serial.print("Totall kilobytes: ");
  Serial.println(SPIFFS.totalBytes() / 1024.0);

  Serial.print("Used kilobytes: ");
  Serial.println(SPIFFS.usedBytes() / 1024.0);

  Serial.print("Left kilobytes: ");
  Serial.println(SPIFFS.totalBytes() / 1024.0 - SPIFFS.usedBytes() / 1024.0);

  uint32_t flash_size = ESP.getFlashChipSize();

  Serial.print("Flash size: ");
  Serial.print(flash_size);
  Serial.println(" bytes");

  File rootOne = SPIFFS.open("/");
  File file = rootOne.openNextFile();

  while (file)
  {
    String fileName = file.path();
    if (fileName.endsWith(".html") || fileName.endsWith(".css") || fileName.endsWith(".js"))
    {
      String contentType = "text/plain";
      if (fileName.endsWith(".html"))
      {
        contentType = "text/html";
      }
      else if (fileName.endsWith(".css"))
      {
        contentType = "text/css";
      }
      else if (fileName.endsWith(".js"))
      {
        contentType = "application/javascript";
      }

      // Check if file is in the 'assets' subdirectory
      if (fileName.startsWith("/assets/"))
      {
        Serial.println(fileName + " assets");
        server.serveStatic(fileName.c_str(), SPIFFS, fileName.c_str());
      }
      else if (fileName == "/index.html")
      {
        server.serveStatic(fileName.c_str(), SPIFFS, fileName.c_str());
      }
    }
    file = rootOne.openNextFile();
  }
  server.enableCORS(); // This is the magic
  server.begin();
  int rc;

  SPI.begin();
  sqlite3_initialize();
  Serial.begin(115200);

  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  listAllFiles();

  // here we initiate the database connection

  if (openDb("/spiffs/attendance_db.db", &db) != SQLITE_OK)
  {
    Serial.println("Failed to open database");
    // Handle the error
  }

  /*  create task for broker that will be executed on core 0  */
  xTaskCreatePinnedToCore(
      brokerTaskCode, /* Task function. */
      "brokerTask",   /* name of task. */
      10000,          /* Stack size of task */
      NULL,           /* parameter of the task */
      1,              /* priority of the task */
      &brokerTask,    /* Task handle to keep track of created task */
      0);             /* pin task to core 0 */
  delay(500);
  broker.init(mqttPort);

  mqttClient.setServer(mqttServer, mqttPort);
  unsigned long start = millis();
  while (!mqttClient.connected())
  {
    Serial.println("Connecting to MQTT broker...");
    if (mqttClient.connect("ESP32"))
    {
      Serial.println("Connected to MQTT broker");
    }
    else
    {
      Serial.print("Failed to connect to MQTT broker, rc=");
      Serial.println(mqttClient.state());
      delay(1000);
    }
    // Break out of the loop after 10 seconds
    if (millis() - start > 10000)
    {
      Serial.println("Connection timed out");
      break;
    }
  }
  // Initialize the database if it's empty
  // initializeDatabase();
}

void loop()
{
  server.handleClient();
}

/*--------------------------------------------------------*/
int openDb(const char *filename, sqlite3 **db)
{
  int rc = sqlite3_open(filename, db);
  if (rc)
  {
    Serial.printf("Can't open database: %s\n", sqlite3_errmsg(*db));
    return rc;
  }
  else
  {
    Serial.printf("Opened database successfully\n");
  }
  return rc;
}
/*--------------------------------------------------------*/

int db_exec(sqlite3 *db, const char *sql)
{
  Serial.println('\n');
  int rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);
  if (rc != SQLITE_OK)
  {
    Serial.printf("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }
  return rc;
}
/*--------------------------------------------------------*/

static int callback(void *data, int argc, char **argv, char **azColName)
{
  web_content += "<tr>";
  for (int i = 1; i < argc; i++)
  {
    web_content += "<td>";
    web_content += argv[i];
    web_content += "</td>";
  }
  web_content += "<td><button onClick =\"del('";
  web_content += argv[1];
  web_content += "')\">Delete</button></td></tr>";
  return 0;
}
void listAllFiles()
{
  Serial.println("Printing files in SPIFFS:");

  File root = SPIFFS.open("/");

  File file = root.openNextFile();

  while (file)
  {

    Serial.print("FILE: ");
    Serial.println(file.name());

    file = root.openNextFile();
  }
}
bool mountSPIFFS()
{
  if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount SPIFFS file system");
    return false;
  }
  Serial.println("SPIFFS file system mounted");
  return true;
}

/*--------------------------------------------------------*/

void handleNotFound()
{

  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
