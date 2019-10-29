#pragma once

namespace confsintax
{
    static constexpr char configurationFile[]{"./config.xml"}; 
    static constexpr char in[]{"in"};       
    static constexpr char component[]{"//component"}; 
    static constexpr char port[]{"port"}; 
    static constexpr char txport[]{"txport"}; 
    static constexpr char mode[]{"mode"};
    static constexpr char name[]{"name"};
    static constexpr char address[]{"address"};
    static constexpr char protocol[]{"protocol"};
    static constexpr char value[]{"value"};
    static constexpr char mask[]{"mask"};
    static constexpr char enable[]{"enable"};
    static constexpr char destination[]{"destination"};

    static constexpr char udpbroadcast[]{"udp-broadcast"};
    static constexpr char udp[]{"udp"};
    static constexpr char file[]{"file"};
    static constexpr char console[]{"console"};
};

enum class ComponentType: uint8_t
{
    udpbroadcast,
    udp,
    file,
    console,
};
inline std::map<std::string,uint8_t> protocolTypeLookup{
                                                        {confsintax::udpbroadcast,(uint8_t)ComponentType::udpbroadcast},
                                                        {confsintax::udp,(uint8_t)ComponentType::udp},
                                                        {confsintax::file,(uint8_t)ComponentType::file},
                                                        {confsintax::console,(uint8_t)ComponentType::console}};