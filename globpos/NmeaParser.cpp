#include "NmeaParser.h"

namespace globpos {

NmeaParser::NmeaParser() {
    reset();
}

void NmeaParser::reset() {
    clearCurrentSentence();
    state = S_SOS;
}

void NmeaParser::setVerifyChecksum(bool verifyChecksum) {
    this->verifyChecksum = verifyChecksum;
}

void NmeaParser::parse(char letter) {
    bool commit = false; // flag to commit collected sentence
    bool clear = false; // flag to clear collected sentence

    // check for starting symbol
    bool isStart = false;
    if (letter == '$') {
        isStart = true;
        if (state != S_SOS) {
#ifdef NMEA_PARSER_PRINT_ERRORS
            fprintf(stderr, "Unexpected start of sentence at state %d\n", state);
#endif
            state = S_SOS;
            commit = true; // maybe there was something valid
        }
    }

    // check for ending symbol
    if (letter == END[currentEnd]) {
        currentEnd++;
    } else {
        currentEnd = 0;

        // can be only \n or any wrong char
        if (letter == END[1] || !isprint(letter)) {
            currentEnd = 2;
        }
    }
    bool isEnd = currentEnd > 0;

    bool isSentence = false; // is it a meaningful character of the sentence
    bool startData = false; // start new current data part

    switch (state) {
        case S_SOS:
            if (isStart) {
                state = S_ADDRESS;
                clear = true;
            } else {
#ifdef NMEA_PARSER_PRINT_ERRORS
                fprintf(stderr, "Wrong starting character %d\n", (int)letter);
#endif
            }
            break;
        case S_ADDRESS:
            if (letter == ',') {
                state = S_DATA;
                startData = true;
            } else {
                addressLength++;
            }
            isSentence = true;
            break;
        case S_DATA:
            if (letter == ',') {
                data.add(currentData);
                startData = true;
                isSentence = true;
            } else if (letter == '*') {
                data.add(currentData);
                state = S_CHECKSUM;
            } else if (isEnd) {
                data.add(currentData);
                state = S_END;
            } else {
                currentData.size++;
                isSentence = true;
            }
            break;
        case S_CHECKSUM:
            if (isEnd) {
                state = S_END;
            } else {
                checksum.add(letter);
            }
            if (checksum.length == CHECKSUM_LENGTH) {
                state = S_END;
            }
            break;
        case S_END:
            if (!isEnd) {
#ifdef NMEA_PARSER_PRINT_ERRORS
                fprintf(stderr, "Wrong ending character %d\n", (int)letter);
#endif
            }
            break;
        default:
            fprintf(stderr, "Unknown state %d\n", state);
            break;
    }

    if (isSentence) {
        if (sentence.length >= MAX_SENTENCE_LENGTH) {
            fprintf(stderr, "Length limit reached\n");
        }
        sentence.add(letter);
    }

    if (startData) {
        currentData.offset = sentence.length; // should be next character after comma
        currentData.size = 0;
    }

    if (isEnd) {
        if (state != S_END) {
#ifdef NMEA_PARSER_PRINT_ERRORS
            fprintf(stderr, "Unexpected end of sentence at state %d\n", state);
#endif
        }

        if (currentEnd == sizeof(END)) {
            currentEnd = 0;

            commit = true;
            state = S_SOS;
        }
    }

    if (commit) {
        if (addressLength > 0 && data.length > 0) {
            if (verifyChecksum) {
                if (verifyCurrentSentence()) {
                    commitCurrentSentence();
                } else {
                    std::string addressStr(sentence.content, addressLength);
                    std::string checksumStr(checksum.content, checksum.length);
                    fprintf(stderr, "Checksum %s verification failed for %s with %d fields\n",
                            checksumStr.c_str(), addressStr.c_str(), (int)data.length);
                }
            } else {
                commitCurrentSentence();
            }
        }
        clear = true;
    }

    if (clear) {
        clearCurrentSentence();
    }
}

void NmeaParser::parse(const char* buffer, size_t length) {
    for (size_t i = 0; i < length; i++) {
        parse(buffer[i]);
    }
}

void NmeaParser::clearCurrentSentence() {
    sentence.clear();
    checksum.clear();
    currentEnd = 0;
    addressLength = 0;
    data.clear();
}

bool NmeaParser::verifyCurrentSentence() {
    if (checksum.length == 0) {
        return true;
    }

    char checksumStr[CHECKSUM_LENGTH + 1];
    checksumStr[CHECKSUM_LENGTH] = '\0';
    for (size_t i = 0; i < checksum.length; i++) {
        checksumStr[i] = checksum.content[i];
    }

    int checksumNum;
    sscanf(checksumStr, "%X", &checksumNum);

    int sum = 0;
    for (size_t i = 0; i < sentence.length; i++) {
        int c = (int)sentence.content[i];
        sum ^= c;
    }
    return sum == checksumNum;
}

void NmeaParser::commitCurrentSentence() {
    if (addressLength > MAX_SENTENCE_LENGTH) {
        fprintf(stderr, "Too long address size\n");
        return;
    }

    auto destSentence = std::make_shared<NmeaSentence>();
    destSentence->address.assign(sentence.content, addressLength);

    for (size_t i = 0; i < data.length; i++) {
        const Part& p = data.content[i];
        if (p.offset + p.size > MAX_SENTENCE_LENGTH) {
            fprintf(stderr, "Too long data field size\n");
            continue;
        }

        std::string destData(sentence.content + p.offset, p.size);
        destSentence->data.push_back(destData);
    }

    completeSentences.push_back(destSentence);
}

const SentenceContainer& NmeaParser::getSentences() const {
    return completeSentences;
}

void NmeaParser::clearSentences() {
    completeSentences.clear();
}

}
