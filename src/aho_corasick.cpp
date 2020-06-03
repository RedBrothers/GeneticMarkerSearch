#include "aho_corasick.h"
#include <queue>
#include <numeric>
#include <fstream>
#include <sstream>


void AhoCorasick::set(const std::vector<std::string> &patterns) {
    reset();
    _patterns = patterns;
    construct();
}


void AhoCorasick::set(std::vector<std::string> &&patterns) {
    reset();
    _patterns = std::move(patterns);
    construct();
}


void AhoCorasick::construct() {
    if (_patterns.empty())
        throw PatternsNotSetException();
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
    if (!_set)
        throw PatternsNotSetException();
}


size_t AhoCorasick::g(size_t s, char c) const {
    if (_states[s].has_key(c))
        return _states[s].next(c);
    else if (!_states[0].has_key(c))
        return 0;
    else
        return -1;
}


void AhoCorasick::construct_trie() {
    auto max_states = std::transform_reduce(
            _patterns.cbegin(), _patterns.cend(),
            0, std::plus<size_t>{},
            [](const std::string &s) { return s.length(); });
    _states.reserve(max_states);

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
    for (auto [_, next] : _states[s].next()) {
        q.push(next);
        _failure[next] = s;
    }

    while (!q.empty()) {
        auto r = q.front(); q.pop();
        for (auto [c, next] : _states[r].next()) {
            q.push(next);

            if (!r)
                continue;

            s = _failure[r];
            while (g(s, c) == -1)
                s = _failure[s];
            _failure[next] = g(s, c);
            _outputs[next].insert(
                    _outputs[next].end(),
                    _outputs[_failure[next]].cbegin(),
                    _outputs[_failure[next]].cend());
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


void AhoCorasick::load_trie(
        const std::string &trie_file_name,
        const std::string &failure_file_name,
        const std::string &outputs_file_name) {

    reset();
    char c;
    size_t num_states, s, n;

    // read trie
    std::ifstream trie_file {trie_file_name};
    trie_file >> num_states;
    _states.reserve(num_states);
    for (size_t id = 0; id < num_states; ++id)
        _states.emplace_back(id);
    while (trie_file >> s >> c >> n)
        _states[s].set_next(c, n);

    // read failure
    std::ifstream failure_file {failure_file_name};
    while (failure_file >> s >> n)
        _failure[s] = n;

    // read outputs
    std::ifstream outputs_file {outputs_file_name};
    while (outputs_file >> s >> n)
        _outputs[s].push_back(n);

    _set = true;
}


void AhoCorasick::save_trie(
        const std::string &trie_file_name,
        const std::string &failure_file_name,
        const std::string &outputs_file_name) const {

    validate();

    // save trie
    std::ofstream trie_file {trie_file_name};
    trie_file << _states.size() << std::endl;
    for (const auto &s : _states)
        for (const auto &[c, n] : s.next())
            trie_file << s.id() << " " << c << " " << n << std::endl;

    // save failure
    std::ofstream failure_file {failure_file_name};
    for (const auto &[s, n] : _failure)
        failure_file << s << " " << n << std::endl;

    // save outputs
    std::ofstream outputs_file {outputs_file_name};
    for (const auto& [s, list] : _outputs)
        for (const auto &e : list)
            outputs_file << s << " " << e << std::endl;
}
