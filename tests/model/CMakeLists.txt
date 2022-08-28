add_executable(ModelTest main.cpp dc_motor.cpp)
target_link_libraries(ModelTest control gtest pthread)
add_test(NAME ModelTest COMMAND ModelTest)
