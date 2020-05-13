#pragma once
#include <vector>
#include <string>
#include <stdint.h>
#include "pugixml.hpp"

class MsgPrepare
{
    public:
        std::vector<uint8_t> prepareFromName(const std::string& name);
    private:
        void updateByRules(const std::string& name,pugi::xml_document& doc);
};