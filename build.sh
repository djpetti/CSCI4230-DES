#!/bin/bash

set -e

CC="g++ -Wall -Werror -g"

$CC -c -o p_box.o p_box.cc
$CC -c -o s_box.o s_box.cc
$CC -c -o expansion.o expansion.cc
$CC -c -o des_s_box.o des_s_box.cc
$CC -c -o key_gen.o key_gen.cc
$CC -c -o f_function.o f_function.cc
$CC -c -o feistel_cipher.o feistel_cipher.cc
$CC -c -o des.o des.cc
$CC -c -o try_des.o try_des.cc

$CC -o try_des *.o

rm -rf *.o
