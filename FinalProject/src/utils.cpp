#include "utils.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <boost/locale.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <archive.h>
#include <archive_entry.h>

namespace ba = boost::locale::boundary;
namespace lc = boost::locale;

Dict        count_words(const std::string& file, size_t start, size_t end) {
    Dict                chunk_map;
    ba::ssegment_index  map(ba::word, file.begin() + start, file.begin() + end);
    map.rule(ba::word_letters);

    for (auto it = map.begin(), e = map.end(); it != e; ++it) {

        std::string word(*it);

        word = lc::normalize(word);
        word = lc::fold_case(word);

        ++chunk_map[word];
    }

    return chunk_map;
}

bool       is_archive(const std::string &file_name) {
    std::vector<std::string> extensions = {".zip", ".tar", ".tar.gz", ".gz", ".7z"};

    return std::find(
            extensions.begin(),
            extensions.end(),
            boost::locale::to_lower(boost::filesystem::extension(file_name))
    ) != extensions.end();

}

bool       is_csv_file(const std::string &file_name) {
    std::vector<std::string> extensions = {".csv"};

    return std::find(
            extensions.begin(),
            extensions.end(),
            boost::locale::to_lower(boost::filesystem::extension(file_name))
    ) != extensions.end();

}

bool       is_fasta_file(const std::string &file_name) {
    std::vector<std::string> extensions = {".fasta", ".fas", ".fna", ".faa", ".ffn"};

    return std::find(
            extensions.begin(),
            extensions.end(),
            boost::locale::to_lower(boost::filesystem::extension(file_name))
    ) != extensions.end();
}

bool       is_text_file(const std::string &file_name) {
    std::vector<std::string> extensions = {".txt"};

    return std::find(
            extensions.begin(),
            extensions.end(),
            boost::locale::to_lower(boost::filesystem::extension(file_name))
    ) != extensions.end();

}

std::string read_archive(const std::string &file_name) {
    std::stringstream           ss;
    std::vector<std::string>    vs;
    struct archive              *a;
    struct archive_entry        *entry;

    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);

    if (archive_read_open_filename(a, file_name.c_str(), 10240) != ARCHIVE_OK)
        exit(1);

    if (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
        la_int64_t  entry_size = archive_entry_size(entry);

        if (archive_entry_filetype(entry) == AE_IFREG) {
            char *buff = new char[entry_size];
            archive_read_data(a, buff, entry_size);

            ss << buff;

            delete[] buff;
        }
    }

    if (archive_read_free(a) != ARCHIVE_OK)
        exit(1);

    return ss.str();
}

bool                        is_valid_marker(std::string marker) {
    const std::string    fasta_symbols = "ACGTURYKMSWBDHVN";
    bool                 hasOnlySyms;

    const auto containsSymbols = [&fasta_symbols](char c) {
        return fasta_symbols.find(c) != std::string::npos;
    };

    hasOnlySyms = std::all_of(marker.cbegin(), marker.cend(), containsSymbols);

    return hasOnlySyms;
}

std::vector<std::string>    read_csv(const std::string &fileName) {
    std::string                 line;
    std::string                 marker;
    std::ifstream               csvFile;
    std::vector<std::string>    vMarkers {};
    std::vector<std::string>    splitResult;

    csvFile.open(fileName);
    while (csvFile.good()) {
        std::getline(csvFile, line);
        boost::split(splitResult, line, boost::is_any_of(","));

        if (splitResult.size() == 2 && is_valid_marker(splitResult[1])) {
            vMarkers.push_back(splitResult[1]);
        }

    }
    return vMarkers;
}

FastaRecord                 read_fasta(const std::string &fileName) {
    std::string     name;
    std::string     line;
    std::ifstream   fastaFile;

    fastaFile.open(fileName);
    if (fastaFile.good()) {
        std::getline(fastaFile, name);
        std::getline(fastaFile, line);
    }
    return FastaRecord{std::move(name), std::move(line)};
}

