
SET(BUILD_API_EXAMPLES ON CACHE BOOL "Build examples that are used for the OpenSimAPIExamples distribution")
MARK_AS_ADVANCED(BUILD_API_EXAMPLES)

SET(EXDIRS ExampleMain ControllerExample MuscleExample CustomActuatorExample
    OptimizationExample_Arm26 checkEnvironment)

IF (BUILD_API_EXAMPLES)

    FOREACH(dir EXDIRS SimpleOptimizationExample SymbolicExpressionReporter)
        INSTALL(DIRECTORY ${dir}
                DESTINATION "${OPENSIM_INSTALL_APIEXAMPLESDIR}")
    ENDFOREACH()
    
    ADD_SUBDIRECTORY(SymbolicExpressionReporter)

ELSEIF (BUILD_API_EXAMPLES)

    FOREACH(dir EXDIRS)
        ADD_SUBDIRECTORY(${dir})
    ENDFOREACH()

ENDIF (BUILD_API_EXAMPLES)

INSTALL(DIRECTORY Plugins DESTINATION "${OPENSIM_INSTALL_APIEXAMPLESDIR}")
