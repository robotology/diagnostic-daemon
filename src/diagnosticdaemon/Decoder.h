/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame, Luca Tricerri
 * email:   marco.accame@iit.it
*/

// - brief
//   Decoder for rop msg
//


// - include guard ----------------------------------------------------------------------------------------------------
#ifndef _DECODER_H_
#define _DECODER_H_

#include "embot_prot_eth_diagnostic_Host.h"
#include "embot_prot_eth_diagnostic.h"

class Decoder
{
public:
    struct Config
    {
        constexpr static size_t minropcapacity = 40;
        size_t ropcapacity {384};
        Config() = default;
        constexpr Config(size_t t) : ropcapacity(t) {}
        bool isvalid() const { return ropcapacity >= minropcapacity; }
    };      
    
    Decoder();        
    ~Decoder();

    bool init(const Config &config);
    bool initted() const;        
    bool decode(uint8_t *ropframe, uint16_t sizeofropframe, const embot::prot::eth::IPv4 &ipv4 = {"10.0.1.98"},bool enableYarpLogger=false);
    
private:    
    bool _initted {false};
    Config _config {};
    embot::prot::eth::diagnostic::Host *_host {nullptr};
    embot::prot::eth::diagnostic::Host::Config _configdiaghost { false, 513, ropdecode};
    static bool ropdecode(const embot::prot::eth::IPv4 &ipv4, const embot::prot::eth::rop::Descriptor &rop);
    
    inline static bool enableYarpLogger_{false};
    static void forewardtoYarpLogger(const std::string& data,embot::prot::eth::diagnostic::TYP severity); 
};


#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------
