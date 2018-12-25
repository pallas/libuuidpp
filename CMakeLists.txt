cmake_minimum_required(VERSION 3.1)

set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
		STRING "Choose the type of build." FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
		"Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

project(libuuidpp)

set(${PROJECT_NAME}_OBJECTS
	uuidpp
	)

set(${PROJECT_NAME}_HEADERS
	)
list(APPEND ${PROJECT_NAME}_HEADERS "${${PROJECT_NAME}_OBJECTS}")

set(${PROJECT_NAME}_TESTS
	test_uuid
	)

add_library(uuidpp OBJECT)
foreach(object ${${PROJECT_NAME}_OBJECTS})
	target_sources(uuidpp PUBLIC ${object}.cc)
endforeach(object)
set_property(TARGET uuidpp PROPERTY POSITION_INDEPENDENT_CODE 1)

add_library(uuidpp-static STATIC $<TARGET_OBJECTS:uuidpp>)
set_target_properties(uuidpp-static PROPERTIES OUTPUT_NAME uuidpp)
add_library(uuidpp-shared SHARED $<TARGET_OBJECTS:uuidpp>)
set_target_properties(uuidpp-shared PROPERTIES OUTPUT_NAME uuidpp SOVERSION 0 VERSION 0.0.0)

find_library(UUID_LIBRARY uuid)
if(UUID_LIBRARY)
	target_link_libraries(uuidpp-static ${UUID_LIBRARY})
	target_link_libraries(uuidpp-shared ${UUID_LIBRARY})
endif()

install(TARGETS
	uuidpp-static
	uuidpp-shared
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	)

add_executable(uuid-filter uuid-filter.cc)
target_link_libraries(uuid-filter uuidpp-shared)
install(TARGETS uuid-filter
	RUNTIME DESTINATION bin
	)

foreach(header ${${PROJECT_NAME}_HEADERS})
	install(FILES ${header}.h DESTINATION "include")
endforeach(header)

enable_testing()
add_custom_target(all_tests)
foreach(test ${${PROJECT_NAME}_TESTS})
	add_executable(${test} EXCLUDE_FROM_ALL ${test}.cc)
	target_link_libraries(${test} uuidpp-static)
	add_test(NAME ${test} COMMAND $<TARGET_FILE:${test}>)
	add_dependencies(all_tests ${test})
endforeach(test)
build_command(CTEST_CUSTOM_PRE_TEST TARGET all_tests)
string(CONFIGURE \"@CTEST_CUSTOM_PRE_TEST@\" CTEST_CUSTOM_PRE_TEST_QUOTED ESCAPE_QUOTES)
file(WRITE "${CMAKE_BINARY_DIR}/CTestCustom.cmake" "set(CTEST_CUSTOM_PRE_TEST ${CTEST_CUSTOM_PRE_TEST_QUOTED})" "\n")
