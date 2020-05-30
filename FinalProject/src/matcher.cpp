#include "utils.h"
#include "matcher.h"


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

        if (fasta._id == "Vsio") {
            while(!_q.try_push(fasta));
            break;
        }

        auto result = _ac.match(fasta._sequence);
        _m[fasta._id] = result;
    }
}
