#include "NmeaParser.h"

namespace globpos {

void NmeaParser::parse(char c) {
}

void NmeaParser::parse(const char* buffer, size_t len) {
    for (size_t i = 0; i < len; i++) {
        parse(buffer[i]);
    }
}

}
