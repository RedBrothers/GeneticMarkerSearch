#include "utils.h"
#include <sstream>
#include <boost/locale.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>

namespace ba = boost::locale::boundary;
namespace lc = boost::locale;
namespace bio = boost::iostreams;


bool has_any_of_extensions(
        const std::string              &file_name,
        const std::vector<std::string> &extensions) {
    return std::find(
            extensions.begin(),
            extensions.end(),
            boost::locale::to_lower(boost::filesystem::extension(file_name))
    ) != extensions.end();
}


bool is_archive(const std::string &file_name) {
    return has_any_of_extensions(file_name, {".zip", ".tar", ".tar.gz", ".gz", ".7z"});
}


bool is_csv_file(const std::string &file_name) {
    return has_any_of_extensions(file_name, {".csv"});
}


bool is_fasta_file(const std::string &file_name) {
    return has_any_of_extensions(file_name, {".fasta", ".fas", ".fna", ".faa", ".ffn"});
}


std::vector<FastaRecord> read_fasta_archive(const std::string &file_name) {
    std::ifstream file(file_name, std::ios_base::in | std::ios_base::binary);
    boost::iostreams::filtering_istream in;
    in.push(bio::gzip_decompressor());
    in.push(file);
    return read_fasta(in);
}


bool is_valid_marker(const std::string& marker) {
    return std::all_of(marker.cbegin(), marker.cend(),
            boost::is_any_of(FASTA_CHARS));
}


bool time_to_stop(size_t k, size_t max) {
    return max ? (k >= max) : false;
}


std::vector<MarkerRecord> read_markers(const std::string &file_name, size_t max_rows) {
    std::vector<MarkerRecord> records {};
    std::ifstream             file {file_name};
    size_t                    rows {0};
    std::string               line;
    std::string               id;
    std::string               marker;
    std::vector<std::string>  split;

    while(std::getline(file, line) && !time_to_stop(rows++, max_rows)) {
        boost::split(split, line, boost::is_any_of(","));
        if (split.size() == 2 && is_valid_marker(split[1]))
            records.emplace_back(split[0], split[1]);
    }
    return records;
}


template<typename Stream>
std::vector<FastaRecord> read_fasta(Stream &stream) {
    std::vector<FastaRecord> records {};
    std::string              line;
    std::string              id;
    std::string              sequence;
    bool                     first {true};

    while (std::getline(stream, line)) {
        if (line.empty())
            continue;

        if (line[0] == FASTA_COMMENT_START)
            continue;

        if (line[0] == FASTA_ID_START) {
            if (!first) {
                records.emplace_back(id, sequence);
                sequence.clear();
            }
            id = line.substr(1, line.find(FASTA_ID_END) - 1);
            first = false;
        } else {
            sequence += line;
        }
    }
    records.emplace_back(id, sequence);
    return records;
}


std::vector<FastaRecord> read_fasta_file(const std::string &file_name) {
    std::fstream file{file_name};
    return read_fasta<std::fstream>(file);
}


void write_result(
        const std::string                    &file_name,
        const std::vector<std::vector<bool>> &result,
        const std::vector<std::string>       &sequence_ids,
        const std::vector<std::string>       &marker_ids) {
    assert(result.size() == sequence_ids.size());
    std::ofstream file{file_name};

    for (const auto& id : marker_ids)
        file << "," << id;
    file << "\n";

    for (size_t j = 0; j < sequence_ids.size(); ++j) {
        file << sequence_ids[j];
        for (auto res : result[j])
            file << "," << res;
        file << "\n";
    }
}


Time::stamp Time::now() {
    return std::chrono::high_resolution_clock::now();
}


float Time::diff(Time::stamp t1, Time::stamp t2) {
    return std::chrono::duration_cast<Time::delta>(t2 - t1).count();
}
