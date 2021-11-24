# Project name
#project(tdd VERSION 0.1.0 LANGUAGES CXX)


#include(GNUInstallDirs)
#FILE(GLOB HDRS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
#FILE(GLOB SRCS *.cc)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(TargetName tests.exapp)

add_executable(${TargetName}
	diagnostic/diagnostic.cxx
	main.cxx
)

add_dependencies(${TargetName}
		vxio_util
		vxio_conio++
)

list(
    APPEND INCLUDE_PATHS
    "${UTIL_INCLUDE_DIR}"
    "${CONIO_INLUDE_DIR}"
#    "${SPP_INCLUDE_DIR}"
	"${TEST_INCLUDE_DIR}"
)

target_include_directories(${TargetName} PUBLIC
        "$<BUILD_INTERFACE:${INCLUDE_PATHS}>"
        )

target_link_libraries(${TargetName}
		${CMAKE_DL_LIBS}
		vxio_util
		vxio_conio++
)
target_compile_features(${TargetName} PUBLIC cxx_std_17)

IF (EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json")
    EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
            ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
            )
ENDIF ()


install(TARGETS ${TargetName}
        RUNTIME DESTINATION bin/${PROJECT_NAME}
        )

#SET(EXPR "\\'A = sin pi/2\;\\'")
#
#
#add_custom_command(TARGET ${TargetName} POST_BUILD
#    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${TargetName}
#
#        "--Test=\\'Util::String::Words\\' ${CMAKE_INSTALL_PREFIX}"
#        "--Test=\\'Util::App\\' --File=${HOME}/applications/bin/logs/App-Test.log"
#        "--Test=\\'Lexer::TokenScan\\' ${EXPR}"
#
#    WORKING_DIRECTORY ${CMAKE_CURRENT_BIN_DIR}
#    COMMENT "Running ${TargetName}..."
#)

