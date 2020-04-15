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

class Daemon {
public:
  /**
   * Start accepting clients.
   */
  void Start(char* portno);

private:
  void WorkWithClient(int client_fd);

  static void Exec(FILE* stream, char* path);
  static void ExecSyscall(FILE* stream, long syscall_no, char* args);

  static void HandleSignal(int signo, siginfo_t* siginfo, void* context);
  static void SetHandler();
  int CreateLocalServer(char* portno);

  static thread_local __sig_atomic_t signalled;
  static const size_t MAX_ARGS_COUNT = 10;
};

#endif