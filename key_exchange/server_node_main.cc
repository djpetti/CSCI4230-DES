#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server_node.h"

// Shows the help message and exits the program.
// Args:
//  program: The name of the program.
void PrintHelpAndExit(const char *program) {
  printf("Usage: %s port kdc_address kdc_port id\n", program);
  exit(1);
}

int main(int argc, const char **argv) {
  // Get the arguments.
  if (argc != 5) {
    PrintHelpAndExit(argv[0]);
  }

  const uint32_t port = atoi(argv[1]);
  const char *kdc_address = argv[2];
  const uint32_t kdc_port = atoi(argv[3]);
  const uint8_t id = atoi(argv[4]);

  // Create the server.
  ::hw1::key_exchange::ServerNode server(kdc_address, kdc_port, id);

  if (!server.Listen(port)) {
    return 1;
  }

  while (true) {
    server.HandleConnection();
  }

  return 0;
}
