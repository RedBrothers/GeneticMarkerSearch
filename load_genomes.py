#!/usr/bin/env python3
import os
import requests
import argparse

from tqdm import tqdm
from bs4 import BeautifulSoup


GENOMES_URL = 'https://1001genomes.org/data/GMI-MPI/releases/v3.1/pseudogenomes/fasta/'
EXTENSION = '.fasta.gz'


def list_genome_urls(genomes_url: str, extension: str = '') -> list:
    page = requests.get(genomes_url).text
    soup = BeautifulSoup(page, 'html.parser')
    return [genomes_url + '/' + node.get('href')
            for node in soup.find_all('a') 
            if node.get('href').endswith(extension)]


def load_genomes(dest: str, n: int = 0):
    urls = list_genome_urls(GENOMES_URL, EXTENSION)
    n = min(len(urls), n) if n > 0 else len(urls)

    if not os.path.exists(dest):
        print(f"Path {dest} doesn't exist. Creating...")
        os.makedirs(dest)

    for url in tqdm(urls[:n]):
        p = os.path.join(dest, url.split('/')[-1])
        if os.path.exists(p):
            continue

        r = requests.get(url)
        if r.ok:
            with open(p, 'wb') as f:
                f.write(r.content)

    print(f"{n} genomes successfully loaded and stored at {dest}")


def main():
    parser = argparse.ArgumentParser("Load pseudogenomes from host server (1001genomes.org).")
    parser.add_argument(
        'dest',
        default='./',
        help='Destination folder. Default ./'
    )
    parser.add_argument(
        '--n',
        default=0,
        type=int,
        help='Number of files to load. Set to non-positive number to load all genomes. Default 0'
    )

    args = parser.parse_args()
    load_genomes(args.dest, args.n)


if __name__ == '__main__':
    main()
