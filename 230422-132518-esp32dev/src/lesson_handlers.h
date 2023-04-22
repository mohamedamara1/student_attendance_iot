#ifndef USER_HANDLER_H
#define USER_HANDLER_H

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

void handleCreatelesson();
void handleReadlesson();
void handleUpdatelesson();
void handleDeletelesson();
void handleGetLessons();


#endif // USER_HANDLER_H
