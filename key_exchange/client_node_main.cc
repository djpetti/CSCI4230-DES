#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

#include "client_node.h"

// The node ID for us to use.
const uint8_t kNodeId = 0;

// Shows the help message and exits the program.
// Args:
//  program: The name of the program.
void PrintHelpAndExit(const char *program) {
  printf("Usage: %s kdc_server kdc_port key server port id\n", program);
  printf("(Keys must be 2 characters long.)\n");
  exit(1);
}

int main(int argc, const char **argv) {
  // Get the arguments.
  if (argc != 7) {
    PrintHelpAndExit(argv[0]);
  }

  const char *kdc_server = argv[1];
  const uint32_t kdc_port = atoi(argv[2]);
  const char *key = argv[3];
  const char *dest_server = argv[4];
  const uint32_t dest_port = atoi(argv[5]);
  const uint8_t dest_id = atoi(argv[6]);
  if (strlen(key) != 2) {
    // Key is not the right length.
    PrintHelpAndExit(argv[0]);
  }

  // Create the client.
  ::hw1::key_exchange::ClientNode client(kdc_server, kdc_port,
                                         (const uint8_t *)key, kNodeId);

  // Add the endpoint.
  client.AddNodeWithId(dest_server, dest_id);

  if (!client.Connect(dest_server, dest_port)) {
    return 1;
  }

  // Send messages to the client.
  char *message = new char[255];
  while (true) {
    printf("Message > ");
    fflush(stdout);

    ::std::cin.getline(message, 255);
    client.SendMessage(message);
  }

  return 0;
}
