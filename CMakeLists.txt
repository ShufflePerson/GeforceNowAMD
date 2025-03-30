cmake_minimum_required(VERSION 3.15) 

project(GFNAmd
    VERSION 1.0
    LANGUAGES CXX
)

if(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8) 
        message(STATUS "Configuring for Windows 64-bit")
    else()
        message(WARNING "Building on Windows, but not 64-bit architecture detected. Ensure your generator targets Win64 if 64-bit is desired.")
    endif()

    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        message(STATUS "Using MSVC compiler")
    else()
        message(WARNING "Not using MSVC compiler. Ensure you are using a Visual Studio generator for MSVC.")
    endif()
else()
    message(WARNING "This CMakeLists.txt is primarily configured for Windows/MSVC. Adjustments may be needed for other platforms.")
endif()

set(CMAKE_CXX_STANDARD 23) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) 
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)


if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/O2)
    add_compile_options(/GL)
    add_compile_options(/LTCG)
    add_compile_options(/fp:fast)

    if(CMAKE_BUILD_TYPE MATCHES Debug)
        add_compile_options(/fsanitize=address)
        add_link_options(/INFERASANLIBS) 
        message(STATUS "AddressSanitizer enabled for Debug builds.")
    endif()

    add_compile_options(/Zc:twoPhase-)

    add_compile_options(/utf-8)
    message(STATUS "Using UTF-8 character set.")

else() 
    message(WARNING "Compiler is not MSVC. Some compiler flags (warnings as errors, static analysis, etc.) may need to be adjusted for your compiler.")
endif()

function(ignore_include_warnings INCLUDE_DIR)
    file(GLOB_RECURSE INCLUDE_FILES "${INCLUDE_DIR}/*.h" "${INCLUDE_DIR}/*.hpp")
    if(MSVC)
        foreach(INCLUDE_FILE IN LISTS INCLUDE_FILES)
            set_source_files_properties(${INCLUDE_FILE} PROPERTIES COMPILE_FLAGS "/w") 
        endforeach()
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang") 
        foreach(INCLUDE_FILE IN LISTS INCLUDE_FILES)
            set_source_files_properties(${INCLUDE_FILE} PROPERTIES COMPILE_FLAGS "-w") 
        endforeach()
    else()
        message(WARNING "Warning suppression for include directories not implemented for compiler: ${CMAKE_CXX_COMPILER_ID}")
    endif()
endfunction()


file(GLOB_RECURSE SOURCE_FILES
    "src/*.cpp",
)

add_library(GFNAmd SHARED ${SOURCE_FILES})
# add_executable(GFNAmd ${SOURCE_FILES})

set_target_properties(GFNAmd PROPERTIES OUTPUT_NAME "GFNAmd")


set_property(GLOBAL PROPERTY CMAKE_MSVC_ parallel TRUE)

install(TARGETS GFNAmd
    RUNTIME DESTINATION bin
)


message(STATUS "CMake configuration finished.")