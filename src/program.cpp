#include <utils.hpp>
#include <program.hpp>
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
        , _e{}
        , _result_file{std::move(result_file)}
        , _markers_file{std::move(markers_file)}
        , _reader{std::move(genomes_path), _q, _e}
        , _matchers{}
        , _verbose{verbose} {
    assert(num_threads >= 2);
    _q.set_capacity(max_queue_size);
    _matchers.reserve(num_threads);
    for (size_t i = 0; i < num_threads - 1; ++i)
        _matchers.emplace_back(_ac, _q, _m, _e);
}


void Program::run() {
    prepare();
    execute();
    save();
    report();
    cleanup();
}


void Program::prepare() {
    // read markers
    auto read_start = Time::now();
    _markers = read_markers(_markers_file);
    auto read_end = Time::now();
    _markers_reading_time = Time::diff(read_start, read_end);

    // prepare Aho-Corasick trie
    auto build_start = Time::now();
    for (const auto &m : _markers)
        _ac.insert(m);
    _ac.finalize();
    auto build_end = Time::now();
    _trie_building_time = Time::diff(build_start, build_end);
}


void Program::execute() {
    // launch reader
    auto read_start = Time::now();
    std::thread reader_thread(&SequenceReader::run, &_reader);

    // launch matchers
    std::vector<std::thread> matcher_threads;
    matcher_threads.reserve(_matchers.size() + 1);
    auto match_start = Time::now();
    for (auto& m : _matchers)
        matcher_threads.emplace_back(&SequenceMatcher::run, &m);

    // wait for reader thread to finish
    reader_thread.join();
    auto read_end = Time::now();
    _genomes_reading_time = Time::diff(read_start, read_end);

    // redirect the new free thread
    _matchers.emplace_back(_ac, _q, _m, _e);
    matcher_threads.emplace_back(&SequenceMatcher::run, &_matchers.back());

    // wait for matcher threads to finish
    for (auto& t : matcher_threads)
        t.join();
    auto match_end = Time::now();
    _genomes_matching_time = Time::diff(match_start, match_end);
}


void Program::save() {
    // extract results from our map
    std::vector<std::string> seq_ids;
    seq_ids.reserve(_m.size());
    std::vector<std::vector<bool>> result;
    result.reserve(_m.size());
    for (auto &&[id, res] : _m) {
        seq_ids.push_back(id);
        std::vector<bool> v(_markers.size(), false);
        for (auto idx : res)
            v[idx] = true;
        result.push_back(std::move(v));
    }

    // save results
    auto write_start = Time::now();
    write_result(_result_file, result, seq_ids, _markers);
    auto write_end = Time::now();
    _results_saving_time = Time::diff(write_start, write_end);

    _num_genomes = seq_ids.size();
    _num_markers = _markers.size();
}


void Program::report() const {
    if (!_verbose) return;
    std::cout
            << "Successfully executed for "
            << _num_genomes << " genomes and "
            << _num_markers << " markers.\n"
            << "Results saved at " << _result_file << ".\n";

    std::cout
            << std::setprecision(3) << std::fixed
            << "\tReading markers:  " << _markers_reading_time << " seconds\n"
            << "\tBuilding trie:    " << _trie_building_time << " seconds\n"
            << "\tReading genomes:  " << _genomes_reading_time << " seconds\n"
            << "\tMatching genomes: " << _genomes_matching_time << " seconds\n"
            << "\tSaving results:   " << _results_saving_time << " seconds\n";

    if (!_e.empty()) {
        std::cout << "During execution, the following errors occurred:\n";
        for (const auto &e : _e)
            std::cout << "\t" << e << "\n";
    }
}


void Program::cleanup() {
    _ac = aho_corasick::trie{};
    _markers.clear();
    _m.clear();
    _q.clear();
}
