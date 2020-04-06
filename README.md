<!-- TOC -->

- [1. DiagnosticDaemon](#1-diagnosticdaemon)
  - [1.1. Introduction](#11-introduction)
  - [1.2. Installation from source on Linux system](#12-installation-from-source-on-linux-system)
    - [1.2.1. YarpLogger ready](#121-yarplogger-ready)
  - [1.3. Execution](#13-execution)
  - [1.4. Default configuration](#14-default-configuration)
    - [1.4.1. Available components](#141-available-components)
  - [1.5. Tags meaning](#15-tags-meaning)
  - [1.6. Use with YarpLogger](#16-use-with-yarplogger)
  - [1.7. Application structure](#17-application-structure)

<!-- /TOC -->

# 1. DiagnosticDaemon

## 1.1. Introduction

The DiagnosticDaemon application has been developed to get diagnostic messages from the RTOS boards, from yarprobotinterface and from any other udp source and redirect them to other applications or write the messages to file or console.

<figure style="display:block;margin-left:auto;margin-right:auto;width:80%">
    <img src="img/diagnosticarchitecture.png" >
    <figcaption><b>Deploy diagram</b></figcaption>
</figure>


## 1.2. Installation from source on Linux system

Prerequisite:

```bash
sudo apt-get install build-essential g++ cmake libboost-system-dev
```
Clone the DiagnosticDaemon repository:

```bash
git clone https://github.com/icub-tech-iit/diagnostic-daemon.git
```

Clone the icub-firmware-shared repository
```bash
git clone https://github.com/icub-tech-iit/icub-firmware-shared.git
```

Compile the icub-firmware-shared:
```bash
cd icub-firmware-shared
mkdir build
cd build
cmake ..
make

```

Compile:
```bash
cd diagnostic-daemon
mkdir build
cd build
cmake ..
make

```

### 1.2.1. YarpLogger ready
If you also need also the YarpLogger connection make sure to select in ccmake the following:

```                                       
COMPILE_WITHYARP                 ON                                                                                                                                             
```                                                

## 1.3. Execution

To execute the application:
```bash
cd diagnostic-daemon/build/bin
./diagnosticdaemon
```

## 1.4. Default configuration

The DiagnosticDaemon has a configuration file in xml called config.xml.
The file must be located in the same folder of the diagnosticdaemon executable file.
Here is the default file:

```xml
<configuration>
    <component protocol="udp-broadcast"  name="boards"     rxport="11000" txport="11000" address="10.0.1.1"   mask="255.255.255.0" mode="copy-raw"    addressfilter="x:10.0.1.4 i:10.0.1.1" enable="true"  value="" destination="file file2 console gui gui2"/>  
    <component protocol="udp"            name="gui"        rxport="8000"  txport="9000"  address="127.0.0.1"  mask=""              mode="copy-raw"    addressfilter="" enable="false"  value="" destination="boards"/>
    <component protocol="udp"            name="gui2"       rxport="8001"  txport="9001"  address="127.0.0.1"  mask=""              mode="copy-raw"    addressfilter="" enable="false"  value="" destination="boards"/>
    <component protocol="file"           name="file"       rxport=""      txport=""      address=""           mask=""              mode="copy-parser" addressfilter="" enable="true"  value="logger.log" destination=""/>
    <component protocol="file"           name="file2"      rxport=""      txport=""      address=""           mask=""              mode="copy-parser" addressfilter="" enable="true"  value="logger2.log" destination=""/>
    <component protocol="console"        name="console"    rxport=""      txport=""      address=""           mask=""              mode="copy-parser" addressfilter="" enable="true"  value="" destination="boards"/>
</configuration>

```
At the start the DiagnosticDaemon reads the file and create as many components as described in the
config.xml file. In this case the following UML diagram describe the created objects:

<figure style="display:block;margin-left:auto;margin-right:auto;width:80%;border:solid 1px">
    <img src="img/diagnosticdaemonobj.png" >
    <figcaption><i>UML Object diagram</i></figcaption>
</figure>

The message flow through the instantiated components:

<figure style="display:block;margin-left:auto;margin-right:auto;width:80%;border:solid 1px">
    <img src="img/diagnosticdaemonmsgz.png" >
    <figcaption><i>UML msg diagram</i></figcaption>
</figure>
In the figure the message flow from RTOS and yarprobotinterface to the file/console/gui.

### 1.4.1. Available components
**upd or udp-broadcast**: send and receive data on udp connection
**console**: write data on console and accept input
**file**: write data on file

## 1.5. Tags meaning

An example component in the config.xml:

```xml
<component protocol="udp-broadcast"  name="boards"     rxport="11000" txport="11000" address="10.0.1.1"   mask="255.255.255.0" mode="copy-raw"    filter="all" enable="true"  value="" destination="file file2 console gui gui2"/>
```

The component tag has the following attributes:

| parameter name | parameter type |parameter value|component| note |
|---------|--------|--------|-----|--------|
|protocol|enum|udp/file/console||component type|
|name|string||all| component name|
|rxport|number||udp|rx ip port|
|txport|number||udp|tx ip port|
|address|ipaddress||udp|ip destination address|
|mask|ipaddress||||
|mode|enum|copy-raw|all|copy the messages to destination with no modifications|
|rules|string|x:1.1.1.1 i:1.1.1.1 x:all i:none|udp|excluded messages from this addresses|
|enable|boolean|true/false|all|enable or disable the component|
|value|string||file|various uses, for file the file name|
|destination|string||udp|messages destination components name|
|yarplogger|boolean|true/false|console|activate in consol component forward to yarplogger|

## 1.6. Use with YarpLogger
DiagnosticDaemon can forward data to YarpLogger.
To eneble this feature use the parameter yarplogger in a protocol="consol"
component.

```xml
enableyarplogger="true"
```

Also before execute DiagnosticDaemon use in the same shell:

```bash
export YARP_FORWARD_LOG_ENABLE=1
```

## 1.7. Application structure

In figure the application class diagram.

<figure style="display:block;margin-left:auto;margin-right:auto;width:80%;border:solid 1px">
    <img src="img/diagnosticdaemonclass.png" >
    <figcaption><i>UML class diagram</i></figcaption>
</figure>
