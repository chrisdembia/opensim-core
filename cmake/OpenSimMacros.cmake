include(CMakeParseArguments)

# Create an OpenSim API library. Here are the arguments:
# VENDORLIB: If this is a vendor library, specify "VENDORLIB" as the first
#   argument. Otherwise, omit.
# LOWERINCLUDEDIRNAME: When installing the headers for this library, make the
#   name of the library all lower-case (e.g., Lepton -> lepton).
# KIT: Name of the library (e.g., Common).
# AUTHORS: A string listing authors of the library.
# LINKLIBS: List of libraries (targets) to link against.
# INCLUDES: List of header files for the library (obtain via file(GLOB ...)).
# SOURCES: List of cpp files for the library (obtain via file(GLOB ...)).
# TESTDIRS: List of subdirectories that contain tests (and a CMakeLists.txt).
# INCLUDEDIRS (optional): Affects how header files are installed. Use this if
#   the library directory contains subdirectories with header files. If this is
#   the case, this variable should be a list of those subdirectories (relative
#   paths). See OpenSim/Simulation/CMakeLists.txt for an example. If omitted,
#   all the headers specified under INCLUDES are installed into the same
#   directory in the installation tree.
#
# Here's an example from OpenSim/Common/CMakeLists.txt:
#
#   OpenSimAddLibrary(
#       KIT Common
#       AUTHORS "Clay_Anderson-Ayman_Habib_and_Peter_Loan"
#       LINKLIBS ${Simbody_LIBRARIES}
#       INCLUDES ${INCLUDES}
#       SOURCES ${SOURCES}
#       TESTDIRS "Test"
#       )
function(OpenSimAddLibrary)

    # Parse arguments.
    # ----------------
    # http://www.cmake.org/cmake/help/v2.8.9/cmake.html#module:CMakeParseArguments
    set(options VENDORLIB LOWERINCLUDEDIRNAME)
    set(oneValueArgs KIT AUTHORS)
    set(multiValueArgs LINKLIBS INCLUDES SOURCES TESTDIRS INCLUDEDIRS)
    cmake_parse_arguments(
        OSIMADDLIB "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    string(TOUPPER "${OSIMADDLIB_KIT}" OSIMADDLIB_UKIT)


    # Version stuff.
    # --------------
    set(OSIMADDLIB_LIBRARY_NAME osim${OSIMADDLIB_KIT})

    add_definitions(
        -DOPENSIM_${OSIMADDLIB_UKIT}_LIBRARY_NAME=${OSIMADDLIB_LIBRARY_NAME}
        -DOPENSIM_${OSIMADDLIB_UKIT}_MAJOR_VERSION=${OPENSIM_MAJOR_VERSION}
        -DOPENSIM_${OSIMADDLIB_UKIT}_MINOR_VERSION=${OPENSIM_MINOR_VERSION}
        -DOPENSIM_${OSIMADDLIB_UKIT}_BUILD_VERSION=${OPENSIM_PATCH_VERSION}
        -DOPENSIM_${OSIMADDLIB_UKIT}_COPYRIGHT_YEARS="2005-2014"
        -DOPENSIM_${OSIMADDLIB_UKIT}_AUTHORS="${AUTHORS}"
        -DOPENSIM_${OSIMADDLIB_UKIT}_TYPE="Shared"
        -DOPENSIM_COMMON_LIBRARY_NAME=${OSIMADDLIB_LIBRARY_NAME}
        -DOPENSIM_COMMON_MAJOR_VERSION=${OPENSIM_MAJOR_VERSION}
        -DOPENSIM_COMMON_MINOR_VERSION=${OPENSIM_MINOR_VERSION}
        -DOPENSIM_COMMON_BUILD_VERSION=${OPENSIM_PATCH_VERSION}
        -DOPENSIM_COMMON_COPYRIGHT_YEARS="2005-2014"
        -DOPENSIM_COMMON_AUTHORS="${AUTHORS}"
        -DOPENSIM_COMMON_TYPE="Shared"
        -DOPENSIM_SIMULATION_LIBRARY_NAME=${OSIMADDLIB_LIBRARY_NAME}
        -DOPENSIM_SIMULATION_MAJOR_VERSION=${OPENSIM_MAJOR_VERSION}
        -DOPENSIM_SIMULATION_MINOR_VERSION=${OPENSIM_MINOR_VERSION}
        -DOPENSIM_SIMULATION_BUILD_VERSION=${OPENSIM_PATCH_VERSION}
        -DOPENSIM_SIMULATION_COPYRIGHT_YEARS="2005-2014"
        -DOPENSIM_SIMULATION_AUTHORS="${AUTHORS}"
        -DOPENSIM_SIMULATION_TYPE="Shared"
        -DOPENSIM_ACTUATORS_LIBRARY_NAME=${OSIMADDLIB_LIBRARY_NAME}
        -DOPENSIM_ACTUATORS_MAJOR_VERSION=${OPENSIM_MAJOR_VERSION}
        -DOPENSIM_ACTUATORS_MINOR_VERSION=${OPENSIM_MINOR_VERSION}
        -DOPENSIM_ACTUATORS_BUILD_VERSION=${OPENSIM_PATCH_VERSION}
        -DOPENSIM_ACTUATORS_COPYRIGHT_YEARS="2005-2014"
        -DOPENSIM_ACTUATORS_AUTHORS="${AUTHORS}"
        -DOPENSIM_ACTUATORS_TYPE="Shared"
        -DOPENSIM_ANALYSES_LIBRARY_NAME=${OSIMADDLIB_LIBRARY_NAME}
        -DOPENSIM_ANALYSES_MAJOR_VERSION=${OPENSIM_MAJOR_VERSION}
        -DOPENSIM_ANALYSES_MINOR_VERSION=${OPENSIM_MINOR_VERSION}
        -DOPENSIM_ANALYSES_BUILD_VERSION=${OPENSIM_PATCH_VERSION}
        -DOPENSIM_ANALYSES_COPYRIGHT_YEARS="2005-2014"
        -DOPENSIM_ANALYSES_AUTHORS="${AUTHORS}"
        -DOPENSIM_ANALYSES_TYPE="Shared"
        -DOPENSIM_TOOLS_LIBRARY_NAME=${OSIMADDLIB_LIBRARY_NAME}
        -DOPENSIM_TOOLS_MAJOR_VERSION=${OPENSIM_MAJOR_VERSION}
        -DOPENSIM_TOOLS_MINOR_VERSION=${OPENSIM_MINOR_VERSION}
        -DOPENSIM_TOOLS_BUILD_VERSION=${OPENSIM_PATCH_VERSION}
        -DOPENSIM_TOOLS_COPYRIGHT_YEARS="2005-2014"
        -DOPENSIM_TOOLS_AUTHORS="${AUTHORS}"
        -DOPENSIM_TOOLS_TYPE="Shared"
        )


    # Add the library.
    # ----------------
    # These next few lines are the most important:
    # Specify the directories in OpenSim that contain header files.
    include_directories(${OpenSim_SOURCE_DIR})

    # Create the library using the provided source and include files.
    add_library(${OSIMADDLIB_LIBRARY_NAME} SHARED
        ${OSIMADDLIB_SOURCES} ${OSIMADDLIB_INCLUDES})

    # This target links to the libraries provided as arguments to this func.
    target_link_libraries(${OSIMADDLIB_LIBRARY_NAME} ${OSIMADDLIB_LINKLIBS})

    # This is for exporting classes on Windows.
    if(OSIMADDLIB_VENDORLIB)
	    set(OSIMADDLIB_FOLDER "Vendor Libraries")
    else()
		set(OSIMADDLIB_FOLDER "Libraries")
    endif()
    set_target_properties(${OSIMADDLIB_LIBRARY_NAME} PROPERTIES
       DEFINE_SYMBOL OSIM${OSIMADDLIB_UKIT}_EXPORTS
       FOLDER "${OSIMADDLIB_FOLDER}" # For Visual Studio.
    )

    # Install.
    # --------
    # Shared libraries (dll, so, dylib) are needed at runtime for applications.
    # Windows .lib files and Linux/Mac .a static archives are only needed at
    # link time.
    install(TARGETS ${OSIMADDLIB_LIBRARY_NAME}
        EXPORT OpenSimTargets
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${OPENSIM_INSTALL_ARCHIVEDIR}")

    # Install headers.
    # ----------------
    set(_INCLUDE_PREFIX "${CMAKE_INSTALL_INCLUDEDIR}")
    if(OSIMADDLIB_VENDORLIB)
        set(_INCLUDE_PREFIX ${_INCLUDE_PREFIX}/Vendors)
    else()
        set(_INCLUDE_PREFIX ${_INCLUDE_PREFIX}/OpenSim)
    endif()
    if(OSIMADDLIB_LOWERINCLUDEDIRNAME)
        string(TOLOWER "${OSIMADDLIB_KIT}" OSIMADDLIB_LKIT)
        set(_INCLUDE_LIBNAME ${OSIMADDLIB_LKIT})
    else()
        set(_INCLUDE_LIBNAME ${OSIMADDLIB_KIT})
    endif()
    if(OSIMADDLIB_INCLUDEDIRS)
        foreach(dir ${OSIMADDLIB_INCLUDEDIRS})
            file(GLOB HEADERS ${dir}/*.h) # returns full pathnames
            install(FILES ${HEADERS}
                DESTINATION ${_INCLUDE_PREFIX}/${_INCLUDE_LIBNAME}/${dir})
        endforeach(dir)
    else()
        install(FILES ${OSIMADDLIB_INCLUDES}
            DESTINATION ${_INCLUDE_PREFIX}/${_INCLUDE_LIBNAME})
    endif()


    # Testing.
    # --------
    enable_testing()

    if(BUILD_TESTING)
        foreach(OSIMADDLIB_TESTDIR ${OSIMADDLIB_TESTDIRS})
            add_subdirectory("${OSIMADDLIB_TESTDIR}")
        endforeach()
    endif()

endfunction()

# Copy a file from the directory containing test files (model files, data,
# etc.) to the directory in which a test will be executed. This function makes
# it easy to re-use files that are used in tests. With an easier mechanism for
# re-using these files, we won't end up version-controlling the same file in
# multiple test directories.
#
# Arguments are a list of files in the test resources directory
# (OPENSIM_SHARED_TEST_FILES_DIR) to copy.
function(OpenSimCopySharedTestFiles)
    if(BUILD_TESTING)
        foreach(filename ${ARGN})
            file(COPY "${OPENSIM_SHARED_TEST_FILES_DIR}/${filename}"
                 DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
        endforeach()
    endif()
endfunction()

# Create test targets for this directory.
# TESTPROGRAMS: Names of test CPP files. One test will be created for each cpp
#   of these files.
# DATAFILES: Files necessary to run the test. These will be copied into the
#   corresponding build directory.
# LINKLIBS: Arguments to TARGET_LINK_LIBRARIES.
# SOURCES: Extra source files for the exectuable.
#
# Here's an example:
#   file(GLOB TEST_PROGRAMS "test*.cpp")
#   file(GLOB DATA_FILES *.osim *.xml *.sto *.mot)
#   OpenSimAddTests(
#       TESTPROGRAMS ${TEST_ROGRAMS}
#       DATAFILES ${DATA_FILES}
#       LINKLIBS osimSingle
#       )
function(OpenSimAddTests)

    if(BUILD_TESTING)

        # Parse arguments.
        # ----------------
        # http://www.cmake.org/cmake/help/v2.8.9/cmake.html#module:CMakeParseArguments
        set(options)
        set(oneValueArgs)
        set(multiValueArgs TESTPROGRAMS DATAFILES LINKLIBS SOURCES)
        cmake_parse_arguments(
            OSIMADDTESTS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

        # If EXECUTABLE_OUTPUT_PATH is set, then that's where the tests will be
        # located. Otherwise, they are located in the current binary directory.
        if(EXECUTABLE_OUTPUT_PATH)
            set(TEST_PATH "${EXECUTABLE_OUTPUT_PATH}")
        else()
            set(TEST_PATH "${CMAKE_CURRENT_BINARY_DIR}")
        endif()

        # Make test targets.
        foreach(test_program ${OSIMADDTESTS_TESTPROGRAMS})
            # NAME_WE stands for "name without extension"
            get_filename_component(TEST_NAME ${test_program} NAME_WE)

            add_executable(${TEST_NAME} ${test_program}
                ${OSIMADDTESTS_SOURCES})
            target_link_libraries(${TEST_NAME} ${OSIMADDTESTS_LINKLIBS})
            add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
            set_target_properties(${TEST_NAME} PROPERTIES
                FOLDER "Tests"
				)

        endforeach()

        # Copy data files to build directory.
        foreach(data_file ${OSIMADDTESTS_DATAFILES})
            # This command re-copies the data files if they are modified;
            # custom commands don't do this.
            file(COPY "${data_file}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
        endforeach()

        #if(UNIX)
        #  add_definitions(-fprofile-arcs -ftest-coverage)
        #  link_libraries(gcov)
        #endif(UNIX)

    endif()

endfunction()


# Create an application/executable. To be used in the Appliations directory.
# APPNAME: Name of the application. Must also be the name of the source file
# containing main().
#
# Here's an example:
#   OpenSimAddApplication(forward)
function(OpenSimAddApplication APPNAME)

    include_directories(${OpenSim_SOURCE_DIR} ${OpenSim_SOURCE_DIR}/Vendors)
    add_executable(${APPNAME} ${APPNAME}.cpp)
    target_link_libraries(${APPNAME} osimTools)
    install(TARGETS ${APPNAME} DESTINATION "${CMAKE_INSTALL_BINDIR}")
    set_target_properties(${APPNAME} PROPERTIES
        FOLDER "Applications")

    if(${OPENSIM_USE_INSTALL_RPATH})
        # TODO @executable_path only makes sense on OSX, so if we use RPATH on
        # Linux we'll have to revisit.

        # bin_dir_to_install_dir is most likely "../"
        file(RELATIVE_PATH bin_dir_to_install_dir
            "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}"
            "${CMAKE_INSTALL_PREFIX}")
        set(bin_dir_to_lib_dir
            "${bin_dir_to_install_dir}${CMAKE_INSTALL_LIBDIR}")
        set_target_properties(${APPNAME} PROPERTIES
            INSTALL_RPATH "\@executable_path/${bin_dir_to_lib_dir}"
            )
    endif()

endfunction()
