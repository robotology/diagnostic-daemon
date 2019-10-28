#pragma once

#include <iostream>
#include <fstream>
#include <memory>

#include "Component.h"

class ComponentFile: public Component
{
    public:
        ComponentFile(boost::asio::io_service &io_service,const pugi::xml_node& node,ConfigurationDepot&);
        virtual ~ComponentFile(){};

        void acceptMsg(EOMDiagnosticUdpMsg&) override;

    private:        
        std::ofstream fstream_;

        std::string filename_;

};

using ComponentFile_sptr=std::shared_ptr<ComponentFile>;