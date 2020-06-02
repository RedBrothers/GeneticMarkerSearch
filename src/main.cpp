#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/locale.hpp>
#include "utils.h"
#include "aho_corasick.h"
#include "config.h"
#include "program.h"


void print_usage(const std::string &binary_name) {
    std::cout << "Usage: " << binary_name << " [config_file]" << std::endl;
}


int main(int argc, char **argv) {
    if (argc != 2) {
        print_usage(std::string{argv[0]});
        return -1;
    }
    std::locale::global(
            boost::locale::generator().generate("en_US.UTF-8"));

    ConfigParser    cp{argv[1]};
    size_t          num_threads{std::stoul(cp.get("num_threads"))};
    size_t          max_queue_size{std::stoul(cp.get("max_queue_size"))};
    std::string     result_file{cp.get("result_file")};
    std::string     markers_file{cp.get("markers_file")};
    std::string     genomes_path{cp.get("genomes_path")};
    bool            verbose{boost::lexical_cast<bool>(cp.get("verbose"))};

    if (num_threads < 2) {
        std::cout << "num_threads must be at least 2, received " << num_threads;
        return -1;
    }

    if (verbose) {
        std::cout
            << "Configuration file " << argv[1] << ":" << std::endl
            << "\tgenomes_path=" << genomes_path << std::endl
            << "\tmarkers_file=" << markers_file << std::endl
            << "\tresult_file=" << result_file << std::endl
            << "\tnum_threads=" << num_threads << std::endl
            << "\tmax_queue_size=" << max_queue_size << std::endl
            << "\tverbose=" << verbose << std::endl << std::endl;
    }
    Program program {
        num_threads,
        max_queue_size,
        result_file,
        markers_file,
        genomes_path,
        verbose
    };

    program.run();
    return 0;
}
