#include "lesson_handlers.h"

// Get all lessons
void handleGetLessons()
{
  DynamicJsonDocument doc(1024);
  JsonArray array = doc.to<JsonArray>();
  String sql = "SELECT * FROM lesson";
  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
  {
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
      JsonObject obj = array.createNestedObject();
      obj["id"] = sqlite3_column_int(stmt, 0);
      obj["course_id"] = sqlite3_column_int(stmt, 1);
      obj["class_id"] = sqlite3_column_int(stmt, 2);
      obj["teacher_id"] = sqlite3_column_int(stmt, 3);
      obj["classroom_id"] = sqlite3_column_int(stmt, 4);
      obj["date"] = sqlite3_column_text(stmt, 5);
      obj["start_time"] = sqlite3_column_text(stmt, 6);
      obj["start_time"] = sqlite3_column_int(stmt, 3);
      obj["end_time"] = sqlite3_column_text(stmt, 7);
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

// Create a new lesson
void handleCreateLesson()
{
  String sql = "";

  // Parse JSON request body
  StaticJsonDocument<500> jsonDocument;
  if (server.hasArg("plain") == false)
  {
    server.send(400, "text/plain", "Missing request body");
    return;
  }
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);

  // Retrieve fields from JSON request body
  int courseId = jsonDocument["course_id"];
  int classId = jsonDocument["class_id"];
  int teacherId = jsonDocument["teacher_id"];
  int classroomId = jsonDocument["classroom_id"];
  String date = jsonDocument["date"];
  String startTime = jsonDocument["start_time"];
  String endTime = jsonDocument["end_time"];

  // Build SQL query to create lesson
  sql = "insert into lesson (course_id, class_id, teacher_id, classroom_id, date, start_time, end_time) ";
  sql += "values (" + String(courseId) + ", " + String(classId) + ", " + String(teacherId) + ", " + String(classroomId) + ", '" + date + "', '" + startTime + "', '" + endTime + "')";

  Serial.println(sql);
  if (db_exec(db, sql.c_str()) == SQLITE_OK)
  {
    // Get the id of the newly created user
    int id = sqlite3_last_insert_rowid(db);

    // Create JSON response
    StaticJsonDocument<100> jsonResponse;
    jsonResponse["message"] = "Lesson created successfully";
    jsonResponse["id"] = id;

    String jsonStr;
    serializeJson(jsonResponse, jsonStr);
    server.send(200, "application/json", jsonStr);
  }
  else
  {
    server.send(400, "text/plain", "Failed to create lesson");
  }
}

// Get a lesson by ID
void handleGetLesson()
{
  int id = server.arg("id").toInt();
  String sql = "select * from lesson where id=" + String(id);

  Serial.println(sql);
  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
  {
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
      // Create JSON response
      StaticJsonDocument<500> jsonResponse;
      jsonResponse["id"] = sqlite3_column_int(stmt, 0);
      jsonResponse["course_id"] = sqlite3_column_int(stmt, 1);
      jsonResponse["class_id"] = sqlite3_column_int(stmt, 2);
      jsonResponse["teacher_id"] = sqlite3_column_int(stmt, 3);
      jsonResponse["classroom_id"] = sqlite3_column_int(stmt, 4);
      jsonResponse["date"] = String(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5)));
      jsonResponse["start_time"] = String(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 6)));
      jsonResponse["end_time"] = String(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 7)));

      String jsonStr;
      serializeJson(jsonResponse, jsonStr);
      server.send(200, "application/json", jsonStr);
    }
    else
    {
      server.send(404, "text/plain", "Lesson not found");
    }
    sqlite3_finalize(stmt);
  }
  else
  {
    server.send(400, "text/plain", "Failed to read lesson");
  }
}
// Update an existing lesson
void handleUpdateLesson()
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
  int course_id = jsonDocument["course_id"];
  int class_id = jsonDocument["class_id"];
  int teacher_id = jsonDocument["teacher_id"];
  int classroom_id = jsonDocument["classroom_id"];
  String date = jsonDocument["date"];
  String start_time = jsonDocument["start_time"];
  String end_time = jsonDocument["end_time"];

  // Build SQL query to update lesson
  sql = "update lesson set ";
  if (course_id != 0)
  {
    sql += "course_id=" + String(course_id) + ",";
  }
  if (class_id != 0)
  {
    sql += "class_id=" + String(class_id) + ",";
  }
  if (teacher_id != 0)
  {
    sql += "teacher_id=" + String(teacher_id) + ",";
  }
  if (classroom_id != 0)
  {
    sql += "classroom_id=" + String(classroom_id) + ",";
  }
  if (!date.isEmpty())
  {
    sql += "date='" + date + "',";
  }
  if (!start_time.isEmpty())
  {
    sql += "start_time='" + start_time + "',";
  }
  if (!end_time.isEmpty())
  {
    sql += "end_time='" + end_time + "',";
  }
  sql.remove(sql.length() - 1); // remove trailing comma
  sql += " where id=" + String(id);

  Serial.println(sql);
  if (db_exec(db, sql.c_str()) == SQLITE_OK)
  {
    // Create JSON response
    StaticJsonDocument<100> jsonResponse;
    jsonResponse["message"] = "Lesson updated successfully";
    jsonResponse["id"] = id;

    String jsonStr;
    serializeJson(jsonResponse, jsonStr);
    server.send(200, "application/json", jsonStr);
  }
  else
  {
    server.send(400, "text/plain", "Failed to update lesson");
  }
}

