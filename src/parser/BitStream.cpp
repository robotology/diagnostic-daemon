#include <iostream>
#include <string.h>
#include <math.h>
#include <vector>
#include <algorithm>

#include "BitStream.h"
#include "syntax.h"


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

bool BitStream::addBytes(unsigned long toadd,uint8_t bytenumber,const std::string& encoding)
{
    if(bytenumber==0)
        return true;
    if(bytenumber>8)
        return false;        
    if(currentByte_>=maxSize_)
        return false;
    if(currentBit_!=0)
        return false;

    uint8_t bytes[bytenumber];
    memset(bytes,0,bytenumber);
    std::copy(static_cast<const char*>(static_cast<const void*>(&toadd)), static_cast<const char*>(static_cast<const void*>(&toadd)) + sizeof(unsigned long), bytes);

    std::vector<uint8_t> bytesVect;
    bytesVect.assign(&bytes[0],&bytes[bytenumber]);
    std::reverse(bytesVect.begin(),bytesVect.end());

    if(encoding==littleendian_)
    {
        uint8_t swap{0};
        switch (bytenumber)
        {
            case 1:
                break;
            case 2:
                swap=bytesVect[0];
                bytesVect[0]=bytesVect[1];
                bytesVect[1]=swap;
            break;
            case 4:
                swap=bytesVect[0];
                bytesVect[0]=bytesVect[3];
                bytesVect[3]=swap;
                swap=bytesVect[2];
                bytesVect[2]=bytesVect[1];
                bytesVect[1]=swap;
            break;
            case 8:
                swap=bytesVect[0];
                bytesVect[0]=bytesVect[7];
                bytesVect[7]=swap;
                swap=bytesVect[1];
                bytesVect[1]=bytesVect[6];
                bytesVect[6]=swap;
                swap=bytesVect[2];
                bytesVect[2]=bytesVect[5];
                bytesVect[5]=swap;
                swap=bytesVect[4];
                bytesVect[4]=bytesVect[3];
                bytesVect[3]=swap;
            break;
            default:
                std::cout<<"Littleendian error:"<<bytenumber<<std::endl;
        }
    }


    for(int t=0;t<bytenumber;++t)
    {
        data_[currentByte_]=bytesVect[t];
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