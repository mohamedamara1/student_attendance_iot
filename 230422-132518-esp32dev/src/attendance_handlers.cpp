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
  // Parse the request body as JSON
  DynamicJsonDocument requestBody(1024);
  DeserializationError error = deserializeJson(requestBody, server.arg("plain"));
  if (error)
  {
    server.send(400, "text/html", "<p>Bad Request</p>");
    return;
  }

  // Extract the attendance's information from the request body
  // int student_id  = requestBody["student_id "];
  // int lesson_id = requestBody["lesson_id"];
  int attendanceId = requestBody["attendance_id"];
  String present = requestBody["present"];

  // Prepare the SQL statement
  sqlite3_stmt *stmt;
  const char *sql = "UPDATE attendance SET present=? WHERE id=? ";
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK)
  {
    server.send(500, "text/html", "<p>Internal Server Error</p>");
    return;
  }

  // Bind the 's information parameters to the prepared statement
  rc = sqlite3_bind_text(stmt, 0, present.c_str(), -1, SQLITE_TRANSIENT);
  rc |= sqlite3_bind_int(stmt, 1, attendanceId);
  if (rc != SQLITE_OK)
  {
    sqlite3_finalize(stmt);
    server.send(500, "text/html", "<p>Internal Server Error</p>");
    return;
  }

  // Execute the prepared statement and check for errors
  rc = sqlite3_step(stmt);
  if (rc != SQLITE_DONE)
  {
    sqlite3_finalize(stmt);
    server.send(500, "text/html", "<p>Internal Server Error</p>");
    return;
  }

  // Send a success response to the client with a status code of 200
  String response = "<p>attendence updated successfully</p>";
  server.send(200, "text/html", response);

  // Finalize the prepared statement
  sqlite3_finalize(stmt);
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
    String presenceWord = "presence/";
    Serial.println(lessonId);

    // presenceString.append(lessonId);
    String presenceString = presenceWord + lessonId;
    Serial.println(presenceString);
    mqttClient.publish(presenceString.c_str(), studentId.c_str());

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
