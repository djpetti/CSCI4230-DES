#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_server.h"

// Shows the help message and exits the program.
// Args:
//  program: The name of the program.
void PrintHelpAndExit(const char *program) {
  printf("Usage: %s port encryption_key\n", program);
  printf("(Key must be 2 characters long.)\n");
  exit(1);
}

int main(int argc, const char **argv) {
  // Get the arguments.
  if (argc != 3) {
    PrintHelpAndExit(argv[0]);
  }

  const uint32_t port = atoi(argv[1]);
  const char *key = argv[2];
  if (strlen(key) != 2) {
    // Key is not the right length.
    PrintHelpAndExit(argv[0]);
  }

  // Create the server.
  ::hw1::transfer::Server server((const uint8_t *)key);
  if (!server.Listen(port)) {
    return 1;
  }

  while (true) {
    server.HandleConnection();
  }

  return 0;
}
