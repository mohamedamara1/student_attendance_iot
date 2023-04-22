#include "attendance_handlers.h"

//Update attendance : HTML 

void handleUpdateAttendance(WebServer* server, sqlite3* db, const String& attendanceId) {
  // Parse the request body as JSON
  DynamicJsonDocument requestBody(1024);
  DeserializationError error = deserializeJson(requestBody, server->arg("plain"));
  if (error) {
    server->send(400, "text/html", "<p>Bad Request</p>");
    return;
  }

  // Extract the attendance's information from the request body
  int student_id  = requestBody["student_id "];
  int lesson_id = requestBody["lesson_id"];
  char present = requestBody["present"];


  // Prepare the SQL statement
  sqlite3_stmt* stmt;
  const char* sql = "UPDATE user SET present=? WHERE id=?";
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    server->send(500, "text/html", "<p>Internal Server Error</p>");
    return;
  }

  // Bind the 's information parameters to the prepared statement
  rc = sqlite3_bind_text(stmt, 1, &present, -1, SQLITE_TRANSIENT);
  rc |= sqlite3_bind_int(stmt, 3, attendanceId.toInt());
  if (rc != SQLITE_OK) {
    sqlite3_finalize(stmt);
    server->send(500, "text/html", "<p>Internal Server Error</p>");
    return;
  }

  // Execute the prepared statement and check for errors
  rc = sqlite3_step(stmt);
  if (rc != SQLITE_DONE) {
    sqlite3_finalize(stmt);
    server->send(500, "text/html", "<p>Internal Server Error</p>");
    return;
  }

  // Send a success response to the client with a status code of 200
  String response = "<p>attendence updated successfully</p>";
  server->send(200, "text/html", response);

  // Finalize the prepared statement
  sqlite3_finalize(stmt);
}
