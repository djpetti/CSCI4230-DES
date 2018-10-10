#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "key_server.h"

// Shows the help message and exits the program.
// Args:
//  program: The name of the program.
void PrintHelpAndExit(const char *program) {
  printf("Usage: %s port\n", program);
  exit(1);
}

int main(int argc, const char **argv) {
  // Get the arguments.
  if (argc != 2) {
    PrintHelpAndExit(argv[0]);
  }

  const uint32_t port = atoi(argv[1]);

  // Create the server.
  ::hw1::key_exchange::KeyServer server;

  if (!server.Listen(port)) {
    return 1;
  }

  while (true) {
    server.HandleConnection();
  }

  return 0;
}
