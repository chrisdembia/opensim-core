#-----------------------------------------------------------------------------

SUBDIRS(Common Simulation Actuators Analyses Utilities Tools Wrapping Examples Tests doc)

INSTALL(FILES OpenSim.h
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/OpenSim")
