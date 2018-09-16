#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"

// Shows the help message and exits the program.
// Args:
//  program: The name of the program.
void PrintHelpAndExit(const char *program) {
  printf("Usage: %s server port encryption_key file\n", program);
  printf("(Key must be 2 characters long.)\n");
  exit(1);
}

int main(int argc, const char **argv) {
  // Get the arguments.
  if (argc != 5) {
    PrintHelpAndExit(argv[0]);
  }

  const char *server = argv[1];
  const uint32_t port = atoi(argv[2]);
  const char *key = argv[3];
  const char *filename = argv[4];
  if (strlen(key) != 2) {
    // Key is not the right length.
    PrintHelpAndExit(argv[0]);
  }

  // Create the client.
  ::hw1::transfer::Client client((const uint8_t *)key);
  if (!client.Connect(server, port)) {
    return 1;
  }

  // Send the file.
  if (!client.SendFile(filename)) {
    return 1;
  }

  return 0;
}
