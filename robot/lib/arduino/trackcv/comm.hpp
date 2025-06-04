#ifndef COMM_H
#define COMM_H

#include "log.hpp"

bool comm_open(char* socketName, Logger* log);
bool comm_send(char data);
int comm_recv();
void comm_close();

#endif