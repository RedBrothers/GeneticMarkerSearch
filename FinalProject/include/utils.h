#ifndef GENES_UTILS_H
#define GENES_UTILS_H
#include "typedefs.h"
#include <vector>
#include <string>
#define TBB_PREVIEW_CONCURRENT_ORDERED_CONTAINERS 1

struct FastaRecord
{
    std::string _id{};
    std::string _sequence{};

    explicit FastaRecord(
            std::string id = "",
            std::string sequence = "")
        : _id{std::move(id)}
        , _sequence{std::move(sequence)} {};
};

struct MarkerRecord
{
    std::string _id{};
    std::string _marker{};

    explicit MarkerRecord(
            std::string id = "",
            std::string marker = "")
        : _id{std::move(id)}
        , _marker{std::move(marker)} {};
};

// reading genomes and markers
template<typename Stream>
std::vector<FastaRecord>    read_fasta(Stream &stream);
std::vector<FastaRecord>    read_fasta_file(const std::string &file_name);
std::vector<FastaRecord>    read_fasta_archive(const std::string &file_name);
std::vector<MarkerRecord>   read_markers(const std::string& file_name, size_t max_rows = 0);

// writing AC results
void write_result(
        const std::string                    &file_name,
        const std::vector<std::vector<bool>> &result,
        const std::vector<std::string>       &sequence_ids,
        const std::vector<std::string>       &marker_ids);

// file extension check routines
bool is_archive(const std::string &file_name);
bool is_csv_file(const std::string& file_name);
bool is_text_file(const std::string& file_name);
bool is_fasta_file(const std::string& file_name);

// fasta constants
const std::string FASTA_CHARS {"ACGTURYKMSWBDHVN"};
constexpr char FASTA_COMMENT_START {';'};
constexpr char FASTA_ID_START {'>'};
constexpr char FASTA_ID_END {' '};

#endif //GENES_UTILS_H
