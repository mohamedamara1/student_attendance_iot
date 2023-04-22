#include "course_handlers.h"

/************************ COURSES **************************/

// Get all courses
void handleGetCourses()
{
    DynamicJsonDocument doc(1024);
    JsonArray array = doc.to<JsonArray>();
    String sql = "SELECT * FROM course";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            JsonObject obj = array.createNestedObject();
            obj["id"] = sqlite3_column_int(stmt, 0);
            obj["name"] = sqlite3_column_text(stmt, 1);
            obj["description"] = sqlite3_column_text(stmt, 2);
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

// Create a new course
void handleCreateCourse()
{
    String sql = "";
    StaticJsonDocument<250> jsonDocument;
    if (server.hasArg("plain") == false)
    {
    }
    String body = server.arg("plain");
    deserializeJson(jsonDocument, body);

    String name = jsonDocument["name"];
    String description = jsonDocument["description"];

    sql = "insert into course(name,description) values('" + name + "','" + description + "')";
    Serial.println(sql);
    if (db_exec(db, sql.c_str()) == SQLITE_OK)
    {
        // Get the id of the newly created course
        int id = sqlite3_last_insert_rowid(db);
        // Create JSON response
        StaticJsonDocument<100> jsonResponse;
        jsonResponse["message"] = "Course created successfully";
        jsonResponse["id"] = id;

        String jsonStr;
        serializeJson(jsonResponse, jsonStr);
        server.send(201, "application/json", jsonStr);
    }
    else
    {
        server.send(400, "text/plain", "Failed to create course");
    }
}

// Delete a course
void handleDeleteCourse()
{
    String sql = "";
    int id = server.arg("id").toInt();
    // int id = server.pathArg(0).toInt();
    Serial.println("delete");

    sql = "delete from course where id=" + String(id);
    Serial.println(sql);
    if (db_exec(db, sql.c_str()) == SQLITE_OK)
    {
        // Create JSON response
        StaticJsonDocument<100> jsonResponse;
        jsonResponse["message"] = "Course deleted successfully";
        jsonResponse["id"] = id;

        String jsonStr;
        serializeJson(jsonResponse, jsonStr);
        server.send(200, "application/json", jsonStr);
    }
    else
    {
        server.send(400, "text/plain", "Failed to delete course");
    }
}