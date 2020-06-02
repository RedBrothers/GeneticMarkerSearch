#include <boost/filesystem.hpp>
#include "utils.h"
#include "reader.h"
#include <exception>

namespace fs = boost::filesystem;


SequenceReader::SequenceReader(
        std::string dir,
        tbb::concurrent_bounded_queue<FastaRecord> &q,
        tbb::concurrent_vector<std::string> &e
        )
        : _dir {std::move(dir)}
        , _q {q}
        , _e {e} {}

void SequenceReader::run() {
    for (auto& p : fs::recursive_directory_iterator(_dir)) {
        auto path = fs::canonical(p).string();
        std::vector<FastaRecord> fasta;
        try {
            if (is_archive(path))
                fasta = read_fasta_archive(path);
            else if (is_fasta_file(path))
                fasta = read_fasta_file(path);
            else
                continue;
        } catch (std::exception &e) {
            _e.push_back("Error reading file " + path + ": " + e.what());
            continue;
        }
        for (auto&& f : fasta)
            while(!_q.try_push(f));
    }

    while(!_q.try_push(FastaRecord(READING_DONE)));
}
