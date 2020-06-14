#include <matcher.hpp>
#include <reader.hpp>
#include <utils.hpp>
#include <exception>


SequenceMatcher::SequenceMatcher(
        aho_corasick::trie                                 &ac,
        tbb::concurrent_bounded_queue<Fasta>               &q,
        tbb::concurrent_map<std::string, std::set<size_t>> &m,
        tbb::concurrent_vector<std::string>                &e)
        : _ac{ac}
        , _q{q}
        , _m{m}
        , _e{e} {}


void SequenceMatcher::run() {
    Fasta fasta {};

    while(true) {
        while (!_q.try_pop(fasta));

        if (fasta.id == READING_DONE) {
            while(!_q.try_push(fasta));
            break;
        }
        try {
            auto result = _ac.parse_text(fasta.seq);
            std::set<size_t> s{};
            for (const auto &r : result) {
                s.insert(r.get_index());
            }
            _m[fasta.id] = std::move(s);
        } catch (std::exception &e) {
            _e.push_back("Error matching genome " + fasta.id + ": " + e.what());
        }
    }
}
