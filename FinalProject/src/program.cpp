#include "utils.h"
#include "config.h"
#include "program.h"
#include <thread>


Program::Program(
        size_t n_matchers,
        size_t max_queue_size,
        std::string result_file,
        std::string markers_file,
        std::string genomes_path)
        : _ac{}
        , _q{}
        , _m{}
        , _result_file{std::move(result_file)}
        , _markers_file{std::move(markers_file)}
        , _reader{std::move(genomes_path), _q}
        , _matchers{}
{
    _q.set_capacity(max_queue_size);
    for (size_t i = 0; i < n_matchers; ++i)
        _matchers.emplace_back(_ac, _q, _m);
}

void Program::run() {
    //
    // prepare markers and AC; maybe extract method?
    //
    auto markers = read_markers(_markers_file);
    std::vector<std::string> m_ids {}, patterns;
    for (const auto& m : markers) {
        m_ids.push_back(m._id);
        patterns.push_back(m._marker);
    }
    _ac.set_patterns(patterns);

    //
    // launch parallel execution
    //
    // launch reader; maybe extract method?
    std::thread reader_thread(&SequenceReader::run, &_reader);

    // launch matchers; maybe extract method?
    std::vector<std::thread> matcher_threads;
    for (auto& m : _matchers)
        matcher_threads.emplace_back(&SequenceMatcher::run, &m);

    // wait for threads to finish
    reader_thread.join();
    for (auto& t : matcher_threads)
        t.join();

    //
    // save results to csv; maybe extract method?
    //
    std::vector<std::string> s_ids;
    std::vector<std::vector<bool>> result;
    for (auto& [id, res] : _m) {
        s_ids.push_back(id);
        result.push_back(res);
    }

    write_result(_result_file, result, s_ids, m_ids);
}
