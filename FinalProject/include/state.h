#ifndef GENES_STATE_H
#define GENES_STATE_H

#include <map>


class State {
    size_t                 _id;
    std::map<char, size_t> _next;
public:
    explicit State(size_t id = 0)
        : _id {id} {}

    ~State() {
        _next.clear();
    }

    [[nodiscard]] size_t id() const {
        return _id;
    }

    [[nodiscard]] bool has_key(char c) const {
        return _next.count(c);
    }

    [[nodiscard]] size_t next(char c) const {
        return _next.at(c);
    }

    [[nodiscard]] std::map<char, size_t> next() const {
        return _next;
    }

    void set_next(char c, const size_t& s) {
        _next[c] = s;
    }
};

#endif //GENES_STATE_H
