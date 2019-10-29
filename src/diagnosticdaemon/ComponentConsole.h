#pragma once

#include <iostream>
#include <memory>
#include <thread>

#include "Component.h"

class ComponentConsole: public Component
{
    public:
        ComponentConsole(boost::asio::io_service &io_service,const pugi::xml_node& node,ConfigurationDepot&);
        virtual ~ComponentConsole(){};

        void acceptMsg(EOMDiagnosticUdpMsg&) override;
        void acceptMsg(std::array<uint8_t,EOMDiagnosticUdpMsg::getSize()>&) override;

    private:
        std::unique_ptr<std::thread> thread_;
        void inputLoop();
        bool active_{true};
};

using ComponentConsole_sptr=std::shared_ptr<ComponentConsole>;