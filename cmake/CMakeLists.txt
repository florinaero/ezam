cmake_minimum_required(VERSION 3.0.0)
project(ezam VERSION 0.1.0)
configure_file(EzamConfig.h.in EzamConfig.h)


include(CTest)
enable_testing()

add_executable(ezam main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
