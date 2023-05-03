#include "user_handlers.h"

/*******************    USERS    *************************/

// Get all users
void handleGetUsers()
{
  DynamicJsonDocument doc(1024);
  JsonArray array = doc.to<JsonArray>();
  String sql = "SELECT * FROM user";
  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
  {
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
      JsonObject obj = array.createNestedObject();
      obj["id"] = sqlite3_column_int(stmt, 0);
      obj["email"] = sqlite3_column_text(stmt, 1);
      obj["isTeacher"] = sqlite3_column_int(stmt, 3);
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

// Create a new user
void handleCreateUser()
{
  String sql = "";
  StaticJsonDocument<250> jsonDocument;
  if (server.hasArg("plain") == false)
  {
  }
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);
  // String id = jsonDocument["id"];

  String email = jsonDocument["email"];
  String password = jsonDocument["password"];
  String isTeacher = jsonDocument["isTeacher"];
  sql = "insert into user(email,password,isTeacher) values('" + email + "','" + password + "','" + isTeacher + "')";
  Serial.println(sql);
  if (db_exec(db, sql.c_str()) == SQLITE_OK)
  {
    // Get the id of the newly created user
    int id = sqlite3_last_insert_rowid(db);

    // Create JSON response
    StaticJsonDocument<100> jsonResponse;
    jsonResponse["message"] = "User created successfully";
    jsonResponse["id"] = id;

    String jsonStr;
    serializeJson(jsonResponse, jsonStr);
    server.send(201, "application/json", jsonStr);
  }
  else
  {
    server.send(400, "text/plain", "Failed to create user");
  }
}

void handleDeleteUser()
{
  String sql = "";
  int id = server.arg("id").toInt();
  // int id = server.pathArg(0).toInt();
  Serial.println("delete");

  sql = "delete from user where id=" + String(id);
  Serial.println(sql);
  if (db_exec(db, sql.c_str()) == SQLITE_OK)
  {
    // Create JSON response
    StaticJsonDocument<100> jsonResponse;
    jsonResponse["message"] = "User deleted successfully";
    jsonResponse["id"] = id;

    String jsonStr;
    serializeJson(jsonResponse, jsonStr);
    server.send(200, "application/json", jsonStr);
  }
  else
  {
    server.send(400, "text/plain", "Failed to delete user");
  }
}
void handleGetUser()
{  
  String sql = "";
  int id = server.arg("id").toInt();
  DynamicJsonDocument doc(1024);
  JsonObject response = doc.to<JsonObject>();

  sql = "select * from user where id=" + String(id);
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
  {
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
      response["id"] = sqlite3_column_int(stmt, 0);
      response["email"] = sqlite3_column_text(stmt, 1);
      response["isTeacher"] = sqlite3_column_int(stmt, 3);
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

// Update an existing user
void handleUpdateUser()
{
  String sql = "";
  int id = server.arg("id").toInt();
  // int id = server.pathArg(0).toInt();

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
  String email = jsonDocument["email"];
  String password = jsonDocument["password"];
  String isTeacher = jsonDocument["isTeacher"];

  // Build SQL query to update user
  sql = "update user set ";
  if (!email.isEmpty())
  {
    sql += "email='" + email + "',";
  }
  if (!password.isEmpty())
  {
    sql += "password='" + password + "',";
  }
  if (!isTeacher.isEmpty())
  {
    sql += "isTeacher='" + isTeacher + "',";
  }
  sql.remove(sql.length() - 1); // remove trailing comma
  sql += " where id=" + String(id);

  Serial.println(sql);
  if (db_exec(db, sql.c_str()) == SQLITE_OK)
  {
    // Create JSON response
    StaticJsonDocument<100> jsonResponse;
    jsonResponse["message"] = "User updated successfully";
    jsonResponse["id"] = id;

    String jsonStr;
    serializeJson(jsonResponse, jsonStr);
    server.send(200, "application/json", jsonStr);
  }
  else
  {
    server.send(400, "text/plain", "Failed to update user");
  }
}
