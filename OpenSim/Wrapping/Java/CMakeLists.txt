#-----------------------------------------------------------------------------
#SET(CMAKE_DEBUG_POSTFIX "_d" CACHE INTERNAL "" FORCE)

SUBDIRS(OpenSimJNI)

IF(BUILD_JAVA_WRAPPING) 

## Now build jar file from the java files created by SWIG. This depends on finding an installation of Java
FIND_PACKAGE ( Java )
IF ( JAVA_COMPILE )
  # MESSAGE ( STATUS " ===========================================" )
  # MESSAGE ( STATUS "  Java" )
  # MESSAGE ( STATUS " ===========================================" )
  ## 

  GET_FILENAME_COMPONENT (JAVA_BIN_PATH "${JAVA_COMPILE}" PATH)
  FIND_PROGRAM ( JAVA_JAVA_H javah PATHS ${JAVA_BIN_PATH} )

  # MESSAGE ( "JAVA_COMPILE=" ${JAVA_COMPILE} )
  # MESSAGE ( "JAVA_SRC=" ${CMAKE_BINARY_DIR}"/OpenSim/Wrapping/Java/OpenSimJNI/src/org/opensim/modeling/*.java" )
  # MESSAGE ( "WD=" ${CMAKE_BINARY_DIR}"/OpenSim/Wrapping/Java/OpenSimJNI/src/" )

  ADD_CUSTOM_TARGET ( JavaCompile
     COMMAND ${JAVA_COMPILE} 
     ${CMAKE_BINARY_DIR}/OpenSim/Wrapping/Java/OpenSimJNI/src/org/opensim/modeling/*.java 
     -source 1.6 -target 1.6
     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/OpenSim/Wrapping/Java/OpenSimJNI/src/)
     
  ADD_CUSTOM_COMMAND(TARGET JavaCompile
                   POST_BUILD
                   COMMAND ${JAVA_ARCHIVE} -cvf org-opensim-modeling.jar
                   org/opensim/modeling/*.class
                   WORKING_DIRECTORY
                   ${CMAKE_BINARY_DIR}/OpenSim/Wrapping/Java/OpenSimJNI/src/
   )
ENDIF ( JAVA_COMPILE )
ENDIF(BUILD_JAVA_WRAPPING)

CONFIGURE_FILE(Matlab/configureOpenSim.m.in
    "${CMAKE_CURRENT_BINARY_DIR}/configureOpenSim.m" @ONLY)
INSTALL(DIRECTORY Matlab
    DESTINATION "${OPENSIM_INSTALL_MATLABDIR}" 
    PATTERN "Matlab/Dynamic_Walker_Example" EXCLUDE
    PATTERN "Matlab/configureOpenSim.m.in" EXCLUDE
    )
INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/configureOpenSim.m"
    DESTINATION "${OPENSIM_INSTALL_MATLABDIR}/Matlab")

