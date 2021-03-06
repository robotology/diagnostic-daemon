 
#include "gtest/gtest.h"

#include "ComponentUdp.h"
#include "ConfigurationDepot.h"
#include <chrono>

TEST(Component, test002) {
    boost::asio::io_service ios;
    ConfigurationDepot depot(ios);

    ComponentUdp receiver(ios,{{"address","0.0.0.0"},{"txport","9878"},{"rxport","9876"},{"broadcast","false"}},depot);
    ComponentUdp sender(ios,{{"address","0.0.0.0"},{"txport","9876"},{"rxport","9877"},{"broadcast","false"}},depot);
    std::string tosend{"test\n"};
    sender.acceptMsg(tosend,4,udp::endpoint(),Severity::none);

    std::chrono::steady_clock::duration timeout{std::chrono::seconds(4)};
    ios.run_for(timeout);

    auto msg=receiver.getRxData();
    std::string str(std::begin(msg), std::end(msg));
    str=str.substr(0,4);
    EXPECT_EQ(str,"test");
}