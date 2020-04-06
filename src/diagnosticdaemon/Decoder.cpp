/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame, Luca Tricerri
 * email:   marco.accame@iit.it
*/

#include "Decoder.h"
#include "Log.h"

#ifdef COMPILE_WITHYARP_DEF 
#include <yarp/os/api.h>
#include <yarp/os/Log.h>
#endif
#include <iosfwd>
#include <iostream>
#include "embot_prot_eth_diagnostic.h"

#include "EoError.h"

extern "C" {
// i give dummy implementation
void* ace_mutex_new(void) { return nullptr; }
int8_t ace_mutex_take(void* , uint32_t ) { return 0; }
int8_t ace_mutex_release(void* ) { return 0; }
void ace_mutex_delete(void* ) {}
double feat_yarp_time_now(void) { return 0; }
}
#warning TODO-acemor: do a function which transform code to string
// later on we shall use a std::string and surely a different text
static const char * geterrormessage(const uint32_t code)
{
    return eoerror_code2string(code);
}


Decoder::Decoder()
: _host(new embot::prot::eth::diagnostic::Host)
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

bool Decoder::decode(uint8_t *ropframe, uint16_t sizeofropframe, const embot::prot::eth::IPv4 &ipv4,bool enableYarpLogger)
{
    Log(Severity::error)<<"forward to Yarp"<<std::endl;
    forewardtoYarpLogger("Pippo",embot::prot::eth::diagnostic::TYP::error);
    enableYarpLogger_=enableYarpLogger;
    if(!initted())
    {
        Log(Severity::error)<<"Decoder::decode"<<std::endl;
        return false;
    }

    embot::core::Data data(ropframe, sizeofropframe);
    return _host->accept(ipv4, data);        
}

static void s_eoprot_print_mninfo_status(const embot::prot::eth::IPv4 &ipv4, embot::prot::eth::diagnostic::InfoBasic * infobasic, uint8_t * extra);

