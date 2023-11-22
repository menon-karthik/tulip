ADD_EXECUTABLE(GSI_svZeroD ${CMAKE_SOURCE_DIR}/test/cppTests/11_GSI_svZeroD/test_GSI_svZeroD.cpp)
TARGET_LINK_LIBRARIES(GSI_svZeroD tulip armadillo)
ADD_EXECUTABLE(GSI_svZeroD_distalR ${CMAKE_SOURCE_DIR}/test/cppTests/11_GSI_svZeroD/test_GSI_svZeroD_distalR.cpp)
TARGET_LINK_LIBRARIES(GSI_svZeroD_distalR tulip armadillo)
