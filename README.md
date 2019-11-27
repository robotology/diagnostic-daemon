# 1. DiagnosticDaemon

## 1.1. Introduction

The DiagnosticDaemon application has been developed to get diagnostic messages from the RTOS boards and redirect them to other applications or write the messages to file or console.

![alt text](img/diagnosticarchitecture.png)
*Deploy diagram*


## 1.2. Installation from source on Linux system

As prerequisite install:

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

Clone the icub-firmware repository
```bash
git clone https://github.com/icub-tech-iit/icub-firmware.git
```
**<i>Temporary agreement</i>**: note that the DiagnosticDaemon,icub-firmware-shared and icub-firmware
repository must be at the same level.

Compile
```bash
cd ~DiagnosticDaemon
mkdir build
cmake ..
make
```


## 1.3. Installation from source on Windows system

Not available for now.

## 1.4. Configuration

The DiagnosticDaemon has a configuration file in xml.

```xml

<configuration>
    <component protocol="udp-broadcast"  name="boards"     rxport="11000" txport="11000" address="10.0.1.1"   mask="255.255.255.0" mode="copy-raw"    filter="all" enable="true"  value="" destination="file file2 console gui gui2"/>  
    <component protocol="udp"            name="gui"        rxport="8000"  txport="9000"  address="127.0.0.1"  mask=""              mode="copy-raw"    filter="all" enable="true" value="" destination="boards"/>
    <component protocol="udp"            name="gui2"       rxport="8001"  txport="9001"  address="127.0.0.1"  mask=""              mode="copy-raw"    filter="all" enable="true" value="" destination="boards"/>
    <component protocol="file"           name="file"       rxport=""      txport=""      address=""           mask=""              mode="copy-parser" filter="all" enable="true"  value="logger.log" destination=""/>
    <component protocol="file"           name="file2"      rxport=""      txport=""      address=""           mask=""              mode="copy-parser" filter="all" enable="true"  value="logger2.log" destination=""/>
    <component protocol="console"        name="console"    rxport=""      txport=""      address=""           mask=""              mode="copy-parser" filter="all" enable="true"  value="" destination="boards"/>
</configuration>

```
At the start the DiagnosticDaemon reads the file and create as many components as described in the
config.xml file.

```xml
<component protocol="udp-broadcast"  name="boards"     rxport="11000" txport="11000" address="10.0.1.1"   mask="255.255.255.0" mode="copy-raw"    filter="all" enable="true"  value="" destination="file file2 console gui gui2"/>
```

The component tag has the following attributes:

protocol:  
name:  
rxport:  
txport:  
address:  
mask:  
mode:  
filter:  
enable:  
value:  
destination:  


