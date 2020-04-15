#include "daemon.hpp"

int main() {
  Daemon daemon;

  daemon.Start("4444");

  return 0;
}