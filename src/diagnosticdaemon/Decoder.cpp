/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame, Luca Tricerri
 * email:   marco.accame@iit.it
*/

#include "Decoder.h"

#include <iostream>

Decoder::Decoder()
: _host(new embot::app::DiagnosticsHost)
{
}

Decoder::~Decoder()
{  
    delete _host; 
}

bool Decoder::init(const Config &config)
{
    if(initted())
    {
        return false;
    }

    if(false == config.isvalid())
    {
        return false;
    }

    _config = config;

    _configdiaghost.ropcapacity = _config.ropcapacity;
    _host->init(_configdiaghost);    

    _initted = true;

    return true; 
}

bool Decoder::initted() const
{
    return _initted;
}

bool Decoder::decode(uint8_t *ropframe, uint16_t sizeofropframe, const embot::eprot::IPv4 &ipv4)
{
    if(!initted())
    {
        std::cout<<"Error Decoder::decode"<<std::endl;
        return false;
    }
    
    embot::utils::Data data(ropframe, sizeofropframe);
    return _host->accept(ipv4, data);        
}

bool Decoder::ropdecode(const embot::eprot::IPv4 &ipv4, const embot::eprot::rop::Descriptor &rop)
{      
    // in here we just print out, hence we use a string // or a std:;string
    char textout[128] = {0};
    
    // i accept only sig<>
    if(embot::eprot::rop::OPC::sig != rop.opcode)
    {
        std::cout<<"Error Decoder::Impl"<<std::endl;
        return false;
    }
    
    switch(rop.id32)
    {
        
        case embot::eprot::diagnostics::InfoBasic::id32:
        {
            embot::eprot::diagnostics::InfoBasic *ib = reinterpret_cast<embot::eprot::diagnostics::InfoBasic*>(rop.value.getU08ptr());

            uint64_t tt = ib->timestamp;
            uint32_t sec = tt/(1000*1000);
            uint32_t tmp = tt%(1000*1000);
            uint32_t msec = tmp / 1000;
            uint32_t usec = tmp % 1000;                    
            const char *text = eoerror_code2string(ib->code);
            char buf[16] = {0};
            if(rop.hassignature() || rop.hastime())
            {
                snprintf(textout, sizeof(textout), "from %s [sig = 0x%x, tim = %lld] -> @ s%d m%d u%d -> %s", ipv4.tostring(buf, sizeof(buf)), rop.signature, rop.time, sec, msec, usec, text);
            }
            else
            {
                snprintf(textout, sizeof(textout), "from %s [no sig, no tim] -> @ s%d m%d u%d: %s", ipv4.tostring(buf, sizeof(buf)), sec, msec, usec, text);
            }
            
            // and use also ib->flags + ib->par16 + ib->par64
            
            std::cout<<textout<<std::endl;
        } 
        break;
        case embot::eprot::diagnostics::Info::id32:
        {
            embot::eprot::diagnostics::Info *info = reinterpret_cast<embot::eprot::diagnostics::Info*>(rop.value.getU08ptr());

            uint64_t tt = info->basic.timestamp;
            uint32_t sec = tt/(1000*1000);
            uint32_t tmp = tt%(1000*1000);
            uint32_t msec = tmp / 1000;
            uint32_t usec = tmp % 1000;                    
            const char *text = eoerror_code2string(info->basic.code);
            const char *mystring = (char*)info->extra;
            char buf[16] = {0};
            if(rop.hassignature() || rop.hastime())
            {
                snprintf(textout, sizeof(textout), "from %s [sig = 0x%x, tim = %lld] -> @ s%d m%d u%d -> %s -- %s", ipv4.tostring(buf, sizeof(buf)), rop.signature, rop.time, sec, msec, usec, text,mystring);
            }
            else
            {
                snprintf(textout, sizeof(textout), "from %s [no sig, no tim] -> @ s%d m%d u%d: %s -- %s", ipv4.tostring(buf, sizeof(buf)), sec, msec, usec, text,mystring);
            }
            
            // and use also ib->flags + ib->par16 + ib->par64
            
            std::cout<<textout<<std::endl;
            break;
        }
        default:
        {
            std::cout<<"unknown"<<std::endl;
        } 
        break;
    }
        
    return true;
}   