 
#include "gtest/gtest.h"

#include <memory>

#include "ComponentUdp.h"
#include "ComponentSynch.h"
#include "ConfigurationDepot.h"

TEST(Component, test003) {
    boost::asio::io_service ios;
    ConfigurationDepot depot(ios);
    {
        auto receiver=std::make_shared<ComponentUdp>(ios,std::map<std::string,std::string>({{"name","receiver"},{"address","0.0.0.0"},{"txport","9878"},{"rxport","9876"},{"broadcast","false"}}),depot);
        auto sender=std::make_shared<ComponentUdp>(ios,std::map<std::string,std::string>({{"name","synchboard"},{"address","0.0.0.0"},{"txport","9876"},{"rxport","9877"},{"broadcast","true"}}),depot);
        auto sync=std::make_shared<ComponentSynch>(std::map<std::string,std::string>({{"name","synch"},{"destination","synchboard"},{ "synchtime","2000"}}),depot);
        depot.addComponent(receiver);
        depot.addComponent(sender);
        depot.addComponent(sync);
    
        std::chrono::steady_clock::duration timeout{std::chrono::seconds(4)};
        ios.run_for(timeout);

        auto msg=receiver->getRxData();
        std::string leftStr(std::begin(msg), std::end(msg));
        leftStr=leftStr.substr(0,5);
        std::string rightStr{"xV4\x12\x10"};

        EXPECT_EQ(leftStr,rightStr);

        depot.clearComponent();
    }
    //std::this_thread::sleep_for(std::chrono::seconds(1));
}