#include "utils.h"
#include "matcher.h"
#include "reader.h"


SequenceMatcher::SequenceMatcher(
        AhoCorasick &ac,
        tbb::concurrent_bounded_queue<FastaRecord> &q,
        tbb::concurrent_map<std::string, std::vector<bool>> &m
        )
        : _ac{ac}
        , _q{q}
        , _m{m} {}

void SequenceMatcher::run() {
    FastaRecord fasta {};

    while(true) {
        while (!_q.try_pop(fasta));

        if (fasta._id == READING_DONE) {
            while(!_q.try_push(fasta));
            break;
        }
        try {
            auto result = _ac.match(fasta._sequence);
            _m[fasta._id] = result;
        } catch (...) {
            // std::cout << "Error while matching the genome " << _fasta.id << std::endl;
        }
    }
}
