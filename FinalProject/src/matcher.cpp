#include "utils.h"
#include "matcher.h"


void SequenceMatcher::run() {
    FastaRecord fasta {};

    while(true) {
        while (!_q.try_pop(fasta));

        if (fasta._id == "Vsio") {
            while(!_q.try_push(fasta));
            break;
        }

        auto result = _ac.match(fasta._sequence);
        _m[fasta._id] = result;
    }
}
