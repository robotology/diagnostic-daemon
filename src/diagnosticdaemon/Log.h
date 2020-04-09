/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

enum class Severity {debug,trace,info,warning,error,max,fatal,none};

class Log
{
public:
    static inline bool active_{true};
    static inline std::vector<Severity> filter_{};

	Log(Severity severity):severity_(severity)
    {
        if(!isToBeLogged())                        
            return;
		severityStr_=translator[severity];
	};

    ~Log()
    {
        if(!isToBeLogged())                        
            return;            
		std::string tmpStr=severityStr_+ss_.str();
		std::cout<<tmpStr<< std::flush;
    }; 

	using streamtype = std::ostream&(std::ostream&);
    Log& operator<<(streamtype)
    {
        if(!isToBeLogged())
            return *this;        
        ss_<<std::endl;
        return *this; 
    };

    template<typename T> Log& operator<<(const T& data)
    {
        if(!isToBeLogged())
            return *this;        
        ss_<<data;
        return *this; 
    };

private:
    std::stringstream ss_;
	std::string severityStr_;
    Severity severity_;
	static inline std::map<Severity,std::string> translator{{Severity::debug,"DEBUG::"},{Severity::error,"ERROR::"},{Severity::trace,"TRACE::"},{Severity::none,""}};

    bool isToBeLogged() const
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

};