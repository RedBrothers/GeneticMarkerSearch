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

    std::string                  markers_file;
    std::string                  sequences_dir;

    tbb::concurrent_bounded_queue<std::string>          _q;
    tbb::concurrent_map<std::string, std::vector<bool>> _m;
public:
    Program(size_t n_matchers, std::string, std::string);
    void run();
};

#endif //GENES_PROGRAM_H
