#ifndef GLOBPOS_NMEASENTENCE_H
#define GLOBPOS_NMEASENTENCE_H

#include <memory>
#include <string>
#include <vector>

namespace globpos {

struct NmeaSentence {
    std::string address;
    std::vector<std::string> data;
};

using SentenceSharedPtr = std::shared_ptr<NmeaSentence>;
using SentenceContainer = std::vector<SentenceSharedPtr>;

}

#endif //GLOBPOS_NMEASENTENCE_H
