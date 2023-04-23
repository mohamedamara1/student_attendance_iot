#include "teacher_handlers.h"

/*******************    TEACHERS    *************************/

// Get all teachers
void handleGetTeachers()
{
  DynamicJsonDocument doc(1024);
  JsonArray array = doc.to<JsonArray>();
  String sql = "SELECT * FROM teacher";
  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
  {
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        JsonObject obj = array.createNestedObject();
        obj["id"] = sqlite3_column_int(stmt, 0);
        obj["name"] = sqlite3_column_text(stmt, 1);
        obj["userId"] = sqlite3_column_int(stmt, 2);
    }
    String jsonStr;
    serializeJson(doc, jsonStr);
    server.send(200, "application/json", jsonStr);
  }
  else
  {
    server.send(400, "text/html", "FAIL");
  }
}

// Create a new teacher
void handleCreateTeacher()
{
  String sql = "";
  StaticJsonDocument<250> jsonDocument;
  if (server.hasArg("plain") == false)
  {
  }
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);
  // String id = jsonDocument["id"];

  String name = jsonDocument["name"];
  int userId = jsonDocument["userId"];
  sql = "insert into teacher(name,user_id) values('" + name + "','" + userId + "')";
  Serial.println(sql);
  if (db_exec(db, sql.c_str()) == SQLITE_OK)
  {
    // Get the id of the newly created teacher
    int id = sqlite3_last_insert_rowid(db);

    // Create JSON response
    StaticJsonDocument<100> jsonResponse;
    jsonResponse["message"] = "Teacher created successfully";
    jsonResponse["id"] = id;

    String jsonStr;
    serializeJson(jsonResponse, jsonStr);
    server.send(201, "application/json", jsonStr);
  }
  else
  {
    server.send(400, "text/plain", "Failed to create teacher");
  }
}

void handleDeleteTeacher()
{
  String sql = "";
  int id = server.arg("id").toInt();
  // int id = server.pathArg(0).toInt();
  Serial.println("delete");

  sql = "delete from teacher where id=" + String(id);
  Serial.println(sql);
  if (db_exec(db, sql.c_str()) == SQLITE_OK)
  {
    // Create JSON response
    StaticJsonDocument<100> jsonResponse;
    jsonResponse["message"] = "Teacher deleted successfully";
    jsonResponse["id"] = id;

    String jsonStr;
    serializeJson(jsonResponse, jsonStr);
    server.send(200, "application/json", jsonStr);
  }
  else
  {
    server.send(400, "text/plain", "Failed to delete teacher");
  }
}

void handleGetTeacher()
{
  String sql = "";
  int id = server.arg("id").toInt();
  // int id = server.pathArg(0).toInt();

  sql = "select * from teacher where id=" + String(id);
  sqlite3_stmt *stmt;

  Serial.println(sql);
  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
  {
    // Create JSON response
    StaticJsonDocument<100> jsonResponse;
    jsonResponse["id"] = id;
    jsonResponse["name"] = sqlite3_column_text(stmt, 1);
    jsonResponse["userId"] = sqlite3_column_int(stmt, 2);

    String jsonStr;
    serializeJson(jsonResponse, jsonStr);
    server.send(200, "application/json", jsonStr);
  }
  else
  {
    server.send(400, "text/plain", "Failed to get teacher");
  }
}

void handleUpdateTeacher()
{
  String sql = "";
  int id = server.arg("id").toInt();

  // Parse JSON request body
  StaticJsonDocument<250> jsonDocument;
  if (server.hasArg("plain") == false)
  {
    server.send(400, "text/plain", "Missing request body");
    return;
  }
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);

  // Retrieve fields from JSON request body
  String name = jsonDocument["name"];
  int userId = jsonDocument["userId"];

  // Build SQL query to update teacher
  sql = "update teacher set ";
  if (!name.isEmpty()) {
    sql += "name='" + name + "',";
  }
  if (userId > 0) {
    sql += "userId=" + String(userId) + ",";
  }
  sql.remove(sql.length() - 1); // remove trailing comma
  sql += " where id=" + String(id);

  Serial.println(sql);
  if (db_exec(db, sql.c_str()) == SQLITE_OK)
  {
    // Create JSON response
    StaticJsonDocument<100> jsonResponse;
    jsonResponse["message"] = "Teacher updated successfully";
    jsonResponse["id"] = id;

    String jsonStr;
    serializeJson(jsonResponse, jsonStr);
    server.send(200, "application/json", jsonStr);
  }
  else
  {
    server.send(400, "text/plain", "Failed to update teacher");
  }
}
