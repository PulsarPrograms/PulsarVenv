cmake_minimum_required(VERSION 3.31)
project(PulsarVenv)

set(CMAKE_CXX_STANDARD 26)

add_executable(PulsarVenv
        src/main.cpp
        src/startup/PulsarStartup.cpp
        src/profile/PulsarProfileManager.cpp
        src/core/PulsarCore.cpp
        src/itils/split/split.cpp
        src/commandHandler/CommandHandler.cpp

)

target_include_directories(PulsarVenv PRIVATE
        src/startup
        src/profile
        src/core
        src/include
        src/itils/split
        src/commandHandler/

)

set(CMAKE_CXX_FLAGS "-static -static-libgcc -static-libstdc++ -fexec-charset=UTF-8")
set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-weak")