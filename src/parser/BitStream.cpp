#include <iostream>
#include <string.h>
#include <math.h>
#include "BitStream.h"


BitStream::BitStream(unsigned int size):data_(size),maxSize_(size)
{}

bool BitStream::addBits(unsigned int toadd,uint8_t bitnumber)
{
    if(bitnumber==0)
        return true;

    if(bitnumber>8)
    {
        std::cout<<"Bit exceed size:"<<bitnumber<<std::endl;
        return false;
    }

    for(size_t pos=0;pos<bitnumber;++pos)
    {
        unsigned int mask=pow(2,pos);
        unsigned int tmp=toadd & mask;
        std::cout<<" ";
        tmp=(tmp>>pos);


        if(currentByte_>=maxSize_)
        {
            std::cout<<"Byte exceed size:"<<maxSize_<<" Current byte:"<<currentByte_<<std::endl;
            return false;
        }
 
        uint8_t value= tmp<<currentBit_; 
        data_[currentByte_]=data_[currentByte_]| value;

        currentBit_++;
        if(currentBit_>=8)
        {
            currentBit_=0;
            currentByte_++;
        }
    }
    return true;
}

bool BitStream::addBytes(unsigned int toadd,uint8_t bytenumber)
{
    if(bytenumber==0)
        return true;
    if(currentByte_>=maxSize_)
        return false;
    if(currentBit_!=0)
        return false;

    uint8_t bytes[bytenumber];
    memset(bytes,0,bytenumber);
    std::copy(static_cast<const char*>(static_cast<const void*>(&toadd)),static_cast<const char*>(static_cast<const void*>(&toadd)) + sizeof(bytenumber*8),bytes);

    for(int t=0;t<bytenumber;++t)
    {
        data_[currentByte_]=bytes[t];
        currentByte_++;
    }
    return true;
}

void BitStream::dump()
{
    for(uint8_t current:data_)
    {
        std::cout<<(int)current<<" ";
    }

}