#ifndef GLOBPOS_SETTINGS_H
#define GLOBPOS_SETTINGS_H

#include <cstdlib>

namespace globpos {

const size_t MAX_SENTENCE_LENGTH = 256;
const size_t CHECKSUM_LENGTH = 2;
const size_t DATA_BUFFER_SIZE = 32;
const char END[] = { '\r', '\n' };

}

#endif //GLOBPOS_SETTINGS_H
