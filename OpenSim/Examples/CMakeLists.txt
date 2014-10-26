
SET(BUILD_API_EXAMPLES ON CACHE BOOL "Build examples that are used for the OpenSimAPIExamples distribution")
MARK_AS_ADVANCED(BUILD_API_EXAMPLES)

SET(EXDIRS ExampleMain ControllerExample MuscleExample CustomActuatorExample
    OptimizationExample_Arm26 checkEnvironment)

# This example handles the BUILD_API_EXAMPLES variable on its own.
ADD_SUBDIRECTORY(SymbolicExpressionReporter)

IF (BUILD_API_EXAMPLES)

    FOREACH(dir EXDIRS SimpleOptimizationExample)
        INSTALL(DIRECTORY ${dir}
                DESTINATION "${OPENSIM_INSTALL_APIEXAMPLESDIR}")
    ENDFOREACH()

ELSEIF (BUILD_API_EXAMPLES)

    FOREACH(dir EXDIRS)
        ADD_SUBDIRECTORY(${dir})
    ENDFOREACH()

ENDIF (BUILD_API_EXAMPLES)

INSTALL(DIRECTORY Plugins DESTINATION "${OPENSIM_INSTALL_APIEXAMPLESDIR}")
