#ifndef GENES_STATE_H
#define GENES_STATE_H

#include <cstdio>
#include <map>

class State
{
public:
    size_t              id;
    std::map<char, int> nexts;

    State(size_t id)    { this-> id = id; }
    ~State()            { nexts.clear(); }


    bool    has_key(char c)                 { return nexts.count(c) >= 1; };
    int     next_state(char c)              { return nexts[c]; };
    void    reg_next_state(char c, int s)   { nexts[c] = s; }
};

#endif //GENES_STATE_H
