<!-- TOC -->

- [1. DiagnosticDaemon (DD)](#1-diagnosticdaemon-dd)
  - [1.1. Introduction](#11-introduction)
  - [1.2. Installation from source on Linux system](#12-installation-from-source-on-linux-system)
    - [1.2.1. YarpLogger ready](#121-yarplogger-ready)
  - [1.3. Execution](#13-execution)
  - [1.4. Default configuration](#14-default-configuration)
    - [1.4.1. Examples](#141-examples)
    - [1.4.2. Available components](#142-available-components)
  - [1.5. Tags meaning](#15-tags-meaning)
  - [1.6. Use with YarpLogger](#16-use-with-yarplogger)
  - [1.7. Application structure](#17-application-structure)
  - [1.8. Configuration messages](#18-configuration-messages)
  - [1.9. Use](#19-use)
  - [1.10. Message list](#110-message-list)
    - [1.10.1. filtermessageset](#1101-filtermessageset)

<!-- /TOC -->

# 1. DiagnosticDaemon (DD)

## 1.1. Introduction

The DiagnosticDaemon (DD) application has been developed to get diagnostic messages from the RTOS boards, from yarprobotinterface and from any other udp source. DiagnosticDaemon redirects the message to other applications or write the messages to file or console. DD works with a list of preloadable components.


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

In this case Yarp should be compiled and installed.  
See https://github.com/robotology/yarp

## 1.3. Execution

To execute the application:
```bash
cd diagnostic-daemon/build/bin
./diagnosticdaemon
```

## 1.4. Default configuration

The DiagnosticDaemon has a configuration file in xml called **config.xml**.
The file must be located in the same folder of the diagnosticdaemon executable file.  
Here is the default file:  


```xml
<configuration>
    <component type="udp"            name="boards"     rxport="11000" txport="11000" address="10.0.1.1"   mask="255.255.255.0" mode="copy-raw"    rules="x:10.0.1.4 i:10.0.1.1" enable="true"  value="" destination="dec gui1 file1"/>  
    <component type="udp"            name="configrx"   rxport="11001" txport="11001" address="127.0.0.1"  mask=""              mode="copy-raw"    rules="" enable="true"  value="" destination="config"/>  
    <component type="udp"            name="gui1"       rxport="8000"  txport="9000"  address="127.0.0.1"  mask=""              mode="copy-raw"    rules="" enable="true"  value="" destination="boards"/>
    <component type="file"           name="file1"      rxport=""      txport=""      address=""           mask=""              mode="copy-parser" rules="" enable="true"  value="logger1.log" destination=""/>
    <component type="file"           name="file2"      rxport=""      txport=""      address=""           mask=""              mode="copy-parser" rules="" enable="true"  value="logger2.log" destination=""/>
    <component type="console"        name="console"    rxport=""      txport=""      address=""           mask=""              mode="copy-parser" rules="" enable="true"  value="" destination=""/>
    <component type="decoder"        name="dec"        rxport=""      txport=""      address=""           mask=""              mode="copy-parser" rules="" enable="true"  value="" destination="file2 console yarp"/>
    <component type="yarplogger"     name="yarp"       rxport=""      txport=""      address=""           mask=""              mode="copy-parser" rules="" enable="true"  value="" destination=""/>
    <component type="config"         name="config"     rxport=""      txport=""      address=""           mask=""              mode="copy-parser" rules="" enable="true"  value="" destination=""/>
</configuration>

```
At the start the DiagnosticDaemon reads the file and create as many components as described in the
config.xml file.  
In this case the following UML diagram describe the created objects:

<figure style="display:block;margin-left:auto;margin-right:auto;width:80%;border:solid 1px">
    <img src="img/diagnosticdaemonobj.png" >
    <figcaption><i>UML Object diagram</i></figcaption>
</figure>  
  
<br/><br/>

The message flow through the instantiated components  (follow step0...3):

<figure style="display:block;margin-left:auto;margin-right:auto;margin_top:20px;width:80%;border:solid 1px">
    <img src="img/diagnosticdaemonmsgz.png" >
    <figcaption><i>UML msg diagram</i></figcaption>
</figure>
In the figure the message flow from RTOS and yarprobotinterface to the file/console/gui.

### 1.4.1. Examples
DD follows various flows.  
**Example #1**:
- Component #1 receives a UDP message
- Component #1 route the message to component #2
- Component #2 send the message to an external application

**Example #2**:
- Component #1 receives a UDP message
- Component #1 route the message to component #2
- Component #2 write message to file

### 1.4.2. Available components
The following components are available.  
  
**upd or udp-broadcast**: send and receive data on udp connection.  
**console**: write data on console and accept keyboard input.  
**file**: write data on file.  
**decoder**: decode message from byte format to string.  
**yarplogger**: send received message to yarplogger.  
**config**: change configuration as it receives a configuration message.

## 1.5. Tags meaning

An example component in the config.xml:

```xml
    <component type="udp"            name="boards"     rxport="11000" txport="11000" address="10.0.1.1"   mask="255.255.255.0" mode="copy-raw"    rules="x:10.0.1.4 i:10.0.1.1" enable="true"  value="" destination="dec gui1 file1"/>  
```

The component tag has the following attributes:

| parameter name | parameter type |parameter value|component| note |
|---------|--------|--------|-----|--------|
|protocol|enum|udp/file/console/yarplogger/config/decoder||component type|
|name|string||all| component name|
|rxport|number||udp|rx ip port|
|txport|number||udp|tx ip port|
|address|ipaddress||udp|ip destination address|
|mask|ipaddress||||
|mode|enum|decode/raw|console|copy the messages to destination with no modifications|
|rules|string|x:1.1.1.1 i:1.1.1.1 x:all i:none|udp|excluded messages from this addresses|
|enable|boolean|true/false|all|enable or disable the component|
|value|string||file|various uses, for file the file name|
|destination|string||udp|messages destination, here the receiver component name|
|yarplogger|boolean|true/false|console|forward to yarplogger|

## 1.6. Use with YarpLogger
DiagnosticDaemon can forward data to YarpLogger.
To eneble this feature use the parameter **enableyarplogger** in a protocol="consol"
component.

```xml
enableyarplogger="true"
```

Also before execute DiagnosticDaemon use in the same shell or in your .bashrc:

```bash
export YARP_FORWARD_LOG_ENABLE=1
export YARP_DEBUG_ENABLE=1
```

Also the DiagnosticDaemon should be compiled with Yarp, see section [YarpLogger ready](#1.2.1.-yarplogger-ready)

## 1.7. Application structure

In figure the application class diagram.

<figure style="display:block;margin-left:auto;margin-right:auto;width:80%;border:solid 1px">
    <img src="img/diagnosticdaemonclass.png" >
    <figcaption><i>UML class diagram</i></figcaption>
</figure>

## 1.8. Configuration messages

## 1.9. Use
All the configuration messages should be send to port udp component connected to a config component.  
In default config.xml the port for configuration is 11001.

## 1.10. Message list

| message name | use | note |
|---------|--------|--------|
|filtermanageset|It can be used for filters setting|none|

### 1.10.1. filtermessageset
This message is used for set the filters in DiagnosticDaemon.

| parameter name | description |values | default |
|---------|--------|--------|--------|
|name|message name|string with message name|empty|
|destinationaddress|Destination address to which the filter is anctivated|dotted string:port|empty|
|type|Type of filter|address|address|
|rules|Rules of exclusion (x:address) and inclusion (i:address)|Special addresses are none and all|x:none i:all|
|propagatetoboard|The rules can be propagated to the boards|ture or false|false|
|persistence|The rules will be saved in the DiagnosticDaemon configuration file|true or false|false|

Example:

```xml
<message name="filtermessageset" type="address" destinationaddress="10.0.1.1 9000" rules="x:10.0.1.4 x:10.0.1.5 i:10.0.1.6" propagatetoboard="false" persistence="false"/>
```

