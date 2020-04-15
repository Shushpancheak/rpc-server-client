#ifndef RPC_DAEMON
#define RPC_DAEMON

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <thread>
#include <wait.h>

namespace client {
FILE* Connect(char* portno);
int Disconnect(char* portno);

int ExecuteFile(FILE* daemon, char* path, char** args);
int ExecuteSyscall(FILE* daemon, long syscall_no, int64_t** args);
}

#endif