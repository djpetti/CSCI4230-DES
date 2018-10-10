#include "key_manager.h"

#include <stdio.h>
#include <string.h>

#include "constants.h"

namespace hw1 {
namespace key_exchange {
namespace {

// All of our communication happens unencrypted, so we set a dummy key for the
// underlying client.
const uint8_t kDummyKey[] = {0, 0};

}

KeyManager::KeyManager(const char *kdc_address, const uint16_t port, uint8_t id)
    : transfer::common::Client(kDummyKey, 8),
      kdc_address_(kdc_address),
      kdc_port_(port),
      id_(id) {}

bool KeyManager::GetMasterKey(uint8_t *key) {
  // Perform the key exhange if necessary.
  if (!EnsureKeyExchanged()) {
    return false;
  }
  // Set the key.
  memcpy(key, kdc_key_, 2);

  return true;
}

bool KeyManager::EnsureKeyExchanged() {
  if (set_key_) {
    // Already have key.
    return true;
  }

  // First, connect to the KDC.
  if (!Connect(kdc_address_, kdc_port_)) {
    // Failed to connect to the KDC.
    return false;
  }

  // Send the initial key request.
  if (!SendKeyRequest()) {
    return false;
  }

  // Perform the key exchange.
  if (!DoKeyExchange()) {
    return false;
  }

  // Disconnect.
  Close();

  return true;
}

bool KeyManager::SendKeyRequest() {
  // Format a request for a session key.
  const uint32_t nonce = nonce_generator_.Generate();
  char *write_at = plain_chunk_buffer_;
  memcpy(write_at++, &id_, 1);
  // Set the second ID field to be the same. This indicates the the KDC that we
  // only want to exchange keys.
  memcpy(write_at++, &id_, 1);
  memcpy(write_at, &nonce, 4);
  write_at += 4;

  printf("Sending initial message from %u with nonce %u.\n", id_, nonce);

  // Send the request.
  if (!SendChunk(plain_chunk_buffer_, write_at - plain_chunk_buffer_)) {
    return false;
  }

  return true;
}

bool KeyManager::DoKeyExchange() {
  printf("Performing key exchange with KDC.\n");

  // First, generate a private key.
  const uint64_t private_key = key_gen_.GeneratePrivateKey();
  // Compute the corresponding public key.
  const uint64_t public_key = key_gen_.ComputePublicKey(private_key);
  printf("Computed public key: %lu\n", public_key);

  // Receive the public key from the KDC.
  char *buffer;
  if (!ReceiveChunk(&buffer, 8)) {
    return false;
  }
  uint64_t other_public_key;
  memcpy((uint8_t *)&other_public_key, buffer, 8);
  printf("Received public key: %lu\n", other_public_key);

  // Send our public key to the KDC.
  if (!SendChunk((const char *)&public_key, 8)) {
    return false;
  }

  // Generate the master key.
  uint64_t session_key =
      key_gen_.ComputeSessionKey(other_public_key, private_key);
  // Truncate to 10 bits for use in DES.
  kdc_key_[0] = session_key & 0xFF;
  kdc_key_[1] = (session_key >> 8) & 0x03;
  printf("Computed master key: 0x%X 0x%X\n", kdc_key_[0], kdc_key_[1]);

  set_key_ = true;
  return true;
}


}  // namespace key_exchange
}  // namespace hw1
