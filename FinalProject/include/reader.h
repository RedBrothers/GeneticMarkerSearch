#ifndef GENES_READER_H
#define GENES_READER_H

#include <tbb/concurrent_queue.h>

#include "utils.h"


class SequenceReader {
    std::string _dir;

    tbb::concurrent_bounded_queue<FastaRecord> &_q;
public:
    SequenceReader(std::string dir, tbb::concurrent_bounded_queue<FastaRecord> &q);
    void run();
};

#endif //GENES_READER_H