bool Decoder::ropdecode(const embot::prot::eth::IPv4 &ipv4, const embot::prot::eth::rop::Descriptor &rop)
{      
    // in here we just print out, hence we use a string // or a std::string
    char textout[128] = {0};
    
    // i accept only sig<>
    if(embot::prot::eth::rop::OPC::sig != rop.opcode)
    {
        Log(Severity::error)<<"Decoder::Impl"<<std::endl;
        return false;
    }

    embot::prot::eth::diagnostic::TYP severity;
#if 1

    switch(rop.id32)
    {
        case embot::prot::eth::diagnostic::InfoBasic::id32:
        {
            embot::prot::eth::diagnostic::InfoBasic *ib = reinterpret_cast<embot::prot::eth::diagnostic::InfoBasic*>(rop.value.getU08ptr());
            severity=ib->flags.getTYP();
            s_eoprot_print_mninfo_status(ipv4, ib, nullptr);
        } break;
        case embot::prot::eth::diagnostic::Info::id32:
        {
            embot::prot::eth::diagnostic::Info *info = reinterpret_cast<embot::prot::eth::diagnostic::Info*>(rop.value.getU08ptr());
            severity=info->basic.flags.getTYP();
            s_eoprot_print_mninfo_status(ipv4, &info->basic, info->extra);

        } break;

        default:
        {
            s_eoprot_print_mninfo_status(ipv4, nullptr, nullptr);
            Log(Severity::none)<<"unknown"<<std::endl;
        }
        break;
    }

#else
    switch(rop.id32)
    {        
        case embot::prot::eth::diagnostic::InfoBasic::id32:
        {
            embot::prot::eth::diagnostic::InfoBasic *ib = reinterpret_cast<embot::prot::eth::diagnostic::InfoBasic*>(rop.value.getU08ptr());
            severity=ib->flags.getTYP();
            embot::core::TimeFormatter tf(ib->timestamp);
            const char *text = geterrormessage(ib->code);
            char buf[16] = {0};
            if(rop.hassignature() || rop.hastime())
            {
                snprintf(textout, sizeof(textout), "from %s [sig = 0x%x, tim = %ld] -> @ %s -> %s", ipv4.tostring(buf, sizeof(buf)), rop.signature, rop.time, tf.to_string().c_str(), text);
            }
            else
            {    switch(rop.id32)
                {
                    case embot::prot::eth::diagnostic::InfoBasic::id32:
                    {
                        embot::prot::eth::diagnostic::InfoBasic *ib = reinterpret_cast<embot::prot::eth::diagnostic::InfoBasic*>(rop.value.getU08ptr());
                        severity=ib->basic.flags.getTYP();
                        embot::core::TimeFormatter tf(ib->timestamp);
                        const char *text = geterrormessage(ib->code);
                        char buf[16] = {0};
                        if(rop.hassignature() || rop.hastime())
                        {
                            snprintf(textout, sizeof(textout), "from %s [sig = 0x%x, tim = %ld] -> @ %s -> %s", ipv4.tostring(buf, sizeof(buf)), rop.signature, rop.time, tf.to_string().c_str(), text);
                        }
                        else
                        {
                            snprintf(textout, sizeof(textout), "from %s [no sig, no tim] -> @ %s: %s", ipv4.tostring(buf, sizeof(buf)), tf.to_string().c_str(), text);
                        }

            #warning TODO-acemor: it is missing the management of flags, par16 and par64.

                        std::cout<<textout<<std::endl;
                    }
                    break;
                    case embot::prot::eth::diagnostic::Info::id32:
                    {
                        embot::prot::eth::diagnostic::Info *info = reinterpret_cast<embot::prot::eth::diagnostic::Info*>(rop.value.getU08ptr());
                        severity=info->basic.flags.getTYP();
                        embot::core::TimeFormatter tf(info->basic.timestamp);
                        const char *text = geterrormessage(info->basic.code);
                        char buf[16] = {0};
                        if(rop.hassignature() || rop.hastime())
                        {
                            snprintf(textout, sizeof(textout), "from %s [sig = 0x%x, tim = %ld] -> @ %s -> %s -- %s", ipv4.tostring(buf, sizeof(buf)), rop.signature, rop.time, tf.to_string().c_str(), text, info->extra);
                        }
                        else
                        {
                            snprintf(textout, sizeof(textout), "from %s [no sig, no tim] -> @ %s: %s -- %s", ipv4.tostring(buf, sizeof(buf)), tf.to_string().c_str(), text, info->extra);
                        }

            #warning TODO-acemor: it is missing the management of flags, par16 and par64.

                        Log(Severity::none)<<textout<<std::endl;
                        break;
                    }
                    default:
                    {
                        Log(Severity::none)<<"unknown"<<std::endl;
                    }
                    break;
                }
                snprintf(textout, sizeof(textout), "from %s [no sig, no tim] -> @ %s: %s", ipv4.tostring(buf, sizeof(buf)), tf.to_string().c_str(), text);
            }
            
#warning TODO-acemor: it is missing the management of flags, par16 and par64.
            
            std::cout<<textout<<std::endl;
        } 
        break;
        case embot::prot::eth::diagnostic::Info::id32:
        {
            embot::prot::eth::diagnostic::Info *info = reinterpret_cast<embot::prot::eth::diagnostic::Info*>(rop.value.getU08ptr());
            severity=info->basic.flags.getTYP();
            embot::core::TimeFormatter tf(info->basic.timestamp);
            const char *text = geterrormessage(info->basic.code);
            char buf[16] = {0};
            if(rop.hassignature() || rop.hastime())
            {
                snprintf(textout, sizeof(textout), "from %s [sig = 0x%x, tim = %ld] -> @ %s -> %s -- %s", ipv4.tostring(buf, sizeof(buf)), rop.signature, rop.time, tf.to_string().c_str(), text, info->extra);
            }
            else
            {
                snprintf(textout, sizeof(textout), "from %s [no sig, no tim] -> @ %s: %s -- %s", ipv4.tostring(buf, sizeof(buf)), tf.to_string().c_str(), text, info->extra);
            }
            
#warning TODO-acemor: it is missing the management of flags, par16 and par64.
            
            Log(Severity::none)<<textout<<std::endl;
            break;
        }
        default:
        {
            Log(Severity::none)<<"unknown"<<std::endl;
        } 
        break;
    }
#endif

    forewardtoYarpLogger(textout,severity);
        
    return true;
}

