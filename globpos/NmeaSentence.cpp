#include "NmeaSentence.h"

namespace globpos {

std::string NmeaSentence::safeGetData(size_t i) const {
    return (i < data.size()) ? data[i] : "";
}

}
