#include <stdio.h>
#include <stdlib.h>
#include <WebServer.h>
#include "credits.h"
#include "web_page.h"
#include <ArduinoJson.h>
#include <FS.h>
#include "SPIFFS.h"
#include <Base64.h>

extern WebServer server;

void handleListFiles();
void handleDeleteFile();
