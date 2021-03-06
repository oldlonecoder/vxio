cmake_minimum_required(VERSION 3.16..3.20 FATAL_ERROR)
# Project name

project(vxio VERSION 0.0.1 LANGUAGES CXX)


include(GNUInstallDirs)

SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )

set(TargetName ${PROJECT_NAME})
SET(FRAMEWORK_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

add_library(
        vxio
        SHARED
        cc/util/object.cc
        cc/util/geometry.cc
        cc/util/iostr.cc
        cc/util/rem.cc
        cc/util/logger.cc
        cc/util/rtdl.cc
        cc/interpret/types.cc
        cc/lexer/token_data.cc
        cc/lexer/lexer.cc
        cc/interpret/alu.cc
        cc/interpret/xio.cc
        cc/interpret/bloc.cc
        cc/vconio/vconio.cc
        cc/vconio/vdc.cc
        cc/interpret/compiler/grammar.cc include/vxio/interpret/compiler/grammar.h
        cc/interpret/vxu.cc include/vxio/interpret/vxu.h
        cc/interpret/compiler/axt.cc include/vxio/interpret/compiler/axt.h
        cc/interpret/compiler/context_data.cc include/vxio/interpret/compiler/context_data.h
        cc/interpret/compiler/vxu.cc_expression.cc
        cc/interpret/compiler/vxu.cc_declvar.cc
        cc/interpret/interpret.cc include/vxio/interpret/interpret.h
        cc/interpret/compiler/vxu.cc_declvar.cc
        cc/interpret/compiler/vxu.cc_expression.cc
        include/vxio/util/expect.h
        cc/db/sqlite/lite.cc include/vxio/db/sqlite/lite.h
		cc/db/schema.cc include/vxio/db/schema.h)


target_compile_definitions(${TargetName} PUBLIC "${TargetName}_DEBUG=$<CONFIG:Debug>")
target_compile_features(${TargetName} PUBLIC cxx_std_17)

include(GenerateExportHeader)
generate_export_header(${TargetName} EXPORT_FILE_NAME ${${PROJECT_NAME}_export})

IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
	EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
			)
ENDIF()

target_include_directories(${TargetName} PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
        )

target_link_libraries(${TargetName} ${CMAKE_DL_LIBS} sqlite3)


add_executable(vxio.app
		main.cc
)
target_link_libraries(vxio.app vxio)
add_dependencies(vxio.app vxio)

install(DIRECTORY
        include/${FrameWorkName}/${PROJECT_NAME}/
        DESTINATION "${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME}"
)

install(TARGETS ${TargetName} vxio.app
		EXPORT ${TargetName}Targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)
