# Monorepo for CSCI 4230

If you are looking for homework 3,
[here](https://github.com/djpetti/CSCI4230-DES/blob/master/semantic_security/)
is a link to it.

If you are looking for the makeup exam answers,
[here](https://github.com/djpetti/CSCI4230-DES/blob/master/makeup_exam/README.md)
is a link to them.

If you are looking for homework 2,
[here](https://github.com/djpetti/CSCI4230-DES/blob/master/hw2_explanation.md)
is a link to it.

This repository implements a toy version of the DES algorithm that encrypts 8-bit
blocks with 10-bit keys and performs a total of two rounds. It is not
particularly secure, but it is useful for educational purposes.

A demonstration application is provided that uses the DES implementation to
"securely" send a file over a TCP socket.

# Running the Code

To build the code, use Bazel:

```
~$ bazel build ...
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
single TCP connection at a time. The client connects to the server, and
encrypts/sends the file data block-by-block. The server uses the same process on
the other end to decrypt the data.

The code uses a simple ECB approach for sending arbitrary-length data. This is
not very secure, but it is easy to implement.

Additionally, a single fixed-length header is exchanged by the server and client
before the actual transfer starts. The header contains the name of the file, and
the size of the file. It is not encrypted.

# Explanation of the Algorithm

The algorithm used is a 2-round Feistel cipher that operates on blocks of a
single byte. The round function uses 2 S-boxes which each take a 4-bit input and
produce a 2-bit output. Each subkey is 8-bits, and generated each round from a
10-bit master key. Decryption is implemented as encryption in reverse, with the
subkeys applied in the opposite order.

# Additional Analysis

I have performed differential cryptoanalysis on this algorithm, the results of
which are available
[here](https://github.com/djpetti/CSCI4230-DES/blob/master/diff_crypto.md).

Additionally, I have a solution to the last problem on the homework
[here](https://github.com/djpetti/CSCI4230-DES/blob/master/entropy.md).

# Homework 2

- [Here](https://www.evernote.com/shard/s625/sh/bcd5790a-d864-42da-a57c-937c967a752f/77aec4c45481d9a320d04d1c0a3e5b29)
is a link to my second homework solutions.

- [Here](https://www.evernote.com/shard/s625/sh/0dbe2e4e-4c5c-43b4-9ad1-4e11d2bb12b6/d022c3aa4e37ca3651a17b7b450fe62b)
  is a link to part 2 of my second homework solutions.
