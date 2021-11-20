cmake_minimum_required(VERSION 3.16..3.20 FATAL_ERROR)
# Project name

project(util VERSION 0.0.1 LANGUAGES CXX)


include(GNUInstallDirs)

SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )

set(TargetName ${FrameWorkName}_${PROJECT_NAME})


add_library(
        ${TargetName}
        SHARED
		cc/iostr.cc  include/vxio/util/iostr.h
		include/vxio/util/dll_config.h
		cc/object.cc include/vxio/util/object.h
		cc/geometry.cc include/vxio/util/geometry.h
		cc/logger.cc include/vxio/util/logger.h cc/rem.cc include/vxio/util/rem.h include/vxio/util/source_location.h)


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
        $<INSTALL_INTERFACE:include/${FrameWorkName}/${PROJECT_NAME}>
        )

target_link_libraries(${TargetName} ${CMAKE_DL_LIBS} )


install(DIRECTORY
        include/${FrameWorkName}/${PROJECT_NAME}/
        DESTINATION "${CMAKE_INSTALL_PREFIX}/include/${FrameWorkName}/${PROJECT_NAME}"
)

install(TARGETS ${TargetName}
		EXPORT ${TargetName}Targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)
