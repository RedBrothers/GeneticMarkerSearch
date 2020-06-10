# Genetic Marker Search

This is a project we completed as a part of our "Parallel Programming in C++" course at Ukrainian Catholic University. 

## Data

Pseudogenomes data that we used for testing can be found [here](https://1001genomes.org/data/GMI-MPI/releases/v3.1/pseudogenomes/fasta/). We've prepared a script `load_genomes.py` to load the required number of files. To load `NUM_FILES` files and store them at `DEST_DIR`, execute
```bash
./load_genomes.py DEST_DIR --n NUM_FILES
```
Every loaded file is a `.gz` archive containing one multi-FASTA file.

Unfortunately, markers we used are not available for public use, but we provide a `sample_markers.csv` file with random markers for basic testing.
