#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <SPI.h>
#include <FS.h>
#include "SPIFFS.h"

#include <WiFi.h>
#include <WebServer.h>
#include "credits.h"
#include "web_page.h"
#include <ArduinoJson.h>
#include "user_handlers.h"


int openDb(const char *, sqlite3 **);
int db_exec(sqlite3 *, const char *);
