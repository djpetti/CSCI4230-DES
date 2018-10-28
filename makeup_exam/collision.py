#!/usr/bin/python


import argparse
import binascii
import random


def pad_with_null(message, padding):
  """ Pads a message with null characters.
  Args:
    padding: Number to add on the front an back.
  Returns:
    Padded version of the message. """
  num_front, num_back = padding
  return "\0" * num_front + message + "\0" * num_back

def find_match(good_message, bad_message):
  """ Makes the hash of the bad message match that of the good one.
  Args:
    good_message: The good message we want to match.
    bad_message: The bad message we want to make match.
  Returns:
    Variations of the good and bad messages that have the same hash. """
  # Generate variations of the good message hash.
  print "Generating good hashes... """
  good_hashes = {}
  front_padding = 0
  back_padding = 0
  good_var = good_message
  for i in range(0, 65536):
    good_hash = binascii.crc32(good_var)
    # We only save the number of padding characters to reduce memory.
    good_hashes[good_hash] = (front_padding, back_padding)

    if random.randint(0, 1):
      good_var += "\0"
      back_padding += 1
    else:
      good_var = "\0" + good_var
      front_padding += 1

  print "Generating bad hashes..."
  bad_hash = binascii.crc32(bad_message)
  padding = 0
  bad_var = bad_message
  while bad_hash not in good_hashes:
    char = random.choice(["\0", "\r"])
    if random.randint(0, 1):
      bad_var += char
    else:
      bad_var = char + bad_var
    padding += 1

    bad_hash = binascii.crc32(bad_var)

    if not padding % 1000:
      print "Tried %d variations." % padding

  good_match = pad_with_null(good_message, good_hashes[bad_hash])
  return good_match, bad_var

def main():
  parser = argparse.ArgumentParser("Find a 32-bit hash collision.")
  parser.add_argument("legit_text",
                      help="The text we want to match the hash of.")
  parser.add_argument("bad_text",
                      help="The fradulent text that we want to match.")
  args = parser.parse_args()

  # Perform the attack.
  good_var, bad_var = find_match(args.legit_text, args.bad_text)

  print "Good variation hash: %d" % binascii.crc32(good_var)
  print "Bad variation hash: %d" % binascii.crc32(bad_var)

  # Save them to files.
  file("good_message.txt", "w").write(good_var)
  file("bad_message.txt", "w").write(bad_var)

if __name__ == "__main__":
  main()
