#ifndef GENES_AHO_CORASICK_H
#define GENES_AHO_CORASICK_H

#include <map>
#include <string>
#include <utility>
#include <vector>
#include <exception>
#include "state.h"


struct PatternsNotSetException : public std::exception {
    [[nodiscard]] const char * what() const noexcept override {
        return "Patterns are not set";
    }
};


class AhoCorasick
{
private:
    std::map<
        size_t,
        std::vector<size_t>
        >                       _outputs;
    std::vector<State>          _states;
    std::map<size_t, size_t>    _failure;
    std::vector<std::string>    _patterns;
public:
    AhoCorasick() : _set{ false } { reset(); }
    ~AhoCorasick() { reset(); }

    void set_patterns(const std::vector<std::string> &patterns);
    [[nodiscard]] std::vector<bool> match(const std::string &text) const;
    void reset();
private:
    bool _set;
    void validate() const;
    void construct_trie();
    void construct_failure();
    [[nodiscard]] size_t g(size_t s, char c) const;
};


#endif //GENES_AHO_CORASICK_H