void Decoder::forewardtoYarpLogger(const std::string& data,embot::prot::eth::diagnostic::TYP severity)
{
    if(!enableYarpLogger_)
        return;

    severity=severity;
#ifdef COMPILE_WITHYARP_DEF 
dasd
    Log(Severity::error)<<"forward to Yarp"<<std::endl;
    switch (severity)
    {
        case embot::prot::eth::diagnostic::TYP::info:
            yInfo(data.c_str());
            break;
        case embot::prot::eth::diagnostic::TYP::debug:
            yDebug(data.c_str());
            break;
        case embot::prot::eth::diagnostic::TYP::warning:
            yWarning(data.c_str());
            break;
        case embot::prot::eth::diagnostic::TYP::error:
            yError(data.c_str());
            break;
        case embot::prot::eth::diagnostic::TYP::fatal:
        case embot::prot::eth::diagnostic::TYP::max:
            yFatal(data.c_str());
            break;
    }
#endif    
}
// section which prints as yarprobot interface does. just cut'n'paste and make it run

// --------------------------------------------------------------------------------------------------------------------
// - definition of static functions
// --------------------------------------------------------------------------------------------------------------------


#include "EoBoards.h"

static void s_print_string(const std::string &str, embot::prot::eth::diagnostic::TYP errortype);

static void s_eoprot_print_mninfo_status(const embot::prot::eth::IPv4 &ipv4, embot::prot::eth::diagnostic::InfoBasic* infobasic, uint8_t * extra);

static void s_process_CANPRINT(const embot::prot::eth::IPv4 &ipv4, embot::prot::eth::diagnostic::InfoBasic* infobasic);

static void s_process_category_Default(const embot::prot::eth::IPv4 &ipv4, embot::prot::eth::diagnostic::InfoBasic* infobasic, uint8_t * extra);

static void s_process_category_Config(const embot::prot::eth::IPv4 &ipv4, embot::prot::eth::diagnostic::InfoBasic* infobasic, uint8_t * extra);


static const char * s_get_sourceofmessage(embot::prot::eth::diagnostic::InfoBasic* infobasic, uint8_t *address);



static void s_eoprot_print_mninfo_status(const embot::prot::eth::IPv4 &ipv4, embot::prot::eth::diagnostic::InfoBasic * infobasic, uint8_t * extra)
{
#undef DROPCODES_FROM_LIST
#define CAN_PRINT_FULL_PARSING

    static const eOerror_code_t codecanprint = EOERRORCODE(eoerror_category_System, eoerror_value_SYS_canservices_canprint);
    eOerror_category_t category = eoerror_code2category(infobasic->code);

#if defined(DROPCODES_FROM_LIST)
    static const eOerror_code_t codes2drop_value[] =
    {
        EOERRORCODE(eoerror_category_System, eoerror_value_SYS_canservices_parsingfailure),
        EOERRORCODE(eoerror_category_System, eoerror_value_SYS_canservices_rxmaisbug),
        EOERRORCODE(eoerror_category_System, eoerror_value_SYS_canservices_rxfromwrongboard),
        EOERRORCODE(eoerror_category_System, eoerror_value_SYS_transceiver_rxseqnumber_error)
    };

    static const int codes2drop_number = sizeof(codes2drop_value) / sizeof(eOerror_code_t);

    int i;

    for(i=0; i<codes2drop_number; i++)
    {
        if(codes2drop_value[i] == infobasic->properties.code)
        {
            return;
        }
    }
#endif

    if((nullptr == infobasic))
    {
        s_print_string(std::string("unknown diagnostic message type"), embot::prot::eth::diagnostic::TYP::debug);
        return;
    }

    if(codecanprint == infobasic->code)
    {
        s_process_CANPRINT(ipv4, infobasic);
    }
    else if(eoerror_category_Config == category)
    {
        s_process_category_Config(ipv4, infobasic, extra);
    }
    else
    {
        s_process_category_Default(ipv4, infobasic, extra);
    }

}


