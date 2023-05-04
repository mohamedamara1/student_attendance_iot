#include "student_handlers.h"

/*******************    STUDENTS    *************************/

// Get all students
void handleGetStudents()
{
  DynamicJsonDocument doc(1024);
  JsonArray array = doc.to<JsonArray>();
  String sql = "SELECT * FROM student";
  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
  {
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
      JsonObject obj = array.createNestedObject();
      obj["id"] = sqlite3_column_int(stmt, 0);
      obj["name"] = sqlite3_column_text(stmt, 1);
      obj["userId"] = sqlite3_column_int(stmt, 2);
      obj["classId"] = sqlite3_column_int(stmt, 3);
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

// Create a new student
void handleCreateStudent()
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
  int classId = jsonDocument["classId"];
  sql = "insert into student(name,user_id,class_id) values('" + name + "','" + userId + "','" + classId + "')";
  Serial.println(sql);
  if (db_exec(db, sql.c_str()) == SQLITE_OK)
  {
    // Get the id of the newly created student
    int id = sqlite3_last_insert_rowid(db);

    // Create JSON response
    StaticJsonDocument<100> jsonResponse;
    jsonResponse["message"] = "Student created successfully";
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

void handleDeleteStudent()
{
  String sql = "";
  int id = server.arg("id").toInt();
  // int id = server.pathArg(0).toInt();
  Serial.println("delete");

  sql = "delete from student where id=" + String(id);
  Serial.println(sql);
  if (db_exec(db, sql.c_str()) == SQLITE_OK)
  {
    // Create JSON response
    StaticJsonDocument<100> jsonResponse;
    jsonResponse["message"] = "Student deleted successfully";
    jsonResponse["id"] = id;

    String jsonStr;
    serializeJson(jsonResponse, jsonStr);
    server.send(200, "application/json", jsonStr);
  }
  else
  {
    server.send(400, "text/plain", "Failed to delete student");
  }
}

void handleGetStudentById()
{
  String sql = "";
  int id = server.arg("id").toInt();

  sql = "SELECT * FROM student WHERE id=" + String(id);
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
  {
    DynamicJsonDocument doc(1024);
    JsonObject response = doc.to<JsonObject>();

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
      response["id"] = sqlite3_column_int(stmt, 0);
      response["name"] = sqlite3_column_text(stmt, 1);
      response["userId"] = sqlite3_column_int(stmt, 2);
      response["classId"] = sqlite3_column_int(stmt, 3);
    }

    String jsonStr;
    serializeJson(doc, jsonStr);
    server.send(200, "application/json", jsonStr);
  }
  else
  {
    server.send(400, "text/plain", "Failed to get student");
  }
}

void handleGetStudentByUserId()
{

  String sql = "";
  int userId = server.arg("userId").toInt();

  sql = "select * from student where user_id=" + String(userId);
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
  {
    DynamicJsonDocument doc(1024);
    JsonObject response = doc.to<JsonObject>();

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
      response["id"] = sqlite3_column_int(stmt, 0);
      response["name"] = sqlite3_column_text(stmt, 1);
      response["userId"] = sqlite3_column_int(stmt, 2);
      response["classId"] = sqlite3_column_int(stmt, 3);
    }

    String jsonStr;
    serializeJson(doc, jsonStr);
    server.send(200, "application/json", jsonStr);
  }
  else
  {
    server.send(400, "text/plain", "Failed to get student");
  }
}

// Update an existing student
void handleUpdateStudent()
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
  String userId = jsonDocument["userId"];
  String classId = jsonDocument["classId"];

  // Build SQL query to update student
  sql = "update student set ";
  if (!name.isEmpty())
  {
    sql += "name='" + name + "',";
  }
  if (!userId.isEmpty())
  {
    sql += "user_id='" + userId + "',";
  }
  if (!classId.isEmpty())
  {
    sql += "class_id='" + classId + "',";
  }
  sql.remove(sql.length() - 1); // remove trailing comma
  sql += " where id=" + String(id);

  Serial.println(sql);
  if (db_exec(db, sql.c_str()) == SQLITE_OK)
  {
    // Create JSON response
    StaticJsonDocument<100> jsonResponse;
    jsonResponse["message"] = "Student updated successfully";
    jsonResponse["id"] = id;

    String jsonStr;
    serializeJson(jsonResponse, jsonStr);
    server.send(200, "application/json", jsonStr);
  }
  else
  {
    server.send(400, "text/plain", "Failed to update student");
  }
}
