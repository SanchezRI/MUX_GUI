# Backends [ModbusTCP]
----------------------------------------------------------------------------------------------
Cross-platform executable drivers for communication with ModbusTCP devices.
## Requirements
- OS Windows 10 and later
- Linux Ubuntu 20.04 LTS and later
- Compillers: g++, MSVC
- Linkers: WinCMake 3.22, CMake 3.22 and later

## ModbusTCP Object
Entity description here

## Technology Stack
### Dependencies
|Header file     |                                                                                                                                              Description                                                                                                                                                                                                     |Operation sys |
|----------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|--------------|
|**_cstdint_**   |Defines several fixed-width integer types, which are guaranteed to have the same size on any platform:	**int8_t, uint8_t**: 8-bit signed and unsigned integers. **int16_t, uint16_t**: 16-bit signed and unsigned integers.																																|Cross-platform|
|**_vector_**    |It is a container that encapsulates dynamic size arrays. The elements are stored contiguously, one after another. This means that a pointer to an element of a **vector** may be passed to any function that expects a pointer to an element of an array.																										|Cross-platform|
|**_string_**    |Strings in C++ are sequences of characters stored in a char array or managed using the std::string class. The std::string class, defined in the **string** header file, provides a wide range of functions for string manipulation, making it more convenient and powerful than C-style strings.																|Cross-platform|
|**_mutex_**     |A **mutex** (short for mutual exclusion) is a synchronization primitive used to protect shared data from being accessed by multiple threads simultaneously. In C++, the **std::mutex** class is defined in the mutex header file and provides mechanisms to lock and unlock threads, ensuring that only one thread can access the shared resource at a time.	|Cross-platform|
|**_functional_**|The **functional** header in C++ provides a set of tools and utilities for general-purpose function manipulation and invocation. It includes function objects, also known as functors, and various utilities to work with callable entities.																													|Cross-platform|
|**_thread_**    |**Multithreading** is a feature in C++ that allows concurrent execution of multiple parts of a program, maximizing CPU utilization. Each part of such a program is called a **thread**. **Threads** are lightweight processes within a process.																												|Cross-platform|
|**_atomic_**    |The **atomic** header introduces a powerful mechanism for managing concurrent access to shared data in multithreaded applications. This header provides atomic types and operations that ensure safe access to variables, preventing data races and potential issues in multithreaded code.																	|Cross-platform|
|**_sstream_**   |The **sstream** header in C++ provides the stringstream class, which allows you to treat strings as input/output streams. This is particularly useful for parsing, formatting, and converting data between strings and other types.																															|Cross-platform|
|**_iomanip_**   |The **iomanip** header in C++ provides a set of functions to manipulate the format of input and output operations. These functions are part of the input/output library and are used to control the formatting of data.																																		|Cross-platform|
|**_chrono_**    |**chrono** is a C++ header that provides a collection of types and functions to work with time. It is a part of the C++ Standard Template Library (STL) and it's included in C++11 and later versions. **chrono** provides three main types of clocks: system_clock, steady_clock, and high_resolution_clock.													|Cross-platform|
|**_ctime_**     |The **ctime** function in C++ is used to convert a time_t value to a human-readable string representing the local time. It is defined in the **ctime** header file.																																															|Cross-platform|
|**_iostream_**  |The #include **iostream** directive is used in C++ to include the standard input-output stream library. This library allows you to use objects like cin, cout, cerr, and clog for input and output operations.																																				|Cross-platform|
|**_algorithm_** |The **algorithm** header in C++ defines a collection of functions designed to be used on ranges of elements. These functions operate through iterators directly on the values, without affecting the structure of any container.																																|Cross-platform|
|**_imgui_**     |Header of **ImGui** is a bloat-free graphical user interface library for C++. It outputs vertex buffers that you can render in your 3D-pipeline enabled application. It is portable, renderer agnostic and self-contained (no external dependencies).																											|Cross-platform|
|**_winsock2_**  |**winsock2.h** is the header file to be included for winsock functions. **ws2_32.lib** is the library file to be linked with the program to be able to use winsock functions. The **WSAStartup** function is used to start or initialise winsock library.																										|Windows       |
|**_ws2tcpip_**  |The **ws2tcpip.h** header is part of the Windows Sockets 2 (Winsock2) API, which provides advanced networking capabilities for Windows applications. It includes functions and definitions for working with IPv4, IPv6, and other protocol-independent networking features.																					|Windows       |
|**_socket_**    |The **sys/socket.h** header in C++ is a part of the POSIX standard and is essential for network programming on Unix-like operating systems. It provides definitions for the socket API, which allows for communication between processes over a network.																										|Linux         |
|**_in_**        |The **netinet/in.h** header file contains definitions for the internet protocol family. Structure: Multicast filter support is accessed by defining feature test macro **_OPEN_SYS_SOCKET_EXT3**. The feature test also exposes symbols in **sys/socket.h**.																									|Linux         |
|**_inet_**      |The **arpa/inet.h** header shall define the in_addr structure as described in **netinet/in.h**. The **arpa/inet.h** header shall define the **INET_ADDRSTRLEN** and **INET6_ADDRSTRLEN** macros as described in **netinet/in.h**. The following shall be declared as functions, or defined as macros, or both.												|Linux         |
|**_unistd_**    |**unistd.h** is a header file in the C and C++ programming languages that provides access to the **POSIX (Portable Operating System Interface)** operating system API. It is defined by the **POSIX.1** standard, which is the base of the Single Unix Specification.																							|Linux         |

### Class Methods

## Bugs
Here is color management for **_Bug Types_**:
- $${\color{red}RED - Blocker }$$
- $${\color{orange}ORANGE - Critical}$$
- $${\color{yellow}YELLOW - Major }$$
- $${\color{green}GREEN - Minor }$$


Here is color management for **_Fix Statuses_**:
- $${\color{red}RED - just \space found \space bug. \space No \space ideas \space for \space resolving }$$
- $${\color{aqua}AQUA - resolving \space bug. \space Have \space multiple \space solutions \space for \space resolving \space bug. }$$
- $${\color{green}GREEN - after \space testing, bug \space is \space resolved }$$

|Date|Bug Name|Bug Type|Description|Fix Ways|Fix Status|Fix Description|
|----|--------|--------|-----------|--------|----------|---------------|
|30/06/2025|Wrong connection status|$${\color{yellow}Major}$$| Method **connect()** always return true| Change **isConnected()** method's source code |$${\color{green}Resolved}$$|Add additional check of socket state|

