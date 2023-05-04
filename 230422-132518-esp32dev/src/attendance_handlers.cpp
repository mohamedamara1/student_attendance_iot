#include "attendance_handlers.h"

void handleGetAttendances()
{
  DynamicJsonDocument doc(1024);
  JsonArray array = doc.to<JsonArray>();
  String sql = "SELECT * FROM attendance";
  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
  {
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
      JsonObject obj = array.createNestedObject();
      obj["id"] = sqlite3_column_int(stmt, 0);
      obj["lessonId"] = sqlite3_column_text(stmt, 1);
      obj["studentId"] = sqlite3_column_text(stmt, 2);
      obj["present"] = sqlite3_column_int(stmt, 3);
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

// Update attendance : HTML

void handleUpdateAttendance()
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
  bool present = jsonDocument["present"];

  // Build SQL query to update attendance
  sql = "update attendance set ";
  sql += "present=" + String(present ? 1 : 0);
  sql += " where id=" + String(id);

  Serial.println(sql);
  if (db_exec(db, sql.c_str()) == SQLITE_OK)
  {
    // Create JSON response
    StaticJsonDocument<100> jsonResponse;
    jsonResponse["message"] = "Attendance updated successfully";
    jsonResponse["id"] = id;

    String jsonStr;
    serializeJson(jsonResponse, jsonStr);
    server.send(200, "application/json", jsonStr);
  }
  else
  {
    server.send(400, "text/plain", "Failed to update attendance");
  }
}

void handleCreateAttendance()
{
  String sql = "";
  StaticJsonDocument<250> jsonDocument;
  if (server.hasArg("plain") == false)
  {
    server.send(400, "text/plain", "Invalid request");
    return;
  }

  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);

  String lessonId = jsonDocument["lessonId"];
  String studentId = jsonDocument["studentId"];

  // insert attendance record into the database
  sql = "INSERT INTO attendance(lesson_id, student_id, present) VALUES('" + lessonId + "', '" + studentId + "', '1')";
  Serial.println(sql);
  if (db_exec(db, sql.c_str()) == SQLITE_OK)
  {
    // publish to MQTT topic
    String presenceWord = "presence";
    Serial.println("lesson id : " + lessonId + " yes");

    // presenceString.append(lessonId);
    presenceWord += lessonId;
    //Serial.println(presenceString);
    mqttClient.publish(presenceWord.c_str(), studentId.c_str());

    // create JSON response
    StaticJsonDocument<100> jsonResponse;
    jsonResponse["message"] = "Attendance created successfully";
    String jsonStr;
    serializeJson(jsonResponse, jsonStr);
    server.send(201, "application/json", jsonStr);
  }
  else
  {
    server.send(400, "text/plain", "Failed to create attendance");
  }
}

void handleDeleteAttendance()
{
  int id = server.arg("id").toInt();
  Serial.println("delete attendance id: " + String(id));

  // Construct the SQL query to delete the attendance record
  String sql = "DELETE FROM attendance WHERE id=" + String(id);

  // Execute the query
  if (db_exec(db, sql.c_str()) == SQLITE_OK)
  {
    // Create JSON response
    StaticJsonDocument<100> jsonResponse;
    jsonResponse["message"] = "Attendance deleted successfully";
    jsonResponse["id"] = id;

    String jsonStr;
    serializeJson(jsonResponse, jsonStr);
    server.send(200, "application/json", jsonStr);
  }
  else
  {
    server.send(400, "text/plain", "Failed to delete attendance");
  }
}

void handleGetAttendancesByLessonId()
{
  String lessonId = server.arg("lessonId");

  String sql = "SELECT attendance.id, attendance.lesson_id, student.name AS student_name, course.name AS course_name, class.name AS class_name, attendance.present "
               "FROM attendance "
               "JOIN student ON attendance.student_id = student.id "
               "JOIN lesson ON attendance.lesson_id = lesson.id "
               "JOIN course ON lesson.course_id = course.id "
               "JOIN class ON lesson.class_id = class.id "
               "WHERE attendance.lesson_id =" +
               lessonId;
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
  {
    DynamicJsonDocument doc(1024);
    JsonArray responseArray = doc.to<JsonArray>();

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
      JsonObject attendanceObject = doc.createNestedObject();
      attendanceObject["id"] = sqlite3_column_int(stmt, 0);
      attendanceObject["lessonId"] = sqlite3_column_int(stmt, 1);

      attendanceObject["student_name"] = sqlite3_column_text(stmt, 2);
      attendanceObject["course_name"] = sqlite3_column_text(stmt, 3);
      attendanceObject["class_name"] = sqlite3_column_text(stmt, 4);

      attendanceObject["present"] = sqlite3_column_int(stmt, 5);
    }

    String jsonStr;
    serializeJson(doc, jsonStr);
    server.send(200, "application/json", jsonStr);
  }
  else
  {
    server.send(400, "text/plain", "Failed to get attendance by lesson ID");
  }
}
