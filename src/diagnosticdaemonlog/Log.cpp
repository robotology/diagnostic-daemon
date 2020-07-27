/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

#include "Log.h"

#include <fstream>
#include <filesystem>

Log::Log(Severity severity):severity_(severity)
{
    if(!alreadyReadConfig_)
    {
        if(std::filesystem::exists(logfilekey_))
            logToFile_=true;
        alreadyReadConfig_=true;
    }

    if(!isToBeLogged())                        
        return;
    severityStr_=translator[severity];
};

Log::~Log()
{
    if(!isToBeLogged())                        
        return;            
    std::string tmpStr=severityStr_+ss_.str();
    std::cout<<tmpStr<< std::flush;
    if(logToFile_)
    {
        std::ofstream out("log.log",std::ofstream::app);
        out<<tmpStr<< std::flush;
    }
}; 

Log& Log::operator<<(streamtype)
{
    if(!isToBeLogged())
        return *this;        
    ss_<<std::endl;
    return *this; 
};


bool Log::isToBeLogged() const
{
    if(!active_)
        return false;
    auto it=std::find_if(filter_.begin(),filter_.end(),[&](Severity current){
        if(current==severity_)
            return true;
        return false;
    });
    if(it!=filter_.end())
        return false;
    return true;
}

