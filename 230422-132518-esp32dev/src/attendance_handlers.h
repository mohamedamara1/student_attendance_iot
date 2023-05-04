#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <WebServer.h>
#include "credits.h"
#include "web_page.h"
#include <ArduinoJson.h>
#include <sMQTTBroker.h> // https://github.com/terrorsl/sMQTTBroker
#include <PubSubClient.h>
#include <string>

extern WebServer server;
extern sqlite3 *db;
extern PubSubClient mqttClient;

extern int db_exec(sqlite3 *, const char *);

void handleGetAttendances();
void handleCreateAttendance();
void handleDeleteAttendance();
void handleDeleteAllAttendances();
void handleUpdateAttendance();
void handleGetAttendancesByLessonId();
