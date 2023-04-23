#include <stdlib.h>
#include <sqlite3.h>
#include <WebServer.h>
#include "credits.h"
#include "web_page.h"
#include <ArduinoJson.h>

extern WebServer server;
extern sqlite3 *db;

extern int db_exec(sqlite3 *, const char *);

void handleGetTeachers();
void handleCreateTeacher();
void handleDeleteTeacher();
void handleGetTeacher();
void handleUpdateTeacher();