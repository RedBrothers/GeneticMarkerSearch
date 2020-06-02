#ifndef GENES_MATCHER_H
#define GENES_MATCHER_H

# define TBB_PREVIEW_CONCURRENT_ORDERED_CONTAINERS true

#include <tbb/concurrent_map.h>
#include <tbb/concurrent_queue.h>
#include <tbb/concurrent_vector.h>

#include "utils.h"
#include "aho_corasick.h"


class SequenceMatcher {
    AhoCorasick &_ac;

    tbb::concurrent_bounded_queue<FastaRecord>          &_q;
    tbb::concurrent_map<std::string, std::vector<bool>> &_m;
    tbb::concurrent_vector<std::string>                 &_e;
public:
    SequenceMatcher(
            AhoCorasick&                                        ac,
            tbb::concurrent_bounded_queue<FastaRecord>          &q,
            tbb::concurrent_map<std::string, std::vector<bool>> &m,
            tbb::concurrent_vector<std::string>                 &e);
    void run();
};

#endif //GENES_MATCHER_H
