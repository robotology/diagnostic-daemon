 
#include "gtest/gtest.h"
#include "Parser.h"

TEST(Parser1, test1) {
    Parser parser;

    std::stringstream ss;
     ss<<"<udpheader name='udpheader'> <param name='startcode' size='1' value='120' longdescription='start code byte1' show='true'/>"
        "<param name='startcode' size='1' uom='byte' value='86' longdescription='start code byte2' show='true'/>" 
        "<param name='startcode' size='1' uom='byte' value='52' longdescription='start code byte3' show='true'/> "
        "<param name='startcode' size='1' uom='byte' value='18' longdescription='start code byte4' show='true'/> "
        "<param name='sizeofbody' size='2' uom='byte' encoding='littleendian' value='16' longdescription='size of body' show='true'/>" 
        "<param name='numberofrops' size='2' uom='byte' encoding='littleendian' value='1' longdescription='number of rops' show='true'/>" 
        "<param name='sequencenum' size='8' uom='byte' encoding='littleendian' value='3' longdescription='sequence number' show='true'/>" 
        "<param name='ageofframe' size='8' uom='byte' encoding='littleendian' value='4' longdescription='age of frame' show='true'/>" 
        
        "<param name='ctrl-ver' size='2' uom='bit' value='0' longdescription='ctrl-ver' show='true'/>"
        "<param name='ctrl-rqstconf' size='1' uom='bit' value='0' longdescription='ctrl-rqstconf' show='true'/>"
        "<param name='ctrl-rqsttime' size='1' uom='bit' value='0' longdescription='ctrl-rqsttime' show='true'/>"
        "<param name='ctrl-plussign' size='1' uom='bit' value='0' longdescription='ctrl-plussign' show='true'/>"
        "<param name='ctrl-plustime' size='1' uom='bit' value='0' longdescription='ctrl-plustime' show='true'/>"
        "<param name='ctrl-confinfo' size='2' uom='bit' value='0' longdescription='ctrl-confinfo' show='true'/>"
        "<param name='ropc' size='1' value='3' uom='byte' longdescription='ropc' show='true'/>"
        "<param name='datasize' size='2' uom='byte' encoding='littleendian' value='8' encoding='littleendian' longdescription='datasize' show='true'/>"
        "<param name='id' size='4' value='65541' uom='byte' encoding='littleendian' longdescription='id' show='true'/>"
        "<param name='data' conditionorvalue='2 3 4 5' conditionname='ropc' conditionallevelup='0' conditionallength='datasize' size='8' value='123' encoding='littleendian' longdescription='id' show='true'/>"
        
        "<param name='stopcode' size='1' uom='byte' value='18' longdescription='stop code byte1' show='true'/>" 
        "<param name='stopcode' size='1' uom='byte' value='52' longdescription='stop code byte2' show='true'/>"
        "<param name='stopcode' size='1' uom='byte' value='86' longdescription='stop code byte3' show='true'/>" 
        "<param name='stopcode' size='1' uom='byte' value='120' longdescription='stop code byte4' show='true'/>" 
        "</udpheader>";
    
    std::vector<uint8_t> out;
    bool check=parser.parse(ss.str().c_str(),out);

    std::array<uint8_t,Parser::maxMsgLenght_> in;
    std::copy_n(out.begin(),out.size(),in.begin());

    std::list<std::tuple<std::string,std::string,std::string>> strout=parser.parse(in);
    parser.dump(strout);
        
	EXPECT_EQ(check,true);
}