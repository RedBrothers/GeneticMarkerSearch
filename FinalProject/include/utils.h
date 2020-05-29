#ifndef GENES_UTILS_H
#define GENES_UTILS_H
#include "typedefs.h"
#include <vector>
#include <string>

struct FastaRecord
{
    std::string id{};
    std::string sequence{};

    FastaRecord(
            std::string id_,
            std::string sequence_
        )
        : id{std::move(id_)}
        , sequence{std::move(sequence_)} {};
};


// file manipulations
template<typename Stream>
std::vector<FastaRecord>    read_fasta(Stream &stream);
std::vector<FastaRecord>    read_fasta_file(const std::string &file_name);
std::vector<FastaRecord>    read_fasta_string(const std::string &text);
std::vector<std::string>    read_markers(const std::string& file_name, size_t max_rows=0);
std::vector<size_t>         split_file(const std::string& file, size_t n_chunks);
bool                        is_archive(const std::string &file_name);
bool                        is_csv_file(const std::string& file_name);
bool                        is_text_file(const std::string& file_name);
bool                        is_fasta_file(const std::string& file_name);
std::string                 read_archive(const std::string &file_name);

void                        write_csv(
        const std::vector <std::pair<std::string, std::vector<bool>>>&  result,
        std::vector <std::string>                                       columns,
        const std::string&                                              file_name
);

const std::string FASTA_CHARS {"ACGTURYKMSWBDHVN"};
constexpr char FASTA_COMMENT_START {';'};
constexpr char FASTA_ID_START {'>'};
constexpr char FASTA_ID_END {' '};

#endif //GENES_UTILS_H
