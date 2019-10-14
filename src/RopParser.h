#pragma once

#include "pugixml.hpp"
#include <list>

class EOMDiagnosticRopMsg;

class RopParser
{
    public:
        RopParser();
        void parse(const EOMDiagnosticRopMsg&);
        void dump();

    private:
        std::list<std::pair<std::string,std::string>> msg_;
        pugi::xml_document doc_;
        pugi::xpath_node_set params_;

        bool checkIfParamIsToBeDeleted(const pugi::xml_node& node);
        void includePreparser();

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
};