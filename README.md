# Multiplexer GUI (MUX GUI)
_________________________________________________________________________________

## Requirements
_________________________________________________________________________________________
For execution:
- OS Windows 10
For development:
- MSVC Compiler 14.39
- WinCMake 3.31.2

## Libraries
________________________________________________________________________________________
- [ImGui](https://github.com/ocornut/imgui)
- [ImPlot](https://github.com/epezent/implot)
- [GLFW3](https://www.glfw.org/)
- [OpenGL3](https://www.opengl.org/)
- [ModbusTCP](https://www.simplymodbus.ca/TCP.htm)

## Installation
_____________________________________________________________________________________________________
### Windows Installer
1. Go to [my JINR-GitLab releases page](https://git.jinr.ru/SanchezRIwork/MUX_GUI/-/releases)
2. Download Windows installer: ***MUX_GUI_{version}_setup.exe***
3. Install to directory which doesn't have any spaces:
   - Example: ``` C:/tango_projects/MUX_GUI```
### Building from source
1. Go to my remote repository [MUX_GUI](https://git.jinr.ru/SanchezRIwork/MUX_GUI/-/tree/master?ref_type=heads) and choose required branch:
   - ***master***: main branch, stable release of app
   - ***develop***: bug fixes, testing, pre-release of app <br>
   You also can see release tags (Example ***v5.2***)
2. Clone repository with command ```git clone <repos URL>```
3. Open ```Visual Studio 2022``` -> ```Open Folder``` -> ```path/to/MUX_GUI```
4. Click ```Build``` -> ```Build All``` (If you have problem, regenerate CMake)
5. Go to ```MUX_GUI/out/build/x64-{Release}{Debug}/``` and execute ```MUX_GUI.exe```

## Using
______________________________________________________________________________________________
### Getting Started
1. Run ***MUX_GUI.exe***
2. Manage and control your device (or devices)!
### UserFlow
1. Connect your device to PoE-Switch
2. Click on "Connect" button
3. Change IP-address, port-number and UnitID if this necessary
4. Browse for your MUX below basic parameters
5. Change ports by clicking on relevant buttons
6. When done with device, click on "Disconnect" and close the program

## Devices
______________________________________________________________________________________________
- [AdcDacDevice repository (Development)]()
- [CommutatorDevice repository (Windows)](https://git.jinr.ru/SanchezRIwork/CommutatorDevice)

## Gallery
______________________________________________________________________________________________
Here is GUI for Commutator Device [MUX (2 &rarr; 6)]<br>:
![MUX GUI Interface](./MUX_GUI/include/images/mux_gui_5.png "MUX GUI main Window")<br>
Here is GUI for Commutator Device [MUX 2x(4 &rarr; 1)]:<br>
![MUX GUI Interface](./MUX_GUI/include/images/mux_gui_6.png "MUX GUI main Window")<br>
Packets between Commutator Device and Master Device:<br>
![MUX_GUI Console](./MUX_GUI/include/images/packets_mux_gui.png "MUX GUI Console packets")<br>

