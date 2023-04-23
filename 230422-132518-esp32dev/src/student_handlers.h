#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <WebServer.h>
#include "credits.h"
#include "web_page.h"
#include <ArduinoJson.h>

extern WebServer server;
extern sqlite3 *db;

extern int db_exec(sqlite3 *, const char *);

void handleGetStudents();
void handleCreateStudent();
void handleDeleteStudent();
void handleGetStudent();
void handleUpdateStudent();