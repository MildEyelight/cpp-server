#pragma once
#include "Socket.h"

int handle_event(int client_fd);
int client_event(const Socket& sock);
int client_event(int server_fd);