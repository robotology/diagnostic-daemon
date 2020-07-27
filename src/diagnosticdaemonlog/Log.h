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

enum class Severity {debug,trace,info,warning,error,fatal,none};

class Log
{
public:
    static inline bool active_{true};
    static inline std::vector<Severity> filter_{};

	Log(Severity severity);

    ~Log();

	using streamtype = std::ostream&(std::ostream&);
    Log& operator<<(streamtype);//std::endl only

    template<typename T> Log& operator<<(const T& data)//All other case
    {
        if(!isToBeLogged())
            return *this;        
        ss_<<data;
        return *this; 
    };

private:
    static constexpr char logfilekey_[] = "pleaselogtofile";
    std::stringstream ss_;
	std::string severityStr_;
    Severity severity_;
	static inline std::map<Severity,std::string> translator{{Severity::debug,"DEBUG::"},{Severity::error,"ERROR::"},{Severity::warning,"WARNING::"},{Severity::info,"INFO::"},{Severity::fatal,"FATAL::"},{Severity::trace,"TRACE::"},{Severity::none,""}};
    inline static bool logToFile_{false};
    inline static bool alreadyReadConfig_{false};
    bool isToBeLogged() const;
};