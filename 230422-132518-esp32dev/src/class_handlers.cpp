#include "class_handlers.h"

/******************* CLASSES *************************/

// Get all classes
void handleGetClasses()
{
    DynamicJsonDocument doc(1024);
    JsonArray array = doc.to<JsonArray>();
    String sql = "SELECT * FROM class";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            JsonObject obj = array.createNestedObject();
            obj["id"] = sqlite3_column_int(stmt, 0);
            obj["name"] = sqlite3_column_text(stmt, 1);
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

// Create a new class
void handleCreateClass()
{
    String sql = "";
    StaticJsonDocument<250> jsonDocument;
    if (server.hasArg("plain") == false)
    {
    }
    String body = server.arg("plain");
    deserializeJson(jsonDocument, body);

    String name = jsonDocument["name"];
    sql = "INSERT INTO class(name) values('" + name + "')";
    Serial.println(sql);
    if (db_exec(db, sql.c_str()) == SQLITE_OK)
    {
        // Get the id of the newly created class
        int id = sqlite3_last_insert_rowid(db);
        // Create JSON response
        StaticJsonDocument<100> jsonResponse;
        jsonResponse["message"] = "Class created successfully";
        jsonResponse["id"] = id;

        String jsonStr;
        serializeJson(jsonResponse, jsonStr);
        server.send(201, "application/json", jsonStr);
    }
    else
    {
        server.send(400, "text/plain", "Failed to create class");
    }
}

// Delete a class
void handleDeleteClass()
{
    String sql = "";
    int id = server.arg("id").toInt();

    sql = "DELETE FROM class WHERE id=" + String(id);
    Serial.println(sql);
    if (db_exec(db, sql.c_str()) == SQLITE_OK)
    {
        // Create JSON response
        StaticJsonDocument<100> jsonResponse;
        jsonResponse["message"] = "Class deleted successfully";
        jsonResponse["id"] = id;
        String jsonStr;
        serializeJson(jsonResponse, jsonStr);
        server.send(200, "application/json", jsonStr);
    }
    else
    {
        server.send(400, "text/plain", "Failed to delete class");
    }
}