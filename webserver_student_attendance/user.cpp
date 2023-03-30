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
