# Multiplexer GUI (MUX GUI)
_________________________________________________________________________________

## Requirements

<span style="color:green">For execution:</span>
- OS Windows 10 <br>

<span style="color:green">For development:</span>
- MSVC Compiler 14.39
- WinCMake 3.31.2

## Libraries

- [ImGui](https://github.com/ocornut/imgui)
- [ImPlot](https://github.com/epezent/implot)
- [GLFW3](https://www.glfw.org/)
- [OpenGL3](https://www.opengl.org/)
- [ModbusTCP](https://www.simplymodbus.ca/TCP.htm)

## Documentation
See information about ModbusTCP library and compapitable devices:
[Backends](./docs/BACKENDS.md) <br>
[Devices](./docs/DEVICES.md) <br>

## Installation

### Windows Installer
1. Go to [my JINR-GitLab releases page](https://git.jinr.ru/SanchezRIwork/MUX_GUI/-/releases)
2. Download Windows installer: <span style="color:green">***MUX_GUI_{version}_setup.exe***</span>
3. Install to directory which doesn't have any spaces:
   - Example: ``` C:/tango_projects/MUX_GUI```
### Building from source
1. Go to my remote repository [MUX_GUI](https://git.jinr.ru/SanchezRIwork/MUX_GUI/-/tree/master?ref_type=heads) and choose required branch:
   - <span style="color:green">***master***: main branch, stable release of app </span>
   - <span style="color:yellow">***develop***: bug fixes, testing, pre-release of app <br> </span>
   You also can see release tags (Example ***v1.0.0-alpha***)
   - <span style="color:red">***hot-fix***: fast bugs fixes only </span>
2. Clone repository with command ```git clone <repos URL>```
3. Open ```Visual Studio 2022``` -> ```Open Folder``` -> ```path/to/MUX_GUI```
4. Click ```Build``` -> ```Build All``` (If you have problem, regenerate CMake)
5. Go to ```MUX_GUI/out/build/x64-{Release}{Debug}/``` and execute ```MUX_GUI.exe```

## Using

### Getting Started
1. Run <span style="color:green">***MUX_GUI.exe***</span>
2. Manage and control your device (or devices)!

### UserFlow
1. Connect your device to PoE-Switch
2. Click on "Connect" button
3. Change IP-address, port-number and UnitID if this necessary
4. Browse for your MUX below basic parameters
5. Change ports by clicking on relevant buttons
6. When done with device, click on "Disconnect" and close the program

## Gallery

Here is GUI for Commutator Device [Connection Popup 1]:<br>
![MUX GUI Interface 1](./include/images/mux_gui_8.PNG "MUX GUI connection popup 1")<br>
Here is GUI for Commutator Device [Connection Popup 2]:<br>
![MUX GUI Interface 2](./include/images/mux_gui_7.PNG "MUX GUI connection popup 2")<br>
Here is GUI for Commutator Device [MUX (2 &rarr; 6)]:<br>
![MUX GUI Interface 3](./include/images/mux_gui_5.PNG "MUX GUI main window 1")<br>
Here is GUI for Commutator Device [MUX 2x(4 &rarr; 1)]:<br>
![MUX GUI Interface 4](./include/images/mux_gui_6.PNG "MUX GUI main window 2")<br>
Packets between Commutator Device and Master Device:<br>
![MUX_GUI Console](./include/images/packets_mux_gui.png "MUX GUI Console packets")<br>

## Bugs
Here is color management for **_Bug Types_**:
- <span style="color:red">RED - Blocker </span>
- <span style="color:orange">ORANGE - Critical</span>
- <span style="color:yellow">YELLOW - Major </span>
- <span style="color:green">GREEN - Minor</span>

Here is color management for **_Fix Statuses_**:
- <span style="color:red">RED - just fined bug. No ideas for resolving </span>
- <span style="color:aqua">AQUA - resolving bug. Have multiple solutions for resolving bug. </span>
- <span style="color:green">GREEN - after testing, bug is resolved</span>

|Date|Bug Name|Bug Type|Description|Fix Ways|Fix Status|Fix Description|
|----|--------|--------|-----------|--------|----------|---------------|
|29/05/2025|Render blaming|<span style="color:orange">Critical</span>| While connection isn't estabilished, app doesn't respond| Add Progress bar to render |<span style="color:aqua">In Progress</span>|Add additional popup, but with syncronization problem|
|30/06/2025|PT doesn't display all packets|<span style="color:yellow">Major</span>|Communication packets doesn't display on the window only in debug terminal|?|<span style="color:red">Finded</span>|Nothing to describe|
|02/07/2025|Wrong connection status on popup|<span style="color:orange">Critical</span>|GUI displayed wrong successful connection to device|Check logic of connection on frontend|<span style="color:aqua">In Progress</span>|Add new if-else block to **_comm_modbus.connect()_**|