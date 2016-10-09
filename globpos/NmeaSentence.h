#ifndef GLOBPOS_NMEASENTENCE_H
#define GLOBPOS_NMEASENTENCE_H

#include <string>
#include <vector>

namespace globpos {

struct NmeaSentence {
    std::string address;
    std::vector<std::string> data;
};

}

#endif //GLOBPOS_NMEASENTENCE_H
