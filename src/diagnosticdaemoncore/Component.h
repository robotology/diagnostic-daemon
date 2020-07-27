/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

// - brief
//   base class for components
//

#pragma once

#include "Syntax.h"
#include "Log.h"

#include <boost/asio.hpp>
#include <memory>
#include <set>

using boost::asio::ip::udp;

class ConfigurationDepot;

class Component
{
    protected:
        static constexpr unsigned int maxMsgLenght_{1500};        

    public:
        Component(const std::map<std::string,std::string>& attributes,ConfigurationDepot&);
        virtual ~Component(){}

        virtual void acceptMsg(std::array<uint8_t,maxMsgLenght_>& msg,unsigned int size,udp::endpoint senderEndPoint,Severity severity)=0;
        virtual void acceptMsg(std::string&,unsigned int ,udp::endpoint,Severity){};
        std::string getName() const {return name_;}
        const std::map<std::string,std::string> getParameterMap() const;

    protected:

        ConfigurationDepot& depot_;
        const std::map<std::string,std::string> parametersMap_;

        std::string name_;
        std::string type_;
        bool enable_;
        std::string mode_;
        std::string destination_;

};


inline bool asBool(const std::string& name,const std::map<std::string,std::string>& attributes)
{
    if(attributes.find(name)!=attributes.end())
    {  
        bool tmp;
        std::istringstream(attributes.at(name)) >> std::boolalpha >> tmp;
        return tmp;
    }
    Log(Severity::error)<< " Missing:"<<name<<" in xml"<<std::endl;
    return false;
}

inline std::string asString(const std::string& name,const std::map<std::string,std::string>& attributes)
{
    if(attributes.find(name)!=attributes.end())
        return attributes.at(name);
    Log(Severity::error)<< " Missing:"<<name<<" in xml"<<std::endl;
    return "";
}

inline int asInt(const std::string& name,const std::map<std::string,std::string>& attributes)
{
    if(attributes.find(name)!=attributes.end())
    {
        auto str=attributes.at(name);
        return std::stoi(str);
    }
    Log(Severity::error)<< " Missing:"<<name<<" in xml"<<std::endl;    
    return 0;
}

using Component_sptr=std::shared_ptr<Component>;
