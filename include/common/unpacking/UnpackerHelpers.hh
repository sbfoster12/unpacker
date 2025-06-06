#ifndef UNPACKERHELPERS_HH
#define UNPACKERHELPERS_HH

//Standard
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

#if __APPLE__
    #include "endianapple.h"
#elif __linu
    #include <endian.h>
#endif


//Custom
#include "midasio.h"

namespace unpackers {

    // Method to check if midas event is an internal midas event
    bool IsHeaderEvent(const TMEvent* thisEvent);

    std::vector<uint64_t> GetXWords(const uint64_t *words, unsigned int &wordNum,int XWords, std::string endianness);
    uint64_t SwapBytes(const uint64_t& word);
    void PrintWordHex(const uint64_t& word);
    uint64_t ExtractBits(const uint64_t& word, int startBit, int endBit);

}

#endif // UNPACKERHELPERS_HH