#include "utils.hpp"
#include "matcher.hpp"
#include "reader.hpp"
#include <exception>


SequenceMatcher::SequenceMatcher(
        aho_corasick::trie                                  &ac,
        tbb::concurrent_bounded_queue<FastaRecord>          &q,
        tbb::concurrent_map<std::string, std::vector<bool>> &m,
        tbb::concurrent_vector<std::string>                 &e)
        : _ac{ac}
        , _q{q}
        , _m{m}
        , _e{e} {}


void SequenceMatcher::run() {
    FastaRecord fasta {};

    while(true) {
        while (!_q.try_pop(fasta));

        if (fasta._id == READING_DONE) {
            while(!_q.try_push(fasta));
            break;
        }
        try {
            auto result = _ac.parse_text(fasta._sequence);
        } catch (std::exception &e) {
            _e.push_back("Error matching genome " + fasta._id + ": " + e.what());
        }
    }
}
