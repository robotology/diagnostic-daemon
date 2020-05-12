#pragma once
#include <vector>
#include <string>
#include <stdint.h>

class MsgPrepare
{
    public:
        std::vector<uint8_t> prepareFromName(const std::string& name);
};