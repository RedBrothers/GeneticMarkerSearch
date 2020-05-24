#ifndef GENES_BASE_MANAGER_H
#define GENES_BASE_MANAGER_H

#include <map>
#include <set>
#include <vector>
#include <string>
#include <functional>
#include "typedefs.h"

class BaseManager {
public:
    void save();
    BaseManager() {}
protected:
    // main dictionary
    GeneMarkerMap gmm; // TODO: Let's discuss more appropriate data structure
//private:

};

#endif //GENES_BASE_MANAGER_H
