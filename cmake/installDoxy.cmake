# ================================
# GENERATING DOXYGEN DOCUMENTATION
# ================================
FIND_PACKAGE(DOXYGEN)

IF(NOT DOXYGEN_FOUND)

  SET(DOXYGEN_GIT_TAG        "Release_1_8_9" )
  SET(DOXYGEN_GIT            "https://github.com/doxygen/doxygen.git" )
  SET(DOXYGEN_INSTALL_DIR ${CMAKE_BINARY_DIR}/external/doxygen/doxygen)

  INCLUDE(ExternalProject)
    
  EXTERNALPROJECT_ADD (doxygen
    PREFIX ${CMAKE_BINARY_DIR}/external/doxygen
    DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/external/doxygen/download
    SOURCE_DIR ${CMAKE_BINARY_DIR}/external/doxygen/source
    GIT_REPOSITORY ${DOXYGEN_GIT}
    GIT_TAG ${DOXYGEN_GIT_TAG}
    URL ${DOXYGEN_URL}
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ${CMAKE_CURRENT_BINARY_DIR}/external/doxygen/source/configure --prefix ${CMAKE_INSTALL_PREFIX}
    INSTALL_DIR ${DOXYGEN_INSTALL_DIR}
    INSTALL_COMMAND ""
  )

  # SET EXECUTABLE
  SET(DOXYGEN_EXECUTABLE ${CMAKE_BINARY_DIR}/external/doxygen/source/bin/doxygen)
  
ENDIF()

