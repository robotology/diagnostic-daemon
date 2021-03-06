#pragma once

#include "pugixml.hpp"
#include <list>
#include <tuple>

class EOMDiagnosticRopMsg;

class RopParser
{
    public:
        RopParser();
        std::list<std::tuple<std::string,std::string,std::string>> parse(const EOMDiagnosticRopMsg&);
        static void dump(const std::list<std::tuple<std::string,std::string,std::string>>& msg);

    private:
        pugi::xml_document doc_;

        bool checkIfParamIsToBeDeleted(const pugi::xml_node& node);
        void includePreparser();

        uint64_t swapBinary(uint64_t value) const;

        //sintax key
        static constexpr char paramkey_[]{"//param"}; 

        static constexpr char namekey_[]{"name"}; 
        static constexpr char valuekey_[]{"value"};
        static constexpr char sizekey_[]{"size"};
        static constexpr char showkey_[]{"show"};
        static constexpr char conditionalkey_[]{"conditional"};
        static constexpr char conditionnamekey_[]{"conditionname"};
        static constexpr char conditionvaluekey_[]{"conditionvalue"};
        static constexpr char nonekey_[]{"none"};
        static constexpr char encoding_[]{"encoding"};
        static constexpr char littleendian_[]{"littleendian"};
        static constexpr char longdescription_[]{"longdescription"};
};
