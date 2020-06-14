#ifndef GENES_MATCHER_H
#define GENES_MATCHER_H

# define TBB_PREVIEW_CONCURRENT_ORDERED_CONTAINERS true

#include <set>
#include <vector>
#include <string>

#include <tbb/concurrent_map.h>
#include <tbb/concurrent_queue.h>
#include <tbb/concurrent_vector.h>

#include <utils.hpp>
#include <aho_corasick.hpp>


class SequenceMatcher {
    aho_corasick::trie &_ac;

    tbb::concurrent_bounded_queue<Fasta>               &_q;
    tbb::concurrent_map<std::string, std::set<size_t>> &_m;
    tbb::concurrent_vector<std::string>                &_e;
public:
    SequenceMatcher(
            aho_corasick::trie                                 &ac,
            tbb::concurrent_bounded_queue<Fasta>               &q,
            tbb::concurrent_map<std::string, std::set<size_t>> &m,
            tbb::concurrent_vector<std::string>                &e);
    void run();
};

#endif //GENES_MATCHER_H
