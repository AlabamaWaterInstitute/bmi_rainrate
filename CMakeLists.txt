cmake_minimum_required(VERSION 3.10)

# Uncomment this and rebuild artifacts to enable debugging
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(bmi_rainrate_cpp_model VERSION 1.0.0 DESCRIPTION "BMI C++ Rainrate Model Shared Library")

# https://www.intel.com/content/www/us/en/develop/documentation/oneapi-dpcpp-cpp-compiler-dev-guide-and-reference/top/compiler-setup/use-the-command-line/use-cmake-with-the-compiler.html
if (INTEL_DPCPP)
    cmake_minimum_required(VERSION 3.20)
    find_package(IntelDPCPP REQUIRED)
endif()

if(WIN32)
    add_library(bmi_rainrate_cpp_model src/bmi_rainrate_cpp.cpp)
else()
    add_library(bmi_rainrate_cpp_model SHARED src/bmi_rainrate_cpp.cpp)
endif()

target_include_directories(bmi_rainrate_cpp_model PRIVATE include)
#Where to look for bmi header, depending on where cmake is run from
target_include_directories(bmi_rainrate_cpp_model PRIVATE ../bmi-cxx/ )

set_target_properties(bmi_rainrate_cpp_model PROPERTIES VERSION ${PROJECT_VERSION})

set_target_properties(bmi_rainrate_cpp_model PROPERTIES PUBLIC_HEADER include/bmi_rainrate_cpp.hpp)
set_target_properties(bmi_rainrate_cpp_model PROPERTIES CXX_VISIBILITY_PRESET default)

include(GNUInstallDirs)

install(TARGETS bmi_rainrate_cpp_model
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

configure_file(bmi_rainrate_cpp_model.pc.in bmi_rainrate_cpp_model.pc @ONLY)

install(FILES ${CMAKE_BINARY_DIR}/bmi_rainrate_cpp_model.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)


# Executable for testing the shared library
add_executable(bmi_rainrate_cpp_model_test src/bmi_main.cpp)
target_link_libraries(bmi_rainrate_cpp_model_test bmi_rainrate_cpp_model)
target_include_directories(bmi_rainrate_cpp_model_test PRIVATE include)
target_include_directories(bmi_rainrate_cpp_model_test PRIVATE ../bmi-cxx/ )