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
        bool start();

        std::list<std::tuple<std::string,std::string,std::string>> parse(const std::vector<uint8_t>& bytetData,pugi::xml_document& xmlSupportDoc/*supporto e debug*/);//Bit to struct (and for debug xmlDoc)
        bool parse(const std::string& xmlStr,std::vector<uint8_t>& bytetData);        //Xml string to byte
        bool parse(const pugi::xml_document& xmlDoc,std::vector<uint8_t>& bytetData); //Xml document to byte
        
        static void dump(const std::list<std::tuple<std::string,std::string,std::string>>& msg);

    private:
        static constexpr unsigned int maxMsgLen_{50};
        static constexpr char msgroot_[]{"msgdepot/udpheader.xml"};
        pugi::xml_document doc_;
        bool started_{false};

        void visit(pugi::xml_node& node,std::list<std::tuple<std::string,std::string,std::string>>& msg,const std::vector<uint8_t>& rop,uint16_t &byteindex,uint8_t &bitindex);
        void fillNodeWithValue(std::list<std::tuple<std::string,std::string,std::string>>& msg,const std::vector<uint8_t>& rop,pugi::xml_node& node,uint16_t &byteindex,uint8_t &bitindex);
        bool checkIfParamIsToBeDeleted(const pugi::xml_node& node);
        unsigned int checkRepetitionNumber(const pugi::xml_node& node);
        void updateVariabileLength(pugi::xml_node& node);
        void includePreparser();

        uint64_t swapBinary(uint64_t value) const;
        uint16_t swapBinary(uint16_t value)  const;   
};

template <typename T> static std::vector<T> tokenize(const std::string& toTokenize) 
{
    std::vector<T> out;
    std::istringstream ss{ toTokenize };
    out = std::vector<T>{ std::istream_iterator<T>{ss},std::istream_iterator<T>() };
    return out;
}

template <class T> constexpr T makeMask(std::size_t len)
{
    return ((static_cast<T>(1) << len)-1);
}    
