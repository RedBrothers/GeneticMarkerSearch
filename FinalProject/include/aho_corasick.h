#ifndef GENES_AHO_CORASICK_H
#define GENES_AHO_CORASICK_H

#include <map>
#include <string>
#include <utility>
#include <vector>
#include <exception>


struct PatternsNotSetException : public std::exception {
    [[nodiscard]] const char * what() const noexcept override {
        return "Patterns are not set";
    }
};


class State {
    size_t                 _id;
    std::map<char, size_t> _next;
public:
    explicit State(size_t id = 0) : _id {id} {}
    ~State() { _next.clear(); }

    [[nodiscard]] size_t id() const { return _id; }
    [[nodiscard]] bool has_key(char c) const { return _next.count(c); }
    [[nodiscard]] size_t next(char c) const { return _next.at(c); }
    [[nodiscard]] std::map<char, size_t> next() const { return _next; }
    void set_next(char c, const size_t& s) { _next[c] = s; }
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

    void set(const std::vector<std::string> &patterns);
    void set(std::vector<std::string> &&patterns);
    [[nodiscard]] std::vector<bool> match(const std::string &text) const;
    void reset();
private:
    bool _set;
    void validate() const;
    void construct();
    void construct_trie();
    void construct_failure();
    [[nodiscard]] size_t g(size_t s, char c) const;
};

#endif //GENES_AHO_CORASICK_H