static void s_print_string(const std::string &str, embot::prot::eth::diagnostic::TYP errortype)
{
    switch(errortype)
    {
        case embot::prot::eth::diagnostic::TYP::info:
        {
            std::cout << "[INFO] " << str << std::endl;
        } break;

        case embot::prot::eth::diagnostic::TYP::debug:
        {
            std::cout << "[DEBUG] " << str << std::endl;
        } break;

        case embot::prot::eth::diagnostic::TYP::warning:
        {
            std::cout << "[WARNING] " << str << std::endl;
        } break;

        case embot::prot::eth::diagnostic::TYP::error:
        {
            std::cout << "[ERROR] " << str << std::endl;
        } break;

        case embot::prot::eth::diagnostic::TYP::fatal:
        {
            std::cout << "[FATAL] " << str << std::endl;
        } break;

        default:
        {
            std::cout << "[ERROR] " << str << std::endl;
        } break;
    }

}


static void s_process_category_Default(const embot::prot::eth::IPv4 &ipv4, embot::prot::eth::diagnostic::InfoBasic* infobasic, uint8_t * extra)
{
    char str[512] = {0};
    embot::prot::eth::diagnostic::TYP type {embot::prot::eth::diagnostic::TYP::info};
    uint8_t address = 0;
    const char * str_source = nullptr;
    const char * str_code = nullptr;
    const char * str_extra = nullptr;
    uint8_t *p64 = nullptr;

    const char *ethboardstr = "tobesolved";

    static const char nullverbalextra[] = "no extra info despite we are in verbal mode";
    static const char emptyextra[] = ".";
    embot::core::TimeFormatter tf(infobasic->timestamp);


    str_source         = s_get_sourceofmessage(infobasic, &address);
    str_code           = eoerror_code2string(infobasic->code);
    str_extra          = nullptr;

    if(embot::prot::eth::diagnostic::EXT::verbal == infobasic->flags.getEXT())
    {
        str_extra = (nullptr == extra) ? (nullverbalextra) : ((const char *)extra);
    }
    else
    {
        str_extra = emptyextra;
    }

    p64 = (uint8_t*)&(infobasic->par64);


    snprintf(str, sizeof(str), " from BOARD %s (%s), src %s, adr %d, time %s: (code 0x%.8x, par16 0x%.4x par64 0x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x) -> %s + %s",
                                ipv4.to_string().c_str(),
                                ethboardstr,
                                str_source,
                                address,
                                tf.to_string().c_str(),
                                infobasic->code,
                                infobasic->par16,
                                p64[7], p64[6], p64[5], p64[4], p64[3], p64[2], p64[1], p64[0],
                                str_code,
                                str_extra
                                );

    s_print_string(std::string(str), type);
}



static void s_process_CANPRINT(const embot::prot::eth::IPv4 &, embot::prot::eth::diagnostic::InfoBasic* )
{
    s_print_string(std::string("received a CAN PRINT but the handler is not implemented yet"), embot::prot::eth::diagnostic::TYP::debug);
//    feat_CANprint(ipv4, infobasic);
}


