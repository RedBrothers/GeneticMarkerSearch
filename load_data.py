import requests
import argparse

from os import path
from bs4 import BeautifulSoup

GENOMES_URL = 'https://1001genomes.org/data/GMI-MPI/releases/v3.1/pseudogenomes/fasta/'
EXT = '.fasta.gz'

def listFD(url, ext=''):
    page = requests.get(url).text
    soup = BeautifulSoup(page, 'html.parser')
    
    return [url + '/' + node.get('href') for node in soup.find_all('a') if node.get('href').endswith(ext)]

def load_n_genomes(n, out_dir):
    genom_paths = listFD(GENOMES_URL, EXT)
    
    for i, gp in enumerate(genom_paths):
        r = requests.get(gp)
        genom_name = genom_paths[i].split('/')[-1]
        if r.ok and not path.exists(out_dir + genom_name):
            with open(out_dir + genom_name, 'wb') as f:
                f.write(r.content)
            n -= 1
        if n == 0:
            break

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
            '--n',
            action='store',
            dest='n',
            default=1,
            type=int
    )

    parser.add_argument(
            '--out-dir', 
            action='store', 
            dest='out_dir',
            default='./'
    )

    args = parser.parse_args()
    load_n_genomes(n=args.n, out_dir=args.out_dir)




if __name__ == '__main__':
    main()
