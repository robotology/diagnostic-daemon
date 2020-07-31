 
#include "gtest/gtest.h"
#include "Parser.h"
#include "pugixml.hpp"

TEST(Parser, test001) {
    Parser parser;
    bool check=parser.start();
    EXPECT_EQ(check,true);

    //**Get xml for roptime msg
    std::stringstream ssIn;
    pugi::xml_document tmpIn;
    pugi::xml_parse_result result=tmpIn.load_file("msgready/roptime.xml");
    if(!result)
        std::cout<<result.description()<<std::endl;
    tmpIn.save(ssIn,"\t", pugi::format_raw | pugi::format_no_declaration |  pugi::format_no_escapes);

    //**xml to bit
    std::vector<uint8_t> out;
    check=parser.parse(ssIn.str().c_str(),out);
    EXPECT_EQ(check,true);

    std::vector<uint8_t> in=out;

    //**bit to xml
    pugi::xml_document xmlSupportDoc;
    std::list<std::tuple<std::string,std::string,std::string>> strout=parser.parse(in,xmlSupportDoc);

    //**xml to bit
    std::vector<uint8_t> out2;
    std::stringstream ssIn2;
    check=parser.parse(xmlSupportDoc,out2);
    
    //**Check
    EXPECT_EQ(check,true);
    EXPECT_EQ(out,out2);
      
	parser.dump(strout);
}