#include "lesson_handlers.h"


// Get all lessons
void handleGetLessons() {
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
        obj["start_time"] = sqlite3_column_text(stmt, 6);        obj["start_time"] = sqlite3_column_int(stmt, 3);
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