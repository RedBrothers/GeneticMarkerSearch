#include <string>
#include <iostream>
#include "utils.h"
#include "aho_corasick.h"


int main(int argc, char **argv) {
    // auto fastas = read_fasta("../f.txt");

    std::vector<std::string> patterns = {"he", "she", "hers", "his"};
    std::string text = "ahishers";

    AhoCorasick ac;

    ac.set_patterns(patterns);
    ac.match(text);

    return 0;
}
