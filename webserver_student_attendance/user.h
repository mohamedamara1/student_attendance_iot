#ifndef USER_HANDLER_H
#define USER_HANDLER_H

#include <WebServer.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <sqlite3.h>


class user {
  public:
    user();
    void handleCreateUser();
    void handleReadUser();
    void handleUpdateUser();
    void handleDeleteUser();

};

#endif // USER_HANDLER_H
