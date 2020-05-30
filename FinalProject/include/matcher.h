#ifndef GENES_MATCHER_H
#define GENES_MATCHER_H

#include <tbb/concurrent_map.h>
#include <tbb/concurrent_queue.h>

#include "utils.h"
#include "aho_corasick.h"


class SequenceMatcher {
    AhoCorasick &_ac;

    tbb::concurrent_bounded_queue<FastaRecord>          &_q;
    tbb::concurrent_map<std::string, std::vector<bool>> &_m;
public:
    SequenceMatcher(
            AhoCorasick& ac,
            tbb::concurrent_bounded_queue<FastaRecord> &q,
            tbb::concurrent_map<std::string, std::vector<bool>> &m);
    void run();
};

#endif //GENES_MATCHER_H
