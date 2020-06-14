#include <string>
#include <vector>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <algorithm>

#include <utils.hpp>
#include <aho_corasick.hpp>


double mean(const std::vector<double>& v) {
    if (v.empty()) return 0.;
    return std::accumulate(v.cbegin(), v.cend(), 0.) / v.size();
}


void test_build_and_match(size_t num_markers) {
    auto markers = read_markers("../../data/markers.csv", num_markers);
    auto sequences = read_fasta_archive("../../data/pseudo10001.fasta.gz");

    aho_corasick::trie trie;
    std::cout << "Number of markers: " << num_markers << std::endl;
    auto t1 = Time::now();
    for (const auto &m : markers)
        trie.insert(m);
    trie.finalize();
    auto t2 = Time::now();
    std::cout << "Trie built in "
        << std::setprecision(3) << std::fixed
        << Time::diff(t1, t2) << " seconds" << std::endl;

    std::vector<double> times;
    for (const auto &s : sequences) {
        auto t3 = Time::now();
        auto result = trie.parse_text(s.seq);
        auto t4 = Time::now();
        auto it = std::unique(result.begin(), result.end());
        times.push_back(Time::diff(t3, t4));
        std::cout
                << std::setprecision(1) << std::fixed
                << s.id
                << " matched in " << times.back() << " seconds, "
                << "found " << std::distance(result.begin(), it) << " matches." <<  std::endl;
    }
    std::cout
            << std::setprecision(1) << std::fixed
            << "Average matching time: " << mean(times) << " seconds." << std::endl;
    std::cout << std::endl;
}


int main() {
    for (size_t n : {1000, 10000, 100000, 1000000})
        test_build_and_match(n);
    return 0;
}
