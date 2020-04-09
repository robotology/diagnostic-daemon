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
#include "embot_prot_eth_diagnostic.h"

#include <list>
#include <condition_variable>
#include <thread>

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
    std::list<std::string> decode(uint8_t *ropframe, uint16_t sizeofropframe, const embot::prot::eth::IPv4 &ipv4 = {"10.0.1.98"});

    
private:    
    bool _initted {false};
    Config _config {};
    embot::prot::eth::diagnostic::Host _host;
    static bool ropdecode(const embot::prot::eth::IPv4 &ipv4, const embot::prot::eth::rop::Descriptor &rop,void*);
    bool ropdecode(const embot::prot::eth::IPv4 &ipv4, const embot::prot::eth::rop::Descriptor &rop);
    embot::prot::eth::diagnostic::Host::Config _configdiaghost;// { false, 513, ropdecode};

    std::mutex lockDecodedMsg_;
    std::list<std::string> decodedMsg_;

    std::mutex mutexcv_;
    std::condition_variable condVar_; 
    bool msgReady_{false};

    //**Util
    void s_print_string(const std::string &str, embot::prot::eth::diagnostic::TYP errortype);
    void s_eoprot_print_mninfo_status(const embot::prot::eth::IPv4 &ipv4, embot::prot::eth::diagnostic::InfoBasic* infobasic, uint8_t * extra);
    void s_process_CANPRINT(const embot::prot::eth::IPv4 &ipv4, embot::prot::eth::diagnostic::InfoBasic* infobasic);
    void s_process_category_Default(const embot::prot::eth::IPv4 &ipv4, embot::prot::eth::diagnostic::InfoBasic* infobasic, uint8_t * extra);
    void s_process_category_Config(const embot::prot::eth::IPv4 &ipv4, embot::prot::eth::diagnostic::InfoBasic* infobasic, uint8_t * extra);
    const char * s_get_sourceofmessage(embot::prot::eth::diagnostic::InfoBasic* infobasic, uint8_t *address);

};
