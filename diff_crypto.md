# Differential Cryptographic Analysis

Differential cryptoanalysis can be used relatively easily to break this simple
algorithm. In my case, I focused on the S-Box S0. I first created
[a table](https://docs.google.com/spreadsheets/d/1cvm0ePpSBWJqbT-oG4JUcJZmFEOcQm2QxfkuvRh5M-Q/edit?usp=sharing)
of the frequencies of each combination of input and output pair XORs. (This
table was produced automatically using a spreadsheet in which I analysed every
possible combination of 4-bit inputs.)

Once I had the table, I created
[a simple program](https://github.com/djpetti/CSCI4230-DES/tree/master/analysis)
which facilitated the search for suitable input pairs which satisfied a
particular input XOR to output XOR relationship. The tricky part of this
was simply isolating the part of the algorithm that XORed the key with
the input, and fed it through S0. I had to correctly undo a variety of
permutations and splitting operations, but I made it work.

I focused my effort on the pairs with both and input and output XOR of
0x1. Using my tool, I found that the plaintext 0x20 satisfied this
requirement. Using my table, I determined that there was a single
possible pair of inputs to the S-Box: 0xE and 0xF. Knowing the plaintext, I
determined that the possible inputs to the S-Box **before** the key was XORed in
were 0x4 and 0x5. Consequenly, I was able to determine that the left 4 bits of
the first round subkey were 0xA. I checked the actual algorithm and verified
that this was correct.

In order to fully break the cipher, I would have to repeat the same analysis on
S1 in order to figure out the full subkey. I would then have to repeat the same
analysis on the second round in order to determine the second subkey.
