#ifndef GLOBPOS_NMEASENTENCE_H
#define GLOBPOS_NMEASENTENCE_H

#include <string>
#include <vector>

namespace globpos {

struct NmeaSentence {
    std::string address;
    std::vector<std::string> data;

    /**
     * Gets data field from container after boundary check.
     * @param[in]  i  Index
     * return Returns the data field or empty string.
     */
    std::string safeGetData(size_t i) const;
};

}

#endif //GLOBPOS_NMEASENTENCE_H
