#ifndef GENES_AHO_CORASICK_H
#define GENES_AHO_CORASICK_H
#include <string>
#include <vector>
#include <map>
#include <queue>
#include "state.h"


class AhoCorasick
{
private:
    std::vector<std::string>                patterns;
    std::map<int, std::vector<std::string>> outputs;
    std::map<int, int>                      failure;
    std::vector<State>                      states;
    int                                     cnt = 0;

    void    construct_goto_func()
    {
        int         idx;
        int         state;
        std::string pattern;
        const char  *chars;

        this->states.push_back(*(new State(0)));
        for (int i = 0; i < this->patterns.size(); i++)
        {
            pattern = this->patterns[i];
            chars = pattern.c_str();
            state = 0;
            idx = 0;
            while (idx < strlen(chars) && this->states[state].has_key(chars[idx]))
            {
                state = this->states[state].nexts[chars[idx]];
                idx++;
            }
            for (int j = idx; j < strlen(chars); j++)
            {
                cnt++;
                this->states[state].reg_next_state(chars[j], cnt);
                this->states.push_back(*(new State(cnt)));
                state = cnt;
            }
            this->outputs[cnt].push_back(pattern);
        }
    }
    void    construct_failure_func()
    {
        std::queue<int> q;
        int             r;
        int             s;
        char            a;
        int             state;

        for (auto & next : this->states[0].nexts)
        {
            q.push(next.second);
            this->failure[next.second] = 0;
        }

        while (!q.empty())
        {
            r = q.front();
            q.pop();

            for (auto next = this->states[r].nexts.begin(); next != this->states[r].nexts.end(); ++next)
            {
                a = next->first;
                s = next->second;

                q.push(s);
                if (r > 0)
                {
                    state = this->failure[r];

                    while (g(state, a) == -1)
                        state = this->failure[state];
                    this->failure[s] = g(state, a);
                    auto words = this->outputs[this->failure[s]];

                    if (!words.empty())
                        this->outputs[s].insert(this->outputs[s].end(), words.begin(), words.end());
                }
            }

        }
    }

    int g(int state, char a)
    {
        if (this->states[state].has_key(a))
            return this->states[state].nexts[a];
        else if (!this->states[0].has_key(a))
            return 0;
        else
            return -1;
    }

public:
    AhoCorasick(std::vector<std::string> in_patterns): patterns{in_patterns} {
        construct_goto_func();
        construct_failure_func();
    }

    std::tuple<std::string, int> match(std::string text_string)
    {
        std::map<int, std::vector<int>> match_places;
        int state = 0;
        int length = 0;
        int dis = 0;
        int bold_num = 0; // count matched words
        int col_len;
        unsigned long long int search_cnt = 0;
        std::string contents = text_string;
        const char *chars = text_string.c_str();

        // string match
        for (int i = 0; i < text_string.length(); i++)
        {
            search_cnt++;
            while (g(state, chars[i]) == -1)
            {
                state = this->failure[state];
                search_cnt++;
            }
            state = g(state, chars[i]);
            if (!this->outputs[state].empty())
            {
                col_len = 0;
                for (int j = 0; j < this->outputs[state].size(); j++)
                {
                    col_len = (this->outputs[state][j].length() > col_len) ? this->outputs[state][j].length() : col_len;
                }
                match_places[i - col_len + 1].push_back(i);
            }
        }

        // make contents formatted html
        for (auto & match_place : match_places)
        {
            dis = match_place.second[0];
            if (match_place.second.size() > 1)
            {
                for (int & i : match_place.second)
                {
                    dis = (i > dis) ? i : dis;
                }
            }
            contents.insert(match_place.first + bold_num * 7, "<b>");
            contents.insert(dis + bold_num * 7 + 4, "</b>");
            bold_num++;
        }
        return {contents, search_cnt};
    }
};
#endif //GENES_AHO_CORASICK_H
