FIND_PACKAGE(DOXYGEN REQUIRED)

# STRING MANIPULATION
STRING (REPLACE ";" " " UQ_SRC_H_SS "${UQ_SRC_H}")
STRING (REPLACE ";" " " UQ_SRC_C_SS "${UQ_SRC_C}")
SET    (DOXY_SRC "${UQ_SRC_C_SS} ${UQ_SRC_H_SS} ${CMAKE_SOURCE_DIR}/docs/Readme.md")

# COPY IMAGE FILES
FILE(COPY ${CMAKE_SOURCE_DIR}/docs/doxy/imgs/ DESTINATION ${PROJECT_BINARY_DIR}/docs/doxy/imgs/)
  
SET( doxyfile_in          ${CMAKE_SOURCE_DIR}/docs/doxy/Doxyfile.in    ) 
SET( doxyfile             ${PROJECT_BINARY_DIR}/docs/doxy/Doxyfile     )
SET( doxy_output_root     ${CMAKE_BINARY_DIR}/docs/doxy                )
SET( doxy_input           ${DOXY_SRC}                                  )
 
CONFIGURE_FILE( ${doxyfile_in} ${doxyfile} @ONLY )

ADD_CUSTOM_TARGET(
  doxydoc
  ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/docs/doxy/Doxyfile
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMENT "Generating API documentation with Doxygen" VERBATIM
)
