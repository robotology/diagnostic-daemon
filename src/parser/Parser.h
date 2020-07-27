/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

// - brief
//   Bit stream class
//

#pragma once

#include "pugixml.hpp"
#include <list>
#include <tuple>
#include <string>
#include <vector>
#include <sstream>

class Parser
{
    public:
        Parser();
        static constexpr unsigned int maxMsgLenght_{1500};

        std::list<std::tuple<std::string,std::string,std::string>> parse(const std::vector<uint8_t>& bytetData,pugi::xml_document& xmlSupportDoc/*supporto e debug*/);//Bit to struct (and for debug xmlDoc)
        bool parse(const std::string& xmlStr,std::vector<uint8_t>& bytetData);        //Xml string to byte
        bool parse(const pugi::xml_document& xmlDoc,std::vector<uint8_t>& bytetData); //Xml document to byte
        
        static void dump(const std::list<std::tuple<std::string,std::string,std::string>>& msg);

    private:
        static constexpr unsigned int maxMsgLen_{50};
        static constexpr char msgroot_[]{"msgdepot/udpheader.xml"};
        pugi::xml_document doc_;

        void visit(pugi::xml_node& node,std::list<std::tuple<std::string,std::string,std::string>>& msg,const std::vector<uint8_t>& rop,uint16_t &byteindex,uint8_t &bitindex);
        void fillNodeWithValue(std::list<std::tuple<std::string,std::string,std::string>>& msg,const std::vector<uint8_t>& rop,pugi::xml_node& node,uint16_t &byteindex,uint8_t &bitindex);
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
};
