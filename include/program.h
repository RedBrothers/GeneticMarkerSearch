#ifndef GENES_PROGRAM_H
#define GENES_PROGRAM_H

# define TBB_PREVIEW_CONCURRENT_ORDERED_CONTAINERS true

#include <vector>
#include <string>
#include <tbb/concurrent_map.h>
#include <tbb/concurrent_queue.h>
#include <tbb/concurrent_vector.h>

#include "utils.h"
#include "reader.h"
#include "matcher.h"
#include "aho_corasick.h"


class Program {

    bool        _verbose;
    std::string _result_file;
    std::string _markers_file;

    AhoCorasick                  _ac;
    SequenceReader               _reader;
    std::vector<SequenceMatcher> _matchers;

    tbb::concurrent_vector<std::string>                 _e;
    tbb::concurrent_bounded_queue<FastaRecord>          _q;
    tbb::concurrent_map<std::string, std::vector<bool>> _m;

public:
    Program(size_t      num_threads,
            size_t      max_queue_size,
            std::string result_file,
            std::string markers_file,
            std::string genomes_dir,
            bool        verbose = false);

    void run();

private:
    void prepare();
    void execute();
    void save();
    void report() const;
    void cleanup();

    std::vector<std::string> _m_ids;

    size_t _num_markers {0};
    size_t _num_genomes {0};
    float  _markers_reading_time {0.};
    float  _trie_building_time {0.};
    float  _genomes_reading_time {0.};
    float  _markers_matching_time {0.};
    float  _results_saving_time {0.};
};

#endif //GENES_PROGRAM_H