// Delete a lesson
void handleDeleteLesson()
{
  int id = server.arg("id").toInt();

  // Build SQL query to delete lesson
  String sql = "delete from lesson where id=" + String(id);

  Serial.println(sql);
  if (db_exec(db, sql.c_str()) == SQLITE_OK)
  {
    // Create JSON response
    StaticJsonDocument<100> jsonResponse;
    jsonResponse["message"] = "Lesson deleted successfully";
    jsonResponse["id"] = id;

    String jsonStr;
    serializeJson(jsonResponse, jsonStr);
    server.send(200, "application/json", jsonStr);
  }
  else
  {
    server.send(400, "text/plain", "Failed to delete lesson");
  }
}
void handleGetNewestLesson()
{
  String studentId = server.arg("student_id");

  DynamicJsonDocument doc(1024);
  JsonObject data = doc.createNestedObject("data");

  String sql = "SELECT l.id, l.date, l.start_time, l.end_time, "
               "c.id AS course_id, c.name AS course_name, "
               "t.id AS teacher_id, t.name AS teacher_name, "
               "cl.id AS classroom_id, cl.number AS classroom_number "
               "FROM lesson AS l "
               "JOIN course AS c ON l.course_id = c.id "
               "JOIN teacher AS t ON l.teacher_id = t.id "
               "JOIN classroom AS cl ON l.classroom_id = cl.id "
               "JOIN class AS cls ON l.class_id = cls.id "
               "JOIN student AS s ON cls.id = s.class_id "
               "WHERE s.id = ? "
               "ORDER BY l.date DESC, l.start_time DESC "
               "LIMIT 1;";

  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
  {
    sqlite3_bind_text(stmt, 1, studentId.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
      data["id"] = sqlite3_column_int(stmt, 0);
      data["date"] = sqlite3_column_text(stmt, 1);
      data["start_time"] = sqlite3_column_text(stmt, 2);
      data["end_time"] = sqlite3_column_text(stmt, 3);
      data["course_id"] = sqlite3_column_int(stmt, 4);
      data["course_name"] = String(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5)));
      data["teacher_id"] = sqlite3_column_int(stmt, 6);
      data["teacher_name"] = String(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 7)));
      data["classroom_id"] = sqlite3_column_int(stmt, 8);
      data["classroom_name"] = String(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 9)));
    }
    else
    {
      server.send(400, "text/html", "No lessons found for the student ID");
      return;
    }
  }
  else
  {
    server.send(400, "text/html", "Failed to execute SQL query");
    return;
  }

  String jsonStr;
  serializeJson(doc, jsonStr);
  server.send(200, "application/json", jsonStr);
}

void handleGetLessonsByTeacherId()
{

  String teacherId = server.arg("teacherId");

  String sql = "SELECT lesson.id, course.name AS course_name, class.name AS class_name, classroom.number AS classroom_number, lesson.date, lesson.start_time, lesson.end_time "
               "FROM lesson "
               "JOIN course ON lesson.course_id = course.id "
               "JOIN class ON lesson.class_id = class.id "
               "JOIN classroom ON lesson.classroom_id = classroom.id "
               "WHERE lesson.teacher_id = '" +
               teacherId + "' "
                           "ORDER BY lesson.date DESC";

  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
  {
    DynamicJsonDocument doc(1024);
    JsonArray response = doc.to<JsonArray>();

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
      JsonObject lesson = response.createNestedObject();

      lesson["id"] = sqlite3_column_int(stmt, 0);
      lesson["Course"] = sqlite3_column_text(stmt, 1);
      lesson["Class"] = sqlite3_column_text(stmt, 2);
      lesson["Classroom"] = sqlite3_column_text(stmt, 3);
      lesson["Date"] = sqlite3_column_text(stmt, 4);
      lesson["Starttime"] = sqlite3_column_text(stmt, 5);
      lesson["Endtime"] = sqlite3_column_text(stmt, 6);
    }

    String jsonStr;
    serializeJson(doc, jsonStr);
    server.send(200, "application/json", jsonStr);
  }
  else
  {
    server.send(400, "text/plain", "Failed to get lessons");
  }
}
