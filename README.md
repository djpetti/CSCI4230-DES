# Toy DES demonstration for CSCI 4230

This repository implements a toy version of the DES algorithm that encrypts 8-bit
blocks with 10-bit keys and performs a total of two rounds. It is not
particularly secure, but it is useful for educational purposes.

A demonstration application is provided that uses the DES implementation to
"securely" send a file over a TCP socket.

# Running the Code

To build the code, use Bazel:

```
~$ bazel build //transfer:server //transfer:client
```

All the executables will be in the `bazel-bin/transfer` directory.

To run the code, first start a server:

```
~$ ./server 1337 ky
```

The server takes two arguments. The first is the port to listen on, and the
second is the two-character encryption key to use for received files.

Next, we can use a client to send a file to the server.

```
~$ ./client 127.0.0.1 1337 ky secret_furry_porn.rar
```

The client takes four arguments. The first is the IP address of the server, the
second is the server port, the third is the two-character encryption key to
use, and the fourth is the name of the file to send. The encryption key *must
match* the key used by the server.

If all goes well, you should see some messages from the server, and a file by
the same name as the one sent will be written to the server machine.

# Explanation of the Code

The implemenation of the code is fairly simple. The server can only handle a
single connection at a time. The client connects to the server, and
encrypts/sends the file data block-by-block. The server uses the same process on
the other end to decrypt the data.

The code uses a simple ECB approach for sending arbitrary-length data. This is
not very secure, but it is easy to implement.

Additionally, a single fixed-length header is exchanged by the server and client
before the actual transfer starts. The header contains the name of the file, and
the size of the file. It is not encrypted.

# Explanation of the Algorithm

The algorithm used is a 2-round Feistel cipher that operates on blocks of a
single byte. The round function uses 2 S-boxes which take a 4-bit input and
produce a 4-bit output. Each subkey is 8-bits, and generated each round from a
10-bit master key. Decryption is implemented as encryption in reverse, with the
subkeys applied in the opposite order.
