cmake_minimum_required(VERSION 3.22)
project(MUX_GUI)

############################################################
# C++ standart
############################################################
set(CMAKE_CXX_STANDARD 14)

############################################################
# Settings for static linking
############################################################
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
set(CMAKE_VS_PLATFORM_TOOLSET "v143")

############################################################
# For static linking in Release
############################################################
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/INCREMENTAL:NO")

############################################################
# Runtime library MSVC
############################################################
if(MSVC)
    # Setting static linking of CRT
    foreach(flag_var
            CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        if(${flag_var} MATCHES "/MD")
            string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        endif()
    endforeach()
    
    # Additional flags for Release
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /Ob2 /DNDEBUG")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG")
endif()

############################################################
# Windows-specific settings
############################################################
if(WIN32)
    message(STATUS "Building for Windows")
    set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/libs/imgui_api")
    set(IMPLOT_DIR "${CMAKE_SOURCE_DIR}/libs/implot_api")
    set(GLFW_DIR "${CMAKE_SOURCE_DIR}/libs/glfw3")
    set(MODBUS_TCP_DIR "${CMAKE_SOURCE_DIR}/libs/modbus_api")

    # GLFW settings - we have to use static GLFW library
    set(GLFW_LIBRARY "${GLFW_DIR}/lib-vc2022/glfw3_mt.lib") # Use multithreading version
    set(GLFW_INCLUDE_DIR "${GLFW_DIR}/include")

    # System libraries
    set(SYSTEM_LIBS ws2_32.lib advapi32.lib shell32.lib user32.lib gdi32.lib)
endif()

set(INCLUDE_DIR_PATH "${CMAKE_SOURCE_DIR}/include")
set(SOURCE_DIR_PATH "${CMAKE_SOURCE_DIR}/source")
set(RESOURCE_DIR_PATH "${CMAKE_SOURCE_DIR}/resource")

############################################################
# Sourse and resource files
############################################################
add_executable(MUX_GUI
    ${SOURCE_DIR_PATH}/main.cpp                 # Main executable file
    ${MODBUS_TCP_DIR}/modbus_tcp.cpp            # Modbus executable file
    ${SOURCE_DIR_PATH}/RenderSystem.cpp         # Render logic of all windows
    ${SOURCE_DIR_PATH}/GuiWindows.cpp           # Master window and LimeSDR slave window
    ${SOURCE_DIR_PATH}/AdcDacWindow.cpp         # Analog Digital Converter slave window
    ${SOURCE_DIR_PATH}/CommutatorWindow.cpp     # Commutator / Multiplexer slave window
    ${SOURCE_DIR_PATH}/StyleEditorWindow.cpp    # Style Editor Window for all styling
    ${SOURCE_DIR_PATH}/PacketTracerWindow.cpp   # Packet Tracer window
    ${RESOURCE_DIR_PATH}/resources.rc           # Icons and other stuff
)

target_include_directories(MUX_GUI PRIVATE 
    ${INCLUDE_DIR_PATH}
)

############################################################
# Adding ImGui
############################################################
target_sources(MUX_GUI PRIVATE
    ${IMGUI_DIR}/imgui/imgui.cpp
    ${IMGUI_DIR}/imgui/imgui_demo.cpp
    ${IMGUI_DIR}/imgui/imgui_draw.cpp
    ${IMGUI_DIR}/imgui/imgui_tables.cpp
    ${IMGUI_DIR}/imgui/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    ${IMPLOT_DIR}/implot.cpp
    ${IMPLOT_DIR}/implot_demo.cpp
    ${IMPLOT_DIR}/implot_items.cpp
)

############################################################
# Include directories
############################################################
target_include_directories(MUX_GUI PRIVATE
    ${IMGUI_DIR}/imgui
    ${IMGUI_DIR}/backends
    ${IMPLOT_DIR}
    ${GLFW_INCLUDE_DIR}
    ${MODBUS_TCP_DIR}
)

############################################################
# Libraries linking
############################################################
if(WIN32)
    target_link_libraries(MUX_GUI PRIVATE
        ${GLFW_LIBRARY}
        opengl32.lib
        gdi32.lib
        ${SYSTEM_LIBS}
    )

endif()

############################################################
# Additional settings for static build
############################################################
if(MSVC)
    target_compile_definitions(MUX_GUI PRIVATE
        $<$<CONFIG:Release>:GLFW_STATIC>
        $<$<CONFIG:Release>:STATIC_BUILD>
    )
endif()