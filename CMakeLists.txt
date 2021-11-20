#
cmake_minimum_required(VERSION 3.16..3.20 FATAL_ERROR)
## Project name
project(vxio.framework VERSION 0.0.1 LANGUAGES CXX)
set(FrameWorkName "vxio")
message ("**************** " ${FrameWorkName} " Master CMake ********************")
#
option(BUILD_UTIL "Enable building the vxio utilities library" ON)
option(BUILD_DIAGNOSTIC "Enable building the diagnostics Library" OFF)
#option(BUILD_LOGGER "Enable building the tea++.club logger utility library" OFF) # disable; moved into the application book library/module
option(BUILD_VAULT "Enable building the vault/[sqlite3,...] Database support  library" OFF)
option(BUILD_XIO "Enable building the vxio  interpreter library" OFF)
option(BUILD_APPBOOK "Enable building the tea++.club application/journal book library" OFF)
option(BUILD_CUI "Enable building the Console User Interface library" OFF)
#option(BUILD_TESTS "Build the tests application based on the diagnostics framework" OFF)
option(BUILD_TEAAPP "Build Interractive GUI vxio XIO Application for testings" OFF)
#

SET(FRAMEWORK_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

message("Framework Root path:" ${FRAMEWORK_ROOT})
if(BUILD_UTIL)
    SET(UTIL_INCLUDE_DIR ${FRAMEWORK_ROOT}/util/include)
    add_subdirectory(util)
endif()

if(BUILD_DIAGNOSTIC)
    SET(DIAGNOSTIC_INCLUDE_DIR ${FRAMEWORK_ROOT}/diagnostic/include)
    add_subdirectory(diagnostic)
endif()


if(BUILD_VAULT)
    SET(VAULT_INCLUDE_DIR ${FRAMEWORK_ROOT}/vault/include)
    add_subdirectory(vault)
endif()
#
if(BUILD_XIO)
    SET(XIO_INCLUDE_DIR ${FRAMEWORK_ROOT}/s++/include)
    add_subdirectory(s++)
endif()
#


#
if(BUILD_CUI)

    SET(CUI_INCLUDE_DIR ${FRAMEWORK_ROOT}/cui++/include)
    add_subdirectory(cui++)
endif()
#
#

if(BUILD_TEAAPP)
    add_subdirectory(tea++.app)
endif()
#
if(BUILD_APPBOOK)
   SET(APPBOOK_INCLUDE_DIR ${FRAMEWORK_ROOT}/appbook/include)
   add_subdirectory(appbook)
endif()
