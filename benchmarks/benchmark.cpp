#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>

#include <utils.hpp>
#include <aho_corasick.hpp>


void test_build_and_match(size_t num_markers) {
    auto markers = read_markers("../../data/markers.csv", num_markers);
    auto sequences = read_fasta_archive("../../data/pseudo10001.fasta.gz");

    aho_corasick::trie trie;
    std::cout << "Number of markers: " << num_markers << std::endl;
    auto t1 = Time::now();
    for (auto& m : markers)
        trie.insert(m._marker);
    auto t2 = Time::now();
    std::cout << "Trie built in " << Time::diff(t1, t2) << " seconds" << std::endl;

    for (const auto &s : sequences) {
        auto t3 = Time::now();
        auto result = trie.parse_text(s._sequence);
        auto t4 = Time::now();
        auto it = std::unique(result.begin(), result.end());
        std::cout
                << s._id
                << " matched in " << Time::diff(t3, t4) << " seconds, "
                << "found " << std::distance(result.begin(), it) << " matches." <<  std::endl;
    }

    std::cout << std::endl;
}


int main() {
    for (size_t n : {1000, 10000, 100000, 1000000})
        test_build_and_match(n);
    return 0;
}