#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include "utils.h"
#include "aho_corasick.h"

#include "tbb/concurrent_queue.h"

typedef std::chrono::duration<float> float_seconds;


template<typename Time>
auto time_diff(Time t1, Time t2) {
    return std::chrono::duration_cast<float_seconds>(t2 - t1).count();
}

int main(int argc, char **argv) {

    size_t max_rows {10};
    auto m_read_start = std::chrono::high_resolution_clock::now();
    auto m_records = read_markers("../markers.csv", max_rows);
    auto m_read_end = std::chrono::high_resolution_clock::now();

    AhoCorasick ac;
    std::vector<std::string> marker_ids {};
    std::vector<std::string> markers {};
    for (const auto& rec : m_records) {
        marker_ids.push_back(rec._id);
        markers.push_back(rec._marker);
    }

    auto build_start = std::chrono::high_resolution_clock::now();
    ac.set_patterns(markers);
    auto build_end = std::chrono::high_resolution_clock::now();

    auto s_read_start = std::chrono::high_resolution_clock::now();
    auto sequences = read_fasta_file("../sequence.fasta");
    auto s_read_end = std::chrono::high_resolution_clock::now();

    std::vector<std::string> sequence_ids {};
    std::vector<std::vector<bool>> results {};

    auto match_start = std::chrono::high_resolution_clock::now();
    for (const auto& seq : sequences) {
        sequence_ids.push_back(seq._id);
        results.push_back(ac.match(seq._sequence));
    }
    auto match_end = std::chrono::high_resolution_clock::now();

    auto write_start = std::chrono::high_resolution_clock::now();
    write_result("../result.csv", results, sequence_ids, marker_ids);
    auto write_end = std::chrono::high_resolution_clock::now();

    std:: cout
            << std::setprecision(3) << std::fixed
            << "Reading markers:   " << time_diff(m_read_start, m_read_end) << " seconds\n"
            << "Building trie:     " << time_diff( build_start,  build_end) << " seconds\n"
            << "Reading sequences: " << time_diff(s_read_start, s_read_end) << " seconds\n"
            << "Matching patterns: " << time_diff( match_start,  match_end) << " seconds\n"
            << "Writing results:   " << time_diff( write_start,  write_end) << " seconds\n";

    return 0;
}
