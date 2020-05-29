#include "aho_corasick.h"
#include <queue>
#include <iostream>


// TODO: add check if (!set) throw ...


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


size_t AhoCorasick::g(size_t s, char c) const {
    if (_states[s].has_key(c)) {
        return _states[s].next(c);
    } else if (_states[0].has_key(c)) {
        return 0;
    } else {
        return -1;
    }
}


void AhoCorasick::construct_trie() {
    size_t id {0};
    _states.emplace_back(id);

    for(const auto& pattern : _patterns) {
        size_t s = 0;
        for (auto c : pattern) {
            if (!_states[s].has_key(c)) {
                _states.emplace_back(++id);
                _states[s].set_next(c, id);
            }
            s = _states[s].next(c);
        }
        _outputs[s].push_back(pattern);
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
            if (r) {
                s = _failure[r];
                while (g(s, c) == -1)
                    s = _failure[s];
                _failure[next] = g(s, c);

                auto words = _outputs[_failure[next]];
                if (!words.empty())
                    _outputs[next].insert(
                            _outputs[next].end(),
                            words.cbegin(), words.cend());
            }
        }
    }
}


void AhoCorasick::match(const std::string &text) const {
    size_t state = 0;

    for (auto i = 0; i < text.length(); ++ i) {
        while (g(state, text[i]) == -1)
            state = _failure.at(state);
        state = g(state, text[i]);
        for (const auto& out : _outputs.at(state)) {
            std::cout
                << out << " found at position "
                << i - out.length() << std::endl;
        }
    }
}
