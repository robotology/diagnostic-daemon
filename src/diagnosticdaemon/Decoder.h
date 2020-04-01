/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame, Luca Tricerri
 * email:   marco.accame@iit.it
*/

// - brief
//   Decoder for rop msg
//

#pragma once

#include "embot_prot_eth_diagnostic_Host.h"
#include "EoError.h"
#include "embot_prot_eth_diagnostic.h"
#include "embot_prot_eth_ropframe.h"


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
    bool decode(uint8_t *ropframe, uint16_t sizeofropframe, const embot::prot::eth::IPv4 &ipv4 = {"10.0.1.98"});
    
private:    
    bool _initted {false};
    Config _config {};
    embot::prot::eth::diagnostic::Host *_host {nullptr};
    embot::prot::eth::diagnostic::Host::Config _configdiaghost { false, 513, ropdecode};
    static bool ropdecode(const embot::prot::eth::IPv4 &ipv4, const embot::prot::eth::rop::Descriptor &rop);
};
