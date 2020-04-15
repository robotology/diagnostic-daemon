/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

#pragma once

#include <map>
#include <sstream>

namespace confsintax
{
    static constexpr char configurationfile[]{"./config.xml"}; 
    static constexpr char in[]{"in"};       
    static constexpr char configuration[]{"configuration"}; 
    static constexpr char component[]{"component"}; 
    static constexpr char rxport[]{"rxport"}; 
    static constexpr char txport[]{"txport"}; 
    static constexpr char mode[]{"mode"};
    static constexpr char name[]{"name"};
    static constexpr char address[]{"address"};
    static constexpr char type[]{"type"};
    static constexpr char value[]{"value"};
    static constexpr char mask[]{"mask"};
    static constexpr char enable[]{"enable"};
    static constexpr char destination[]{"destination"};
    static constexpr char rules[]{"rules"};

    static constexpr char udpbroadcast[]{"udp-broadcast"};
    static constexpr char udp[]{"udp"};
    static constexpr char file[]{"file"};
    static constexpr char console[]{"console"};
    static constexpr char disabled[]{"disabled"};
    static constexpr char configurator[]{"configurator"};
    static constexpr char decoder[]{"decoder"};
    static constexpr char yarplogger[]{"yarplogger"};
    static constexpr char config[]{"config"};
};

namespace confmessage
{
    static constexpr char message[]{"message"}; 
    static constexpr char name[]{"name"};
    static constexpr char filtermessageset[]{"filtermessageset"}; 
    static constexpr char destinationaddress[]{"destinationaddress"}; 
    static constexpr char rules[]{"rules"}; 
    
}


inline bool asBool(const std::string& name,const std::map<std::string,std::string>& attributes)
{
    //TODO add check    
    bool tmp;
    std::istringstream(attributes.at(name)) >> std::boolalpha >> tmp;
    return tmp;
}

inline std::string asString(const std::string& name,const std::map<std::string,std::string>& attributes)
{
    //TODO add check
    return attributes.at(name);
}

inline int asInt(const std::string& name,const std::map<std::string,std::string>& attributes)
{
    //TODO add check
    auto str=attributes.at(name);
    return std::stoi(str);
}
