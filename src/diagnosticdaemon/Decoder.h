/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
*/

// - brief
//   Depot for components
//

#pragma once

#include "DiagnosticsHost.h"
#include "EoError.h"
#include "embot_eprot_diagnostics.h"
#include "embot_eprot_ropframe.h"


// to be placed in the pimpl of the main object and conditionally included
class Decoder
{
public:
    struct Config
    {
        constexpr static size_t minropcapacity = 40;
        size_t ropcapacity {384};
        Config() = default;
        constexpr Config(int t) : ropcapacity(t) {}
        bool isvalid() const { return ropcapacity >= minropcapacity; }
    };      
    
    Decoder();        
    ~Decoder();

    bool init(const Config &config);
    bool initted() const;        
    bool decode(uint8_t *ropframe, uint16_t sizeofropframe, const embot::eprot::IPv4 &ipv4 = {"10.0.1.98"});
    
private:    
    struct Impl;
    Impl *pImpl;        
};

    
inline Decoder *decoder {nullptr};

struct Decoder::Impl
{
    bool _initted {false};
    Config _config {};
    embot::app::DiagnosticsHost *_host {nullptr};
    embot::app::DiagnosticsHost::Config _configdiaghost { false, 513, ropdecode};
     

    Impl() : _host(new embot::app::DiagnosticsHost)
    {
    }
       
    ~Impl()
    {
        delete _host;            
    }
           
    static bool ropdecode(const embot::eprot::IPv4 &ipv4, const embot::eprot::rop::Descriptor &rop)
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
   
       
    bool init(const Config &config)
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
   
    bool initted() const
    {
        return _initted;
    }
       
    bool decode(uint8_t *ropframe, uint16_t sizeofropframe, const embot::eprot::IPv4 &ipv4 = {"10.0.1.98"})
    {
        if(!initted())
        {
            std::cout<<"Error Decoder::decode"<<std::endl;
            return false;
        }
       
        embot::utils::Data data(ropframe, sizeofropframe);
        return _host->accept(ipv4, data);        
    }  
   
};

inline Decoder::Decoder()
: pImpl(new Impl)
{
}

inline Decoder::~Decoder()
{  
    delete pImpl;
}

inline bool Decoder::init(const Config &config)
{
    return pImpl->init(config);
}

inline bool Decoder::initted() const
{
    return pImpl->initted();
}

inline bool Decoder::decode(uint8_t *ropframe, uint16_t sizeofropframe, const embot::eprot::IPv4 &ipv4)
{
    return pImpl->decode(ropframe, sizeofropframe, ipv4);
}
