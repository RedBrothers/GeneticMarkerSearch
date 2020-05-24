#include <boost/filesystem.hpp>
#include "../include/indexer.h"
#include "../include/utils.h"

namespace fs = boost::filesystem;

void Indexer::run() {
    index_dir(indir_);
}

void Indexer::index_dir(const std::string& dir) {
    for (auto& p : fs::recursive_directory_iterator(dir)) {
        auto path = fs::canonical(p).string();
        std::string file;
        if (is_archive(path)) {
            file = read_archive(path);
        } else if (is_fasta_file(path)) {
            file = read_fasta(path);
        } else {
            continue;
        }
        file_queue_.push_front(std::move(file));
    }
}