static void s_process_category_Config(const embot::prot::eth::IPv4 &ipv4, embot::prot::eth::diagnostic::InfoBasic* infobasic, uint8_t * extra)
{
    char str[512] = {0};
    embot::prot::eth::diagnostic::TYP type {embot::prot::eth::diagnostic::TYP::info};
    embot::core::TimeFormatter tf(infobasic->timestamp);
    eOerror_value_t value = eoerror_code2value(infobasic->code);
    const char *ethboardstr = "tobesolved";

    switch(value)
    {
        case eoerror_value_CFG_candiscovery_started:
        {
            uint16_t maskcan2 = infobasic->par16;
            uint64_t brdnum =     (infobasic->par64 & 0x0000ff0000000000) >> 40;
            const char *canboardname = eoboards_type2string(static_cast<eObrd_type_t>(brdnum));
            uint16_t maskcan1 = (infobasic->par64 & 0xffff000000000000) >> 48;
            eObrd_protocolversion_t prot = {};
            eObrd_firmwareversion_t appl = {};
            uint64_t reqpr =      (infobasic->par64 & 0x000000ffff000000) >> 24;
            uint64_t reqfw =      (infobasic->par64 & 0x0000000000ffffff);
            uint8_t num =0;
            prot.major = reqpr >> 8;
            prot.minor = reqpr & 0xff;
            appl.major = (reqfw >> 16) & 0xff;
            appl.minor = (reqfw >> 8)  & 0xff;
            appl.build = reqfw & 0xff;
            num = eo_common_hlfword_bitsetcount(maskcan1)+eo_common_hlfword_bitsetcount(maskcan2);

            snprintf(str, sizeof(str), " from BOARD %s (%s) @ %s: CAN discovery has started for %d %s boards on (can1map, can2map) = (0x%.4x, 0x%.4x) with target can protocol ver %d.%d and application ver %d.%d.%d.",
                                        ipv4.to_string().c_str(),
                                        ethboardstr,
                                        tf.to_string().c_str(),

                                        num, canboardname,
                                        maskcan1, maskcan2,
                                        prot.major, prot.minor,
                                        appl.major, appl.minor, appl.build
                                        );

            s_print_string(std::string(str), type);

        } break;

        case eoerror_value_CFG_candiscovery_ok:
        {
            uint8_t num = infobasic->par16 & 0x00ff;
            eObool_t fakesearch = (0x0000 == (infobasic->par16 & 0xf000)) ? (eobool_false) : (eobool_true);
            uint64_t brdnum =     (infobasic->par64 & 0x0000ff0000000000) >> 40;
            const char *canboardname = eoboards_type2string(static_cast<eObrd_type_t>(brdnum));
            uint64_t searchtime = (infobasic->par64 & 0xffff000000000000) >> 48;
            eObrd_protocolversion_t prot = {};
            eObrd_firmwareversion_t appl = {};
            uint64_t reqpr =      (infobasic->par64 & 0x000000ffff000000) >> 24;
            uint64_t reqfw =      (infobasic->par64 & 0x0000000000ffffff);
            char strOK[80] = "OK";

            prot.major = reqpr >> 8;
            prot.minor = reqpr & 0xff;
            appl.major = (reqfw >> 16) & 0xff;
            appl.minor = (reqfw >> 8)  & 0xff;
            appl.build = reqfw & 0xff;


            if(eobool_true == fakesearch)
            {
                snprintf(strOK, sizeof(strOK), "OK but FAKE (without any control on CAN w/ get-fw-version<> message)");
            }

            snprintf(str, sizeof(str), " from BOARD %s (%s) @ %s: CAN discovery is %s for %d %s boards with target can protocol ver %d.%d and application ver %d.%d.%d. Search time was %d ms",
                                        ipv4.to_string().c_str(),
                                        ethboardstr,
                                        tf.to_string().c_str(),
                                        strOK,
                                        num, canboardname,
                                        prot.major, prot.minor,
                                        appl.major, appl.minor, appl.build,
                                        (int)searchtime
                                        );

            s_print_string(std::string(str), type);

        } break;

        case eoerror_value_CFG_candiscovery_detectedboard:
        {
            uint64_t brdnum =     (infobasic->par64 & 0x0000ff0000000000) >> 40;
            const char *canboardname = eoboards_type2string(static_cast<eObrd_type_t>(brdnum));
            uint64_t searchtime = (infobasic->par64 & 0xffff000000000000) >> 48;
            eObrd_protocolversion_t prot{0};
            eObrd_firmwareversion_t appl{0};
            uint64_t reqpr =      (infobasic->par64 & 0x000000ffff000000) >> 24;
            uint64_t reqfw =      (infobasic->par64 & 0x0000000000ffffff);
            uint8_t address;
            const char *source = s_get_sourceofmessage(infobasic, nullptr);
            prot.major = reqpr >> 8;
            prot.minor = reqpr & 0xff;
            appl.major = (reqfw >> 16) & 0xff;
            appl.minor = (reqfw >> 8)  & 0xff;
            appl.build = reqfw & 0xff;
            address = infobasic->par16 & 0x000f;


            snprintf(str, sizeof(str), " from BOARD %s (%s) @ %s: CAN discovery has detected a %s board in %s addr %d with can protocol ver %d.%d and application ver %d.%d.%d Search time was %d ms",
                                        ipv4.to_string().c_str(),
                                        ethboardstr,
                                        tf.to_string().c_str(),

                                        canboardname,
                                        source, address,
                                        prot.major, prot.minor,
                                        appl.major, appl.minor, appl.build,
                                        (int)searchtime
                                        );
            s_print_string(std::string(str), type);

        } break;

        case eoerror_value_CFG_candiscovery_boardsmissing:
        {
            uint8_t numofmissing = infobasic->par16 & 0x00ff;
            const char *canboardname = eoboards_type2string(static_cast<eObrd_type_t>(infobasic->par16 >> 8));
            uint64_t searchtime = (infobasic->par64 & 0xffff000000000000) >> 48;
            uint16_t maskofmissing = infobasic->par64 & 0x000000000000ffff;
            const char *source = s_get_sourceofmessage(infobasic, nullptr);
            uint8_t n = 1;
            uint8_t i = 0;

            snprintf(str, sizeof(str), " from BOARD %s (%s) @ %s: CAN discovery after %d ms has detected %d missing %s boards in %s:",
                                        ipv4.to_string().c_str(),
                                        ethboardstr,
                                        tf.to_string().c_str(),

                                        (int)searchtime,
                                        numofmissing,
                                        canboardname,
                                        source
                                        );
            s_print_string(std::string(str), type);

            for(i=1; i<15; i++)
            {
                if(eobool_true == eo_common_hlfword_bitcheck(maskofmissing, i))
                {
                    snprintf(str, sizeof(str), "%d of %d: missing %s BOARD %s:%s:%d",
                                                n, numofmissing, canboardname,
                                                ipv4.to_string().c_str(), source, i
                                                );
                    s_print_string(std::string(str), type);
                    n++;

                }
            }

        } break;

        case eoerror_value_CFG_candiscovery_boardsinvalid:
        {
            uint8_t numofinvalid = infobasic->par16 & 0x00ff;
            const char *canboardname = eoboards_type2string(static_cast<eObrd_type_t>(infobasic->par16 >> 8));
            uint64_t invalidmask = infobasic->par64;
            const char *source = s_get_sourceofmessage(infobasic, nullptr);
            uint8_t n = 1;
            uint8_t i = 0;
            const char *empty = "";
            const char *wrongtype = "WRONG BOARD TYPE";
            const char *wrongprot = "WRONG PROTOCOL VERSION";
            const char *wrongappl = "WRONG APPLICATION VERSION";

            snprintf(str, sizeof(str), " from BOARD %s (%s) @ %s: CAN discovery has detected %d invalid %s boards in %s:",
                                        ipv4.to_string().c_str(),
                                        ethboardstr,
                                        tf.to_string().c_str(),

                                        numofinvalid,
                                        canboardname,
                                        source
                                        );
            s_print_string(std::string(str), type);



            for(i=1; i<15; i++)
            {
                uint64_t val = (invalidmask >> (4*i)) & 0x0f;
                if(0 != val)
                {
                    snprintf(str, sizeof(str), "%d of %d: wrong %s BOARD %s:%s:%d because it has: %s %s %s",
                                                n, numofinvalid, canboardname,
                                                ipv4.to_string().c_str(), source, i,
                                                ((val & 0x1) == 0x1) ? (wrongtype) : (empty),
                                                ((val & 0x2) == 0x2) ? (wrongappl) : (empty),
                                                ((val & 0x4) == 0x4) ? (wrongprot) : (empty)
                                                );
                    s_print_string(std::string(str), type);
                    n++;

                }
            }

        } break;

        default:
        {
            s_process_category_Default(ipv4, infobasic, extra);

        } break;

        case EOERROR_VALUE_DUMMY:
        {
            snprintf(str, sizeof(str), " from BOARD %s (%s) @ %s: unrecognised eoerror_category_Config error value:",
                                    ipv4.to_string().c_str(),
                                    ethboardstr,
                                    tf.to_string().c_str()
                                    );
            s_print_string(std::string(str), type);

        } break;

    }

}




static const char * s_get_sourceofmessage(embot::prot::eth::diagnostic::InfoBasic* infobasic, uint8_t *address)
{
    static const char * const sourcenames[] =
    {
        "LOCAL",
        "CAN1",
        "CAN2",
        "UNKNOWN"
    };


    uint8_t source = static_cast<uint8_t>(embot::core::tointegral(infobasic->flags.getSRC()));

    if(nullptr != address)
    {
        *address = static_cast<uint8_t>(embot::core::tointegral(infobasic->flags.getADR()));
    }

    return((source > embot::core::tointegral(embot::prot::eth::diagnostic::SRC::can2)) ? (sourcenames[3]) : (sourcenames[source]));
}

// end of file

