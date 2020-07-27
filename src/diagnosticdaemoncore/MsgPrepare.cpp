#include "MsgPrepare.h"
#include "Parser.h"
#include "Log.h"
#include "Syntax.h"

#include <fstream>
#if defined _WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#endif // _WIN32



std::vector<uint8_t> MsgPrepare::prepareFromName(const std::string& name)
{
    std::string toload{confsyntax::msgready};
    toload+=name+".xml";
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(toload.c_str());
    if(result.status == pugi::status_file_not_found)
    {
        Log(Severity::error)<<"config.xml not found"<<std::endl;
        return std::vector<uint8_t>();
    }
    if(result.status != pugi::status_ok)
    {
        Log(Severity::error)<<"config.xml reading"<<std::endl;
        return std::vector<uint8_t>();
    }

    updateByRules(name,doc);

    Parser parser;
    bool res=parser.start();
    if(!res)
    {
        Log(Severity::error)<<"parser start error"<<std::endl;
        return std::vector<uint8_t>();   
    }

    std::vector<uint8_t> out;
    bool check=parser.parse(doc,out);
    if(!check)
    {
        Log(Severity::error)<<"parsing error"<<std::endl;
        return std::vector<uint8_t>();        
    }

    return out;
}

void MsgPrepare::updateByRules(const std::string& name,pugi::xml_document& doc)
{
    //************
    //TODO with XML
    //************

    std::string toload{confsyntax::msgready};
    toload+=name+"_rules.xml";
    
    pugi::xpath_node xnode = doc.select_node("//param[@name='data']");
    pugi::xml_node node=xnode.node();


    unsigned long time_in_micros{0};
#if defined _WIN32
    SYSTEMTIME tv;
    GetLocalTime(&tv);
    time_in_micros = 1000000 * tv.wSecond+ 1000 * tv.wMilliseconds;
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    time_in_micros = 1000000 * tv.tv_sec + tv.tv_usec;
#endif // _WIN32
    std::stringstream ss;
    ss<<time_in_micros;
    std::string str=ss.str();
    node.attribute("value").set_value(str.c_str());
}