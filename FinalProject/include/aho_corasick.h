#ifndef GENES_AHO_CORASICK_H
#define GENES_AHO_CORASICK_H

#include <map>
#include <string>
#include <utility>
#include <vector>
#include "state.h"


class AhoCorasick
{
private:
    std::vector<std::string>                   _patterns;
    std::vector<State>                         _states;
    std::map<size_t, size_t>                   _failure;
    std::map<size_t, std::vector<std::string>> _outputs; // why vector?
public:
    AhoCorasick() : _set{ false } { reset(); }
    ~AhoCorasick() { reset(); }

    void set_patterns(const std::vector<std::string> &patterns);
    void match(const std::string &text) const;
    void reset();
private:
    bool _set;
    void construct_trie();
    void construct_failure();
    [[nodiscard]] size_t g(size_t s, char c) const;
};


#endif //GENES_AHO_CORASICK_H
