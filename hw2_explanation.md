In this homework, there are three nodes that communicate with each-other: Nodes
A and B, as well as a KDC.

# Implementation Details

These include implementation details for key exchange and generation.

## Needham-Schroeder Implementation

The implementation of the N-S protocol is similar to the vanilla
one. The main change is the addition of a nonce value on message 3. This is in
order to prevent replay attacks. The other change I made was dropping messages 4
and 5 for simplicity. I didn't see this handshake adding too much security.
After all, we know that only node B should be able to even decrypt the session
key, meaning that the posibility of a malicious node B is not an issue.
Similarly, only A should be able to decrypt message 2.

## Diffie-Hellman Implementation

My implementation of Diffie-Hellman is also extremely simple and not
particularly secure. For computational Diffie-Hellman to really work, you need
to use very, very big numbers. Since this project is educational, and I don't
have libraries installed to deal with integers that large, I used a smallish
prime in the 2 million range as my modulo base. I also chose this to be a
so-called "safe" prime, which gives it the useful property that any number
smaller than it has a high probability of having a long cycle length when used
as a generator. This allowed me to choose the generator randomly, being fairly
confident that the cycle length would be large.

Other than that, my implementation is by-the-book. I did, however, feel the need
to implement the square-and-multiply algorithm, so that my exponents were
bounded. Otherwise, my choice of private keys would have been extremely limited
if I wanted to avoid integer overflow.

In order to get the key exchange working, I tacked on some extra steps to the
N-S protocol. When the KDC received the first message, it would check if the
node was unknown, and if it was, it would perform a key exchange. The key
exchange protocol was very simple: Essentially, the KDC would generate and send
an 8-byte public key, and the other node would generate and send back a
corresponding 8-byte public key. Then both entities would use their private key
to establish a master key between the node and the KDC.

# Building and Running

See the base README for instructions on building.

To test the code, first start the KDC:
```
~$ bazel-bin/key_exchange/key_server 6219
```

The last argument is the port for the KDC to listen on.

Next, start the server node:
```
~$ bazel-bin/key_exchange/kdc_server_node 6220 127.0.0.1 6219 1
```

The first argument is the port for the node to listen on, the second is the
address of the KDC, the third is the port to connect to the KDC on, and the last
is a unique node ID value.

Finally, we're ready to run the client:
```
~$ bazel-bin/key_exchange/kdc_client_node 127.0.0.1 6219 0 127.0.0.1 6220 1
```

The first two arguments are the address and port of the KDC, the third argument
is a unique ID value for this node, and the last three arguments are the
address, port, and ID of the server node.

Assuming this all works, the client will prompt you for a message. Type
something in and press enter, and the server will display your message.
