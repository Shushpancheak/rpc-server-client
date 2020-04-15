#include <iostream>
#include "daemon.hpp"

void Daemon::Start(char* portno) {
  SetHandler();
  int server_fd = CreateLocalServer(portno);

  while (!signalled) {
    int cur_fd = accept(server_fd, nullptr, nullptr);

    // TODO: use thread pool
    std::thread new_thread (&Daemon::WorkWithClient, this, cur_fd);
  }

  // TODO: join threads...

  close(server_fd);
}

void Daemon::WorkWithClient(int client_fd) {
  // Block all signals
  sigset_t mask;
  sigfillset(&mask);
  sigprocmask(SIG_SETMASK, &mask, NULL);

  FILE* client = fdopen(client_fd, "r+");

  char* line = nullptr;
  size_t len = 0;

  while (strcmp(line, "EXIT\n") != 0) {
    getline(&line, &len, client);

    char* query = strtok(line, " ");

    if (!strcmp(query, "EXECUTE")) {
      char* path = strtok(nullptr, " ");

      Exec(client, path);
    } else if (!strcmp(query, "SYSCALL")) {
      char* syscall_num_str = strtok(nullptr, " ");
      long syscall_num = strtol(syscall_num_str, nullptr, 10);

      // TODO: if no arguments, it will fail...
      ExecSyscall(client, syscall_num, syscall_num_str + strlen(syscall_num_str) + 1);
    } else {
      fprintf(client, "INVALID QUERY\n");
    }
  }

  free(line);
}

thread_local __sig_atomic_t Daemon::signalled = 0;

void Daemon::HandleSignal(int signo, siginfo_t* siginfo, void* context)
{
  signalled = 1;
}

void Daemon::SetHandler() {
  struct sigaction act = {.sa_sigaction = Daemon::HandleSignal,
                          .sa_flags = SA_SIGINFO};
  sigaction(SIGINT, &act, NULL);
  sigaction(SIGTERM, &act, NULL);
}

int Daemon::CreateLocalServer(char* portno) {
  int server_fd = 0;

  if (-1 == (server_fd = socket(AF_INET, SOCK_STREAM, 0))) {
    perror("socket");
    return 1;
  };

  struct sockaddr_in address = {AF_INET, htons(strtol(portno, NULL, 10)), 0};
  inet_aton("127.0.0.1", &address.sin_addr);

  if (-1 == bind(server_fd, (struct sockaddr*)&address, sizeof(address))) {
    perror("bind");
    return 1;
  }

  if (listen(server_fd, SOMAXCONN) == -1) {
    perror("listen");
    return 1;
  }

  return server_fd;
}
void Daemon::Exec(FILE *stream, char *path) {
  if (0 != access(path, F_OK)) {
    fprintf(stream, "NOT FOUND\n");
  } else if (0 != access(path, R_OK)) {
    fprintf(stream, "NOT EXECUTABLE\n");
  } else {
    fprintf(stream, "OK\n");

    pid_t pid = fork();

    if (pid == 0) {
      execl(path, path, NULL);
      fprintf(stream, "FAIL\n");
    } else {
      int wstatus = 0;
      waitpid(pid, &wstatus, 0);

      fprintf(stream, "%d\n", wstatus);
    }
  }
}
void Daemon::ExecSyscall(FILE *stream, long syscall_no, char* args) {
  size_t args_count = 0;
  int64_t arg[MAX_ARGS_COUNT] = {};

  args = strtok(args, " ");

  while (args[0] != '\0') {
    arg[args_count++] = strtoll(args, nullptr, 10);
    args = strtok(nullptr, " ");
  }

  long res = 0;

  switch (args_count) {
  case 0: res = syscall(syscall_no); break;
  case 1: res = syscall(syscall_no, arg[0]); break;
  case 2: res = syscall(syscall_no, arg[0], arg[1]); break;
  case 3: res = syscall(syscall_no, arg[0], arg[1], arg[2]); break;
  case 4: res = syscall(syscall_no, arg[0], arg[1], arg[2], arg[3]); break;
  case 5: res = syscall(syscall_no, arg[0], arg[1], arg[2], arg[3], arg[4]); break;
  case 6: res = syscall(syscall_no, arg[0], arg[1], arg[2], arg[3], arg[4], arg[5]); break;
  case 7: res = syscall(syscall_no, arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6]); break;
  case 8: res = syscall(syscall_no, arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7]); break;
  case 9: res = syscall(syscall_no, arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8]); break;

  default:
    fprintf(stream, "INVALID QUERY\n");
  }

  fprintf(stream, "%d\n", res);
}
