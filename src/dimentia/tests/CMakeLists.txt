add_executable (dimentia_test dimentia_test.cpp)
target_link_libraries (dimentia_test dimentia)

add_executable (result_types result_types.cpp)
target_link_libraries (result_types dimentia)
