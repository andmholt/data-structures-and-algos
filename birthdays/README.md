# Birthdays

Birthdays uses a self-implemented hashtable with quadratic probing to simulate the collection of randomly surveyed birthdates until a collision is found (when two people have the same birthday). 

Interestingly enough, one will find that with a correct implementation and large enough data set to ensure accuracy, precisely 50.7% of the time a collision will occur in 23 birthdays or less.

### Creating the binaries
make creates two binaries: `birthdays` and `wordGen`

Since both the hashtable and word generator for random birthday simulator run on randomness, I split the birthdays test program and word generator program into two separate executables, so that the random seeds will not interfere with each other.

### birthdays
`birthdays` is the main binary which runs the birthday tests with the hashtable:

`./birthdays NUM_TESTS INPUT_FILE`

Where `NUM_TESTS` is the number of tests you would like to run, and `INPUT_FILE` is the file the program will use as input for the words to run the tests.

### wordGen
`wordGen` is a secondary binary which may be used to generate randomized 30-char strings and ints in order to carry out the birthday tests:

`./wordGen NUM_WORDS OUTPUT_FILE`

where `NUM_WORDS` is the number of random words to be generated and `OUTPUT_FILE` is the file the words will be outputted to

I have already included `words.txt`, which is a file containing 30k words, for user convenience (word generation can take awhile).