SET (this_target test_extrapolator)
project(${this_target})
file(GLOB src *.cpp)
link_directories(${Boost_LIBRARIES} /usr/local/lib)
if(NOT MSVC)
	set(test_libs boost_test_exec_monitor-mt)
endif()
foreach(TEST ${src})
    get_filename_component(TEST_NAME ${TEST} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_NAME})
    target_link_libraries(${TEST_NAME} extrapolator ${test_libs})
    add_test(NAME ${TEST_NAME} COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME})
endforeach()
