#include "utils.h"
#include "config.h"
#include "program.h"
#include <thread>
#include <iomanip>
#include <iostream>

Program::Program(
        size_t      num_matcher_threads,
        size_t      max_queue_size,
        std::string result_file,
        std::string markers_file,
        std::string genomes_path,
        bool        verbose)
        : _ac{}
        , _q{}
        , _m{}
        , _result_file{std::move(result_file)}
        , _markers_file{std::move(markers_file)}
        , _reader{std::move(genomes_path), _q}
        , _matchers{}
        , _verbose{verbose}
{
    _q.set_capacity(max_queue_size);
    for (size_t i = 0; i < num_matcher_threads; ++i)
        _matchers.emplace_back(_ac, _q, _m);
}

void Program::run() {
    //
    // prepare markers and AC; maybe extract method?
    //
    auto m_read_start = std::chrono::high_resolution_clock::now();
    // TODO: remove this 100
    auto markers = read_markers(_markers_file, 100);
    auto m_read_end = std::chrono::high_resolution_clock::now();

    std::vector<std::string> m_ids {}, patterns;
    for (const auto& m : markers) {
        m_ids.push_back(m._id);
        patterns.push_back(m._marker);
    }

    auto build_start = std::chrono::high_resolution_clock::now();
    _ac.set_patterns(patterns);
    auto build_end = std::chrono::high_resolution_clock::now();

    //
    // launch parallel execution
    //
    // launch reader; maybe extract method?
    auto s_read_start = std::chrono::high_resolution_clock::now();
    std::thread reader_thread(&SequenceReader::run, &_reader);

    // launch matchers; maybe extract method?
    std::vector<std::thread> matcher_threads;
    auto match_start = std::chrono::high_resolution_clock::now();
    for (auto& m : _matchers)
        matcher_threads.emplace_back(&SequenceMatcher::run, &m);

    // wait for threads to finish
    reader_thread.join();
    auto s_read_end = std::chrono::high_resolution_clock::now();

    for (auto& t : matcher_threads)
        t.join();
    auto match_end = std::chrono::high_resolution_clock::now();

    //
    // save results to csv; maybe extract method?
    //
    std::vector<std::string> s_ids;
    std::vector<std::vector<bool>> result;
    for (auto& [id, res] : _m) {
        s_ids.push_back(id);
        result.push_back(res);
    }
    auto write_start = std::chrono::high_resolution_clock::now();
    write_result(_result_file, result, s_ids, m_ids);
    auto write_end = std::chrono::high_resolution_clock::now();

    if (_verbose) {
        std::cout
                << std::setprecision(3) << std::fixed
                << "Reading markers:   " << time_diff(m_read_start, m_read_end) << " seconds\n"
                << "Building trie:     " << time_diff( build_start,  build_end) << " seconds\n"
                << "Reading sequences: " << time_diff(s_read_start, s_read_end) << " seconds\n"
                << "Matching patterns: " << time_diff( match_start,  match_end) << " seconds\n"
                << "Writing results:   " << time_diff( write_start,  write_end) << " seconds\n";
    }
}
