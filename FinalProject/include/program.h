#ifndef GENES_PROGRAM_H
#define GENES_PROGRAM_H

#include <vector>
#include <string>
#include <tbb/concurrent_map.h>
#include <tbb/concurrent_queue.h>

#include "utils.h"
#include "reader.h"
#include "matcher.h"
#include "aho_corasick.h"


class Program {
    AhoCorasick                  _ac;
    SequenceReader               _reader;
    std::vector<SequenceMatcher> _matchers;

    std::string                  _result_file;
    std::string                  _markers_file;

    tbb::concurrent_bounded_queue<FastaRecord>          _q;
    tbb::concurrent_map<std::string, std::vector<bool>> _m;
public:
    Program(size_t n_matchers, size_t max_queue_size, std::string, std::string, std::string);
    void run();
};

#endif //GENES_PROGRAM_H
