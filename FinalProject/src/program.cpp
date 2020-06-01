#include "utils.h"
#include "program.h"
#include <thread>
#include <cassert>
#include <iomanip>
#include <iostream>

Program::Program(
        size_t      num_threads,
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
    assert(num_threads >= 2);
    _q.set_capacity(max_queue_size);
    _matchers.reserve(num_threads);
    for (size_t i = 0; i < num_threads - 1; ++i)
        _matchers.emplace_back(_ac, _q, _m);
}

void Program::run() {
    //
    // prepare markers and AC; maybe extract method?
    //
    auto m_read_start = Time::now();
    auto markers = read_markers(_markers_file);
    auto m_read_end = Time::now();

    std::vector<std::string> m_ids, patterns;
    m_ids.reserve(markers.size());
    patterns.reserve(markers.size());
    for (auto &&m : markers) {
        m_ids.push_back(std::move(m._id));
        patterns.push_back(std::move(m._marker));
    }
    markers.clear();

    auto build_start = Time::now();
    _ac.set(std::move(patterns));
    auto build_end = Time::now();

    //
    // launch parallel execution
    //
    // launch reader; maybe extract method?
    auto s_read_start = Time::now();
    std::thread reader_thread(&SequenceReader::run, &_reader);

    // launch matchers; maybe extract method?
    std::vector<std::thread> matcher_threads;
    matcher_threads.reserve(_matchers.size() + 1);
    auto match_start = Time::now();
    for (auto& m : _matchers)
        matcher_threads.emplace_back(&SequenceMatcher::run, &m);

    // wait for threads to finish
    reader_thread.join();
    auto s_read_end = Time::now();

    // redirect the new free thread
    _matchers.emplace_back(_ac, _q, _m);
    matcher_threads.emplace_back(&SequenceMatcher::run, &_matchers.back());

    for (auto& t : matcher_threads)
        t.join();
    auto match_end = Time::now();

    //
    // save results to csv; maybe extract method?
    //
    std::vector<std::string> s_ids;
    s_ids.reserve(_m.size());
    std::vector<std::vector<bool>> result;
    result.reserve(_m.size());
    for (auto&& [id, res] : _m) {
        s_ids.push_back(std::move(id));
        result.push_back(std::move(res));
    }
    _m.clear();

    auto write_start = Time::now();
    write_result(_result_file, result, s_ids, m_ids);
    auto write_end = Time::now();

    if (_verbose) {
        std::cout
                << std::setprecision(3) << std::fixed
                << "Reading markers:   " << Time::diff(m_read_start, m_read_end) << " seconds\n"
                << "Building trie:     " << Time::diff( build_start,  build_end) << " seconds\n"
                << "Reading sequences: " << Time::diff(s_read_start, s_read_end) << " seconds\n"
                << "Matching patterns: " << Time::diff( match_start,  match_end) << " seconds\n"
                << "Writing results:   " << Time::diff( write_start,  write_end) << " seconds\n";
    }
}
