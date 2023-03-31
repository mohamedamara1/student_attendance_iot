#include "user.h"

// Get all users
void handleGetUsers(WebServer* server, Database* db) {
  web_content ="<table style='width:100%'><tr><th>Id</th><th>Email</th><th>IsTeacher</th><th>DEL</th></tr>";
  String sql = "SELECT * FROM user";
    if (db_exec(db, sql.c_str()) == SQLITE_OK) {  
      web_content += "</table>";   
    }
  else 
    web_content = "FAIL";  
  server.send (200, "text/html", web_content );  

}
/*
// or this code 

void handleGetUsers(WebServer* server, sqlite3* db) {
  web_content = "<table style='width:100%'><tr><th>Id</th><th>Email</th><th>IsTeacher</th><th>DEL</th></tr>";
  
  sqlite3_stmt* stmt;
  const char* sql = "SELECT * FROM user";
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc == SQLITE_OK) {
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      int id = sqlite3_column_int(stmt, 0);
      const char* email = (const char*)sqlite3_column_text(stmt, 1);
      int is_teacher = sqlite3_column_int(stmt, 2);
      web_content += "<tr><td>" + String(id) + "</td><td>" + String(email) + "</td><td>" + String(is_teacher) + "</td><td>DEL</td></tr>";
    }
    web_content += "</table>";
    sqlite3_finalize(stmt);
  } else {
    web_content = "FAIL";
  }
  
  server->send(200, "text/html", web_content);
}
*/

//---------------------------------Get user by ID : Format the user's information as an HTML table -----------------------------------------
/*
void handleReadUser(WebServer* server, sqlite3* db, const String& userId) {
  // Prepare the SQL statement
  sqlite3_stmt* stmt;
  const char* sql = "SELECT * FROM user WHERE id=?";
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    server->send(500, "text/plain", "Internal Server Error");
    return;
  }

  // Bind the user ID parameter to the prepared statement
  rc = sqlite3_bind_int(stmt, 1, userId.toInt());
  if (rc != SQLITE_OK) {
    sqlite3_finalize(stmt);
    server->send(500, "text/plain", "Internal Server Error");
    return;
  }

  // Execute the prepared statement and retrieve the user's information
  rc = sqlite3_step(stmt);
  if (rc == SQLITE_ROW) {
    int id = sqlite3_column_int(stmt, 0);
    const char* email = (const char*)sqlite3_column_text(stmt, 1);
    int is_teacher = sqlite3_column_int(stmt, 2);

    // Format the user's information as an HTML table
    String htmlResponse = "<table><tr><th>ID</th><th>Email</th><th>Is Teacher</th></tr>";
    htmlResponse += "<tr><td>" + String(id) + "</td><td>" + String(email) + "</td><td>" + String(is_teacher) + "</td></tr>";
    htmlResponse += "</table>";

    // Send the HTML response to the client with a status code of 200
    server->send(200, "text/html", htmlResponse);
  } else {
    server->send(404, "text/plain", "User not found");
  }

  // Finalize the prepared statement
  sqlite3_finalize(stmt);
}
*/

//--------------------------------------------




/*
// Get a user by ID
void handleGetUserById(WebServer* server, Database* db) {
  // Get the user ID from the URL parameter
  int userId = server->arg("id").toInt();

  // Query the database to get the user by ID
  User user = db->getUserById(userId);

  // Check if the user exists
  if (user.getId() == -1) {
    // User not found, send a 404 response
    server->send(404);
    return;
  }

  // Convert the user to JSON
  String json = userToJson(user);

  // Send the JSON response to the client
  server->send(200, "application/json", json);
}

// Create a new user
void handleCreateUser(WebServer* server, Database* db) {
  // Get the user details from the request body
  String body = server->arg("plain");
  User user = jsonToUser(body);

  // Check if the email already exists
  if (db->getUserByEmail(user.getEmail()).getId() != -1) {
    // Email already exists, send a 409 response
    server->send(409);
    return;
  }

  // Insert the user into the database
  db->insertUser(user);

  // Send a success response
  server->send(201);
}

// Update an existing user
void handleUpdateUser(WebServer* server, Database* db) {
  // Get the user ID from the URL parameter
  int userId = server->arg("id").toInt();

  // Query the database to get the user by ID
  User user = db->getUserById(userId);

  // Check if the user exists
  if (user.getId() == -1) {
    // User not found, send a 404 response
    server->send(404);
    return;
  }

  // Get the updated user details from the request body
  String body = server->arg("plain");
  User updatedUser = jsonToUser(body);

  // Update the user in the database
  db->updateUser(updatedUser);

  // Send a success response
  server->send(204);
}

// Delete a user by ID
void handleDeleteUser(WebServer* server, Database* db) {
  // Get the user ID from the URL parameter
  int userId = server->arg("id").toInt();

  // Query the database to get the user by ID
  User user = db->getUserById(userId);

  // Check if the user exists
  if (user.getId() == -1) {
    // User not found, send a 404 response
    server->send(404);
    return;
  }

  // Delete the user from the database
  db->deleteUser(userId);

  // Send a success response
  server->send(204);
}
*/
