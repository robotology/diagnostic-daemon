/* Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

#pragma once
#include "Syntax.h"
#include <map>

enum class ComponentType: uint8_t
{
    udpbroadcast,
    udp,
    file,
    console,
    disabled,
    decoder,
    yarplogger,
    synch,
    config
};

inline std::map<std::string,uint8_t> componentTypeLookup{
                                                        {confsyntax::udpbroadcast,(uint8_t)ComponentType::udpbroadcast},
                                                        {confsyntax::udp,(uint8_t)ComponentType::udp},
                                                        {confsyntax::file,(uint8_t)ComponentType::file},
                                                        {confsyntax::console,(uint8_t)ComponentType::console},
                                                        {confsyntax::disabled,(uint8_t)ComponentType::disabled},
                                                        {confsyntax::decoder,(uint8_t)ComponentType::decoder},
                                                        {confsyntax::yarplogger,(uint8_t)ComponentType::yarplogger},
                                                        {confsyntax::synch,(uint8_t)ComponentType::synch},
                                                        {confsyntax::config,(uint8_t)ComponentType::config}
                                                        };

template <typename T> static std::vector<T> tokenize(const std::string& toTokenize) {
    std::vector<T> out;
    std::istringstream ss{ toTokenize };
    out = std::vector<T>{ std::istream_iterator<T>{ss},std::istream_iterator<T>() };
    return out;
}                                                     