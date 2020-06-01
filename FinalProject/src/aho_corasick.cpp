#include "aho_corasick.h"
#include <queue>
#include <iostream>
#include <exception>


void AhoCorasick::set_patterns(const std::vector<std::string> &patterns) {
    _patterns = patterns;
    construct_trie();
    construct_failure();
    _set = true;
}


void AhoCorasick::reset() {
    _states.clear();
    _failure.clear();
    _outputs.clear();
    _patterns.clear();
    _set = false;
}


void AhoCorasick::validate() const {
    if (!_set || _patterns.empty())
        throw PatternsNotSetException();
}


size_t AhoCorasick::g(size_t s, char c) const {
    if (_states[s].has_key(c)) {
        return _states[s].next(c);
    } else if (!_states[0].has_key(c)) {
        return 0;
    } else {
        return -1;
    }
}


void AhoCorasick::construct_trie() {
    size_t id {0};
    _states.emplace_back(id);

    for (auto it = _patterns.cbegin(); it != _patterns.cend(); ++it) {
        auto pattern = *it;
        size_t state = 0;

        for (auto c : pattern) {
            if (!_states[state].has_key(c)) {
                _states.emplace_back(++id);
                _states[state].set_next(c, id);
            }
            state = _states[state].next(c);
        }
        _outputs[state].push_back(
                std::distance(_patterns.cbegin(), it));
    }
}


void AhoCorasick::construct_failure() {
    std::queue<size_t> q;

    size_t s = 0;
    for (auto& [_, next] : _states[s].next()) {
        q.push(next);
        _failure[next] = s;
    }

    while (!q.empty()) {
        auto r = q.front(); q.pop();
        for (auto& [c, next] : _states[r].next()) {
            q.push(next);

            if (!r)
                continue;

            s = _failure[r];
            while (g(s, c) == -1)
                s = _failure[s];
            _failure[next] = g(s, c);

            auto indexes = _outputs[_failure[next]];
            if (!indexes.empty())
                _outputs[next].insert(
                        _outputs[next].end(),
                        indexes.cbegin(), indexes.cend());
        }
    }
}


std::vector<bool> AhoCorasick::match(const std::string &text) const {
    validate();

    size_t state = 0;
    std::vector<bool> matches(_patterns.size(), false);

    for (char c : text) {
        while (g(state, c) == -1)
            state = _failure.at(state);
        state = g(state, c);
        if (_outputs.count(state))
            for (auto idx : _outputs.at(state))
                matches[idx] = true;
    }

    return matches;
}
