#include "MsgPrepare.h"
#include "Parser.h"
#include <fstream>
#include "Log.h"
#include <sys/time.h>


std::vector<uint8_t> MsgPrepare::prepareFromName(const std::string& name)
{
    std::string toload="msgready/"+name+".xml";
    
    pugi::xml_document doc;

    pugi::xml_parse_result result = doc.load_file(toload.c_str());
    if(result.status == pugi::status_file_not_found)
    {
    Log(Severity::error)<<"config.xml not found"<<std::endl;
    return std::vector<uint8_t>();;
    }
    if(result.status != pugi::status_ok)
    {
    Log(Severity::error)<<"config.xml reading"<<std::endl;
    return std::vector<uint8_t>();;
    }

    updateByRules(name,doc);
   

    Parser parser;
    std::vector<uint8_t> out;
    bool check=parser.parse(doc,out);

    return out;
}

void MsgPrepare::updateByRules(const std::string& name,pugi::xml_document& doc)
{
    //************
    //TODO with XML
    //************

    std::string toload="msgready/"+name+"_rules.xml";
    
    pugi::xpath_node xnode = doc.select_node("//param[@name='data']");
    pugi::xml_node node=xnode.node();

    struct timeval tv;
    gettimeofday(&tv,NULL);
    unsigned long time_in_micros = 1000000 * tv.tv_sec + tv.tv_usec;
    std::stringstream ss;
    ss<<time_in_micros;
    std::string str=ss.str();
    node.attribute("value").set_value(str.c_str());
}