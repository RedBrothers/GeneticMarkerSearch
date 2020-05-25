#ifndef GENES_UTILS_H
#define GENES_UTILS_H
#include "typedefs.h"
#include <vector>
#include <string>

struct FastaRecord
{
    std::string name;
    std::string sequence;
};


// file manipulations
std::vector<std::string>    read_csv(const std::string& filename);
FastaRecord                 read_fasta(const std::string& filename);
std::vector<size_t>         split_file(const std::string& file, size_t n_chunks);
bool                        is_archive(const std::string &file_name);
bool                        is_csv_file(const std::string& file_name);
bool                        is_fasta_file(const std::string& file_name);
std::string                 read_archive(const std::string &file_name);

static constexpr char fasta_delim {'>'};


#endif //GENES_UTILS_H
