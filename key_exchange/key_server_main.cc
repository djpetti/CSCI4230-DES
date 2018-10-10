#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "key_server.h"

// Shows the help message and exits the program.
// Args:
//  program: The name of the program.
void PrintHelpAndExit(const char *program) {
  printf("Usage: %s port key_b\n", program);
  printf("(Keys must be 2 characters long.)\n");
  exit(1);
}

int main(int argc, const char **argv) {
  // Get the arguments.
  if (argc != 3) {
    PrintHelpAndExit(argv[0]);
  }

  const uint32_t port = atoi(argv[1]);
  const char *key_b = argv[2];
  if (strlen(key_b) != 2) {
    // Key is not the right length.
    PrintHelpAndExit(argv[0]);
  }

  // Create the server.
  ::hw1::key_exchange::KeyServer server;

  // Add some clients for testing.
  server.AddClient(1, (const uint8_t *)key_b);

  if (!server.Listen(port)) {
    return 1;
  }

  while (true) {
    server.HandleConnection();
  }

  return 0;
}
