/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri, Nicolo Genesio
 * email:   luca.tricerri@iit.it
*/

// - brief
//   Bit stream class
//

#pragma once
#include <vector>
#include <stdint.h>

class BitStream
{
    public:
        BitStream(unsigned int size);
        bool addBits(unsigned int toadd,uint8_t bitnumber);
        bool addBytes(unsigned long toadd,uint8_t bytenumber,const std::string& encoding);
        void dump();
        std::vector<uint8_t> data_;

    private:
        uint8_t currentBit_{0};
        unsigned int currentByte_{0};
        unsigned int maxSize_{0};
};