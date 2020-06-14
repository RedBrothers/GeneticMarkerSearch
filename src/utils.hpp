#ifndef GENES_UTILS_H
#define GENES_UTILS_H
#include <vector>
#include <string>
#include <chrono>


struct Fasta {
    std::string id{};
    std::string seq{};

    explicit Fasta(
            std::string id_ = "",
            std::string seq_ = "")
        : id{std::move(id_)}
        , seq{std::move(seq_)} {}
};


// reading genomes and markers
template<typename Stream>
std::vector<Fasta>       read_fasta(Stream &stream);
std::vector<Fasta>       read_fasta_file(const std::string &file_name);
std::vector<Fasta>       read_fasta_archive(const std::string &file_name);
std::vector<std::string> read_markers(const std::string& file_name, size_t max_rows = 0);

// writing AC results
void write_result(
        const std::string                    &file_name,
        const std::vector<std::vector<bool>> &result,
        const std::vector<std::string>       &fasta_ids,
        const std::vector<std::string>       &markers);

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

// time utils
namespace Time {
    typedef std::chrono::time_point<std::chrono::high_resolution_clock> stamp;
    typedef std::chrono::duration<float> delta;

    stamp now();
    float diff(stamp t1, stamp t2);
}

#endif //GENES_UTILS_H
