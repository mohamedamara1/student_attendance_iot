#include "filesystem_handlers.h"


// List all files in SPIFFS and return as JSON
void handleListFiles()
{
  DynamicJsonDocument doc(1024);
  JsonArray array = doc.to<JsonArray>();
  File root = SPIFFS.open("/");
  File file = root.openNextFile();
  while (file)
  {
    JsonObject obj = array.createNestedObject();

    obj["name"] = file.name();
    obj["size"] = file.size();

    file.close(); // close the current file before opening the next one
    file = root.openNextFile();
  }
  String jsonStr;
  serializeJson(doc, jsonStr);
  server.send(200, "application/json", jsonStr);
}

// Delete a file from SPIFFS
void handleDeleteFile()
{
  String filename = server.arg("filename");
  if (SPIFFS.remove(filename))
  {
    // Create JSON response
    StaticJsonDocument<100> jsonResponse;
    jsonResponse["message"] = "File deleted successfully";
    jsonResponse["filename"] = filename;

    String jsonStr;
    serializeJson(jsonResponse, jsonStr);
    server.send(200, "application/json", jsonStr);
  }
  else
  {
    server.send(400, "text/plain", "Failed to delete file");
  }
}