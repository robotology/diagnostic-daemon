/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

#pragma once

#include <map>

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
    static constexpr char protocol[]{"protocol"};
    static constexpr char value[]{"value"};
    static constexpr char mask[]{"mask"};
    static constexpr char enable[]{"enable"};
    static constexpr char destination[]{"destination"};
    static constexpr char addressfilter[]{"addressfilter"};

    static constexpr char udpbroadcast[]{"udp-broadcast"};
    static constexpr char udp[]{"udp"};
    static constexpr char file[]{"file"};
    static constexpr char console[]{"console"};
    static constexpr char disabled[]{"disabled"};
};

enum class ComponentType: uint8_t
{
    udpbroadcast,
    udp,
    file,
    console,
    disabled
};

inline std::map<std::string,uint8_t> componentTypeLookup{
                                                        {confsintax::udpbroadcast,(uint8_t)ComponentType::udpbroadcast},
                                                        {confsintax::udp,(uint8_t)ComponentType::udp},
                                                        {confsintax::file,(uint8_t)ComponentType::file},
                                                        {confsintax::console,(uint8_t)ComponentType::console},
                                                        {confsintax::disabled,(uint8_t)ComponentType::disabled}
                                                        };