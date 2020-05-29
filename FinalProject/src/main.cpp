#include <string>
#include <iostream>

#include "utils.h"
#include "aho_corasick.h"
#include <chrono>
#include <iomanip>

typedef std::chrono::duration<float> float_seconds;


int main(int argc, char **argv) {
    // auto fastas = read_fasta("../f.txt");
    //std::vector<std::string> patterns = {"he", "she", "hers", "his"};
    //std::string text = "ahishers";

    size_t max_rows {1000000};
    auto read_start = std::chrono::high_resolution_clock::now();
    auto markers = read_markers("../markers.csv", max_rows);
    auto read_end = std::chrono::high_resolution_clock::now();
    auto read_time = std::chrono::duration_cast<float_seconds>(read_end - read_start);

    AhoCorasick ac;

    auto build_start = std::chrono::high_resolution_clock::now();
    ac.set_patterns(markers);
    auto build_end = std::chrono::high_resolution_clock::now();
    auto build_time = std::chrono::duration_cast<float_seconds>(build_end - build_start);

    std:: cout
            << std::setprecision(3) << std::fixed
            << "Reading: " << read_time.count() << " seconds\n"
            << "Building trie: " << build_time.count() << " seconds\n";

    ac.match(markers[8]+markers[2]+markers[1]);

    return 0;
}
