# DAQ-Driver
![](https://github.com/Microflown-Technologies/DAQ-Driver/workflows/CI/badge.svg)
![](https://github.com/Microflown-Technologies/DAQ-Driver/workflows/DOCS/badge.svg)

Using this library you can connect to a [Voyager](https://www.microflown.com/products/portable-measuring-systems/voyager/) 
and use it as a DAQ ([Data Acquisition](https://en.wikipedia.org/wiki/Data_acquisition)) device.

## Dependencies
- [CMake](https://github.com/Kitware/CMake) - CMake is a cross-platform, open-source build system generator.
- [IXWebsocket](https://github.com/machinezone/IXWebSocket) - Websocket library used to transport data as messages.
- [zeroconf](https://github.com/yvz/zeroconf) - mDNS library for automatic discovery of devices.
- [Protobuf](https://github.com/protocolbuffers/protobuf) - Google's language-neutral, platform-neutral, extensible mechanism for serializing structured data.
- [Qt Framework](https://github.com/qt) *(optional)* - Used by optional debugger GUI and provides extra functionality.

## Supported platforms
- Linux
- Windows, Visual Studio 2019

## Building
There are also precompiled version of this library available through NuGet or via releases.
### Windows
Make sure you have Visual Studio 2017 installed. Next, run the [GenerateSLN.ps1](Scripts/GenerateSLN.ps1) using PowerShell.  
The script will download any missing dependencies and generate a Visual Studio Solution (SLN).  
You can find in the SLN folder relative to the root of this project. You can include the generated solution in your own project.  

### Linux
Make sure you have the required packages installed, preferably using your distro's package manager.  
On Debian (or Debian based distro's like Ubuntu or Linux Mint) you can run this command to install all needed dependencies.
```{r, engine='bash', count_lines}
sudo apt -y install build-essential cmake libprotoc-dev doxygen graphviz git protobuf-compiler
```
Open a terminal and navigate to the root of this project and run the following commands.
```{r, engine='bash', count_lines}
mkdir Build  
cmake ..  
make -j4  
sudo make install  
```

## Debugger
Optionaly you can also build the debugger. The debugger can be used to check the basic functionality of the DAQ driver.  
To build the debugger you need Qt. You can [download Qt for free]((https://www.qt.io/download)).  
Open the [CMakeList.txt](CMakeList.txt) file using Qt Creator. Next go to the Projects tab, and change the 'BUILD_DEBUGGER' option from OFF to ON.  
If you've configured your environment properly, it should now be possible to compile and run the project.


