# Genetic Marker Search

This is a project we completed as a part of our "Parallel Programming in C++" course at Ukrainian Catholic University. 

We solve a problem of finding whether any of a given set of genetic markers are present in given genome sequences. In fact, this is an application of string matching problem, so it can be solved using a classic algorithm like [Aho-Corasick](https://en.wikipedia.org/wiki/Aho%E2%80%93Corasick_algorithm).

Our main goal was to implement an efficient parallel implementation, since the project owner has really large amount of genome files and wishes to utilize their multi-core machine. With this in mind, we decided to use CPU-thread-based parallelism. 

The main matching part is designed as a producer-consumer pipeline, with one thread loading the genomes from the drive and storing them into a bounded queue (in order not to run out of memory), and multiple threads performing string matching. For threading, we chose `std::thread` and for efficient communication between threads we use concurrent data structures from Intel's `TBB`. 

## Data

Pseudogenomes data that we used for testing can be found [here](https://1001genomes.org/data/GMI-MPI/releases/v3.1/pseudogenomes/fasta/). We've prepared a script `load_genomes.py` to load the required number of files. To load `NUM_FILES` files and store them at `DEST_DIR`, execute
```bash
./load_genomes.py DEST_DIR --n NUM_FILES
```
Every loaded file is a `.gz` archive containing one multi-FASTA file.

Unfortunately, markers we used are not available for public use, but we provide a `sample_markers.csv` [file](https://gist.github.com/lekhovitsky/72744a626b610d10153d71ee44d0927e) with random markers for basic testing.

## Requirements

The program mostly relies on basic C++ 17 functionality, but uses some third-party libraries. Particularly, `boost` is used for various file manipulations, and Intel's `TBB` for efficient concurrent data structures. Including `TBB` into the project might be tricky on certain systems, so we provide a `FindTBB.cmake` file (not our work) which solves this problem.
