#pragma once
#include <vector>
#include <stdint.h>

class BitStream
{
    public:
        BitStream(unsigned int size);
        bool addBits(unsigned int toadd,uint8_t bitnumber);
        bool addBytes(unsigned int toadd,uint8_t bytenumber);
        void dump();
        std::vector<uint8_t> data_;

    private:
        uint8_t currentBit_{0};
        unsigned int currentByte_{0};
        unsigned int maxSize_{0};
};