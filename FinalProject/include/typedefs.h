#ifndef GENES_TYPEDEFS_H
#define GENES_TYPEDEFS_H
#include <set>
#include <map>
#include <string>
#include <vector>
#include <functional>

// not sure whether name injections into
// client code is a good practice
using Dict = std::map<std::string, size_t>;
using Tuple = std::pair<std::string, size_t>;
using OrderedDict = std::vector<Tuple>;
using GeneMarkerMap = std::map<std::pair<std::string, std::string>, bool>;

const std::string INDEXING_DONE = "INDEXING DONE";
const Dict COUNTING_DONE = { {"COUNTING" , 0} };

#endif //GENES_TYPEDEFS_H
