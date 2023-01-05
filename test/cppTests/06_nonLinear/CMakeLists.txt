# Create the application
ADD_EXECUTABLE(testNL ${CMAKE_SOURCE_DIR}/test/cppTests/06_nonLinear/test_nonLinear.cpp)
TARGET_LINK_LIBRARIES(testNL tulip exprtk armadillo)


