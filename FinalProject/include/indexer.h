#ifndef GENES_INDEXER_H
#define GENES_INDEXER_H

#include <string>
#include "queue.h"
#include "utils.h"

class Indexer {
public:
    Indexer(std::string indir,
            Deque<std::vector<FastaRecord>>& file_queue)
            : indir_{ std::move(indir) }
            , file_queue_{ file_queue } {}

    void run();
private:
    std::string indir_;
    Deque<std::vector<FastaRecord>>& file_queue_;

    void index_dir(const std::string& dir);
};

#endif //GENES_INDEXER_H
