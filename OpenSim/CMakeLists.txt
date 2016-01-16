
set(SIMULATION_SUBDIRS 
        Common
        Simulation
        Simulation/Control
        Simulation/Manager
        Simulation/Model
        Simulation/Wrap
        Simulation/SimbodyEngine
        Actuators
        Analyses
        Tools
        )

set(SOURCES)
set(INCLUDES)
foreach(dir ${SIMULATION_SUBDIRS})
    file(GLOB SOURCES_ONEDIR ${dir}/*.cpp)
    file(GLOB INCLUDES_ONEDIR ${dir}/*.h)
    list(APPEND SOURCES ${SOURCES_ONEDIR})
    list(APPEND INCLUDES ${INCLUDES_ONEDIR})
endforeach()

OpenSimAddLibrary(
    KIT Single
    LINKLIBS ${Simbody_LIBRARIES} osimLepton
    INCLUDES ${INCLUDES}
    SOURCES ${SOURCES}
    TESTDIRS Common/Test
             Simulation/SimbodyEngine
             Simulation/Test
             Actuators/Test
             Tools/Test
    INCLUDEDIRS ${SIMULATION_SUBDIRS}
    )

add_library(osimTools ALIAS osimSingle)
add_library(osimCommon ALIAS osimSingle)
add_library(osimSimulation ALIAS osimSingle)
add_library(osimAnalyses ALIAS osimSingle)
add_library(osimActuators ALIAS osimSingle)

add_subdirectory(Sandbox)
add_subdirectory(Utilities)
add_subdirectory(Examples)
add_subdirectory(Tests)

install(FILES OpenSim.h DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/OpenSim")
