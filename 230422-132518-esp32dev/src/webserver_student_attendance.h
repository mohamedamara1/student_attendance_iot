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

#include <sMQTTBroker.h>  // https://github.com/terrorsl/sMQTTBroker
#include <PubSubClient.h> // https://github.com/knolleary/pubsubclient



#include "user_handlers.h"
#include "attendance_handlers.h"
#include "lesson_handlers.h"
#include "class_handlers.h"
#include "classroom_handlers.h"
#include "course_handlers.h"
#include "student_handlers.h"
#include "teacher_handlers.h"
#include "filesystem_handlers.h"



int openDb(const char *, sqlite3 **);
int db_exec(sqlite3 *, const char *);
