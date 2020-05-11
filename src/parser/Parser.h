#pragma once

#include "pugixml.hpp"
#include <list>
#include <tuple>
#include <array>
#include <string>
#include <vector>
#include <sstream>

class EOMDiagnosticRopMsg;

class Parser
{
    private:
        static constexpr unsigned int maxMsgLenght_{1500};
    public:
        Parser();
        std::list<std::tuple<std::string,std::string,std::string>> parse(const std::array<uint8_t,maxMsgLenght_>&);
        bool parse(const std::string& msg,std::vector<uint8_t>&);
        static void dump(const std::list<std::tuple<std::string,std::string,std::string>>& msg);

    private:
        pugi::xml_document doc_;

        void visit(pugi::xml_node node,std::list<std::tuple<std::string,std::string,std::string>>& msg,const std::array<uint8_t,maxMsgLenght_>& rop,uint16_t &byteindex,uint8_t &bitindex);
        void fillNodeWithValue(std::list<std::tuple<std::string,std::string,std::string>>& msg,const std::array<uint8_t,maxMsgLenght_>& rop,pugi::xml_node& node,uint16_t &byteindex,uint8_t &bitindex);
        bool checkIfParamIsToBeDeleted(const pugi::xml_node& node);
        unsigned int checkRepetitionNumber(const pugi::xml_node& node);
        void updateVariabileLength(pugi::xml_node& node);
        void includePreparser();

        uint64_t swapBinary(uint64_t value) const;
        uint16_t swapBinary(uint16_t value)  const;
        
        static std::vector<std::string> tokenize(const std::string& value)
        {
            std::istringstream iss(value);
            std::vector<std::string> results(std::istream_iterator<std::string>{iss},std::istream_iterator<std::string>());
            return results;
        }

        template <class T>
        constexpr T make_mask(std::size_t len)
        {
            return ((static_cast<T>(1) << len)-1);
        }

        //node and attribute name
        static constexpr char param_[]{"param"}; 
        static constexpr char udpheader_[]{"udpheader"}; 
        
        //sintax key
        static constexpr char namekey_[]{"name"}; 
        static constexpr char valuekey_[]{"value"};
        static constexpr char sizekey_[]{"size"};
        static constexpr char showkey_[]{"show"};
        static constexpr char conditionalkey_[]{"conditional"};
        static constexpr char conditionnamekey_[]{"conditionname"};
        static constexpr char conditionorvaluekey_[]{"conditionorvalue"};
        static constexpr char conditionolevelup_[]{"conditionallevelup"};
        static constexpr char repetitionnamekey_[]{"repetitionname"};
        static constexpr char nonekey_[]{"none"};
        static constexpr char encoding_[]{"encoding"};
        static constexpr char littleendian_[]{"littleendian"};
        static constexpr char longdescription_[]{"longdescription"};
        static constexpr char conditionallengthkey_[]{"conditionallength"};
        static constexpr char uomkey_[]{"uom"};
        static constexpr char bitkey_[]{"bit"};
        static constexpr char bytekey_[]{"byte"};
        
};
