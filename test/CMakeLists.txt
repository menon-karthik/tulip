#INCLUDE_DIRECTORIES(
#  "${CMAKE_SOURCE_DIR}/src/"
#  "${CMAKE_SOURCE_DIR}/src/actions/"
#  "${CMAKE_SOURCE_DIR}/src/models/"
#  "${CMAKE_SOURCE_DIR}/src/data/"
#  "${CMAKE_SOURCE_DIR}/src/tools/"
#  "${CMAKE_SOURCE_DIR}/src/tools/grSch"
#  "${CMAKE_SOURCE_DIR}/src/tools/poly"
#  "${CMAKE_SOURCE_DIR}/src/tools/simLaunch"
#  "${CMAKE_SOURCE_DIR}/src/tools/spgrid"
#  "${CMAKE_SOURCE_DIR}/src/tools/uq")

# =======================
# SELECT TESTS TO INCLUDE
# =======================
OPTION(test_runModels "Run LPN Models with defualt parameter set" ON)

# ==========================
# SET TARGET LOCATION TO BIN
# ==========================
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ==========
# RUN MODELS
# ==========
IF(test_runModels)
  ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/test/01_runModel)
ENDIF()
