#-----------------------------------------------------------------------------

SUBDIRS(Common Simulation Actuators Analyses Utilities Tools Wrapping Examples Tests doc)

<<<<<<< HEAD
INSTALL(FILES OpenSim.h
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/OpenSim")
=======
INSTALL_FILES(/sdk/include/OpenSim OpenSim.h)
>>>>>>> master
