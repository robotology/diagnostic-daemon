#include "MsgPrepare.h"
#include "Parser.h"
#include <fstream>
#include "Log.h"


std::vector<uint8_t> MsgPrepare::prepareFromName(const std::string& name)
{
    std::string toload="msgready/"+name+".xml";
    /*
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

    */

    std::ifstream fs(toload);
    std::string xml;
    if(fs) {
        std::ostringstream ss;
        ss << fs.rdbuf();
        xml = ss.str();
    }
    Parser parser;
    std::vector<uint8_t> out;
    bool check=parser.parse(xml,out);

    return out;
}