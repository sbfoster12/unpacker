#include "UnpackerHelpers.hh"

bool unpackers::IsHeaderEvent(const TMEvent* thisEvent) {
    return (thisEvent->event_id == 0x8000) or (thisEvent->event_id == 0x8001) or (thisEvent->event_id == 0x8002);
}

std::vector<uint64_t> unpackers::GetXWords(const uint64_t *words, unsigned int &wordNum,int XWords, std::string endianness) {

    std::vector<uint64_t> outWordsVec(XWords);
    const uint64_t* currentWord = words+wordNum;

    for (unsigned int i = 0; i < XWords; ++i) {
        
        if (endianness == "bigendian") { outWordsVec[i] = SwapBytes(currentWord[i]); }
        else { outWordsVec[i] = currentWord[i]; }
        
    }

    //increment the word number
    wordNum+=XWords;

    return outWordsVec;
}

uint64_t unpackers::SwapBytes(const uint64_t& word) { return be64toh(word); }

void unpackers::PrintWordHex(const uint64_t& word) {
    std::cout << std::setw(16) << std::setfill('0') << std::hex << word << std::dec << std::endl;
}

uint64_t unpackers::ExtractBits(const uint64_t& word, int startBit, int endBit) {
    // Calculate the mask to extract the desired bits
    uint64_t mask = (1ULL << (endBit - startBit + 1)) - 1;
    
    // Shift and mask the value to extract the desired bits
    uint64_t extractedBits = (word >> startBit) & mask;

    return extractedBits;
}