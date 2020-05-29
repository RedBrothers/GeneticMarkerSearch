#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include "utils.h"
#include "aho_corasick.h"

#include "tbb/concurrent_queue.h"

typedef std::chrono::duration<float> float_seconds;


int main(int argc, char **argv) {
    tbb::concurrent_bounded_queue<std::string> q;
    q.set_capacity(1);
    q.push("1");
    std::string out;
    q.pop(out);
    std::cout << out << std::endl << std::endl;

    size_t max_rows {10};
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
            << "Building trie: " << build_time.count() << " seconds\n\n";

    auto matches = ac.match(markers[8]+markers[2]+markers[1]);
    for (auto i = 0; i < markers.size(); ++i) {
        std::cout << "Marker: " << markers[i] << " : " << matches[i] << std::endl;
    }

    return 0;
}
