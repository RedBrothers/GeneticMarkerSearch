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


bool has_any_of_extensions(
        const std::string &file_name,
        const std::vector<std::string> &extensions
        ) {
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

bool is_text_file(const std::string &file_name) {
    return has_any_of_extensions(file_name, {".txt"});
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

bool is_valid_marker(const std::string& marker) {
    return std::all_of(marker.cbegin(), marker.cend(),
            boost::is_any_of(FASTA_CHARS));
}

std::vector<std::string> read_csv(const std::string &file_name) {
    std::vector<std::string>    markers {};
    std::ifstream               file;
    std::string                 line;
    std::string                 marker;
    std::vector<std::string>    split_result;

    file.open(file_name);
    while (std::getline(file, line)) {
        boost::split(split_result, line, boost::is_any_of(","));

        if (split_result.size() == 2 && is_valid_marker(split_result[1])) {
            markers.push_back(split_result[1]);
        }
    }
    return markers;
}

std::vector<FastaRecord> read_fasta(const std::string &file_name) {
    std::vector<FastaRecord> records {};
    std::ifstream            file;
    std::string              line;
    std::string              id;
    std::string              sequence;
    bool                     first {true};

    file.open(file_name);
    while (std::getline(file, line)) {
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



