#!/usr/bin/env python3
import numpy as np
import pandas as pd
from Bio import SeqIO
from datetime import datetime
from ahocorapy.keywordtree import KeywordTree


for n_markers in (1_000, 10_000, 100_000, 1_000_000,):
    markers = pd.read_csv("./markers.csv", header=None, index_col=0, nrows=n_markers)

    print(f"Number of markers: {n_markers}")

    trie = KeywordTree()
    start = datetime.now()
    for marker in markers.values.flatten():
        trie.add(marker)
    trie.finalize()
    end = datetime.now()
    print(f"Trie built in {(end-start).total_seconds():.1f} seconds")

    times = []
    for f in SeqIO.parse("./pseudo10001.fasta", "fasta"):
        start = datetime.now()
        res = trie.search_all(f.seq)
        n_matches = len(list(res))
        end = datetime.now()
        times.append((end - start).total_seconds())
        print(f"{f.id} : {n_matches:6} matches : {times[-1]:5.1f} seconds")
    
    print(f"Average matching time: {np.mean(times):5.1f} seconds")
    print("")
