#ifndef GENES_UTILS_H
#define GENES_UTILS_H
#include "typedefs.h"
#include <vector>
#include <string>

struct FastaRecord
{
    std::string id;
    std::string sequence;

    FastaRecord(
            std::string id_,
            std::string sequence_
        )
        : id{std::move(id_)}
        , sequence{std::move(sequence_)} {};
};


// file manipulations
std::vector<std::string>    read_csv(const std::string& file_name);
std::vector<FastaRecord>    read_fasta(const std::string& file_name);
std::vector<size_t>         split_file(const std::string& file, size_t n_chunks);
bool                        is_archive(const std::string &file_name);
bool                        is_csv_file(const std::string& file_name);
bool                        is_text_file(const std::string& file_name);
bool                        is_fasta_file(const std::string& file_name);
std::string                 read_archive(const std::string &file_name);

const std::string FASTA_CHARS {"ACGTURYKMSWBDHVN"};
constexpr char FASTA_COMMENT_START {';'};
constexpr char FASTA_ID_START {'>'};
constexpr char FASTA_ID_END {' '};

#endif //GENES_UTILS_H
