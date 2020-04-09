/* Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

#pragma once
#include "Sintax.h"
#include <map>

enum class ComponentType: uint8_t
{
    udpbroadcast,
    udp,
    file,
    console,
    disabled,
    decoder,
    yarplogger
};

inline std::map<std::string,uint8_t> componentTypeLookup{
                                                        {confsintax::udpbroadcast,(uint8_t)ComponentType::udpbroadcast},
                                                        {confsintax::udp,(uint8_t)ComponentType::udp},
                                                        {confsintax::file,(uint8_t)ComponentType::file},
                                                        {confsintax::console,(uint8_t)ComponentType::console},
                                                        {confsintax::disabled,(uint8_t)ComponentType::disabled},
                                                        {confsintax::decoder,(uint8_t)ComponentType::decoder},
                                                        {confsintax::yarplogger,(uint8_t)ComponentType::yarplogger}
                                                        };

template <typename T> std::vector<T> tokenize(const std::string& value)
{
    std::istringstream iss(value);
    std::vector<std::string> results(std::istream_iterator<std::string>{iss},std::istream_iterator<std::string>());
    return results;
}                                                        