#ifndef GENES_QUEUE_MANAGER_H
#define GENES_QUEUE_MANAGER_H

#include "base_manager.h"
#include "queue.h"
#include "indexer.h"
#include <mutex>

class QueueManager : public BaseManager {
public:
    QueueManager(
            std::string indir,
            size_t n_count_threads,
            size_t n_merge_threads);
    void run();
private:
    std::mutex m_;

    Deque<std::string> file_queue_;
    Deque<Dict> dict_queue_;

    Indexer indexer;
//    std::vector<Counter> counters;
//    std::vector<Merger> mergers;

    std::string out_by_a;
    std::string out_by_n;
};

#endif //GENES_QUEUE_MANAGER_H
