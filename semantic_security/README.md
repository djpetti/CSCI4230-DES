# Semantic Security

My implementation of the Blum-Goldwasser encryption algorithm is contained here.
I wrote a test (contained in ```tests/test_blum_goldwasser.cc```) that performs
the encryption on the given plaintext, and then decrypts it again and checks
that the decryption matches. You can run the test as follows:

```
~$ bazel test --test_output=all ...
```

It can be seen that the test passes. Furthermore, it prints out the ciphertext
and the final output from the RNG, which in this case are 0x57 0xFD 0xC8 and
94913 respectively. (Note that I padded the given plaintext with an extra 4 bits
of zeros in order to align it to the byte boundary.)
