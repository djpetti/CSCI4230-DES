# Make-Up Exam Explanation

## Hash Functions

The problem did not explicitly specify which hash function to use. Arguably,
in the real world, these kinds of attacks would mostly be deployed against
cryptographic hashes such as MD5 and SHA1. For this implementation, however,
even MD5 would have required me to generate 2^80 messages, which was not
particularly feasible. Therefore, for this assigment, I used CRC32 instead. This
is not a cryptographic hash, and not intended to be secure against deliberate
attacks, but it is only 32 bits, and thus requires only the generation of 2^16
messages, which is much easier.

### Implementation

My implementation is in the ```collision.py``` file. It can be run with two
arguments.

```
~$ ./collision.py good_text bad_text
```

As the names suggest, good_text is the text that you are trying to match the
hash of. The second argument is the fraudulent text that you want to substitute.
The program will first generate and save 2^16 variations of good_text. It will
then generate variations of bad_text until one matches, which generally occurs
fairly rapidly. When it is done, the program will save the resulting messages
into the files "good_message.txt" and "bad_message.txt". The contents of these
files can be read and printed in Python, and will appear the same as the
original inputs. However, hashing them with CRC32 will confirm that they do,
indeed, have the same hash.

Included are the "good_message.txt" and "bad_message.txt" files produced when
running the program on the text given in the assignment.

## Elliptic Curves and ECC

I got almost full points on the ECC part of the original test, so I didn't
bother to do this problem.

## Primality and Factorization

I first implemented the Miller-Rabin algorithm for this assignment. Using it, I
confirmed that all the numbers given were prime except for the one that was
obviously even. (I used a threshold probability of 0.99999.)

For the second part of the problem, I implemented the Pollard-Rho algorithm, and
used it to factor 520482. This yielded the prime factors 2, 3, 223, and 389.
