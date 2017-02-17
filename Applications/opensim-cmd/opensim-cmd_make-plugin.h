#ifndef OPENSIM_CMD_MAKE_PLUGIN_H_
#define OPENSIM_CMD_MAKE_PLUGIN_H_
/* -------------------------------------------------------------------------- *
 *                       OpenSim:  opensim-cmd_make-plugin.h                  *
 * -------------------------------------------------------------------------- *
 * The OpenSim API is a toolkit for musculoskeletal modeling and simulation.  *
 * See http://opensim.stanford.edu and the NOTICE file for more information.  *
 * OpenSim is developed at Stanford University and supported by the US        *
 * National Institutes of Health (U54 GM072970, R24 HD065690) and by DARPA    *
 * through the Warrior Web program.                                           *
 *                                                                            *
 * Copyright (c) 2005-2016 Stanford University and the Authors                *
 * Author(s): Frank C. Anderson, Ayman Habib, Chris Dembia                    *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may    *
 * not use this file except in compliance with the License. You may obtain a  *
 * copy of the License at http://www.apache.org/licenses/LICENSE-2.0.         *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 * -------------------------------------------------------------------------- */

#include <iostream>
#include <fstream>
#include <regex>
#include <locale>

#include <docopt.h>
#include "parse_arguments.h"

#include <OpenSim/OpenSim.h>

static const char HELP_MAKE_PLUGIN[] =
R"(Show description of properties in an OpenSim class.

Usage:
  opensim-cmd [options]... make-plugin [<name> [<class>]...]
  opensim-cmd make-plugin -h | --help

Description:
  TODO

  Loading a plugin has no effect on this subcommand. TODO remove that option,
  and test the behavior when specifying a subcommand.

Examples:
  opensim-cmd make-plugin 
)";

static const std::string templatePluginDLLh = 
R"(#ifndef @PLUGINNAME_UPPER@_DLL_H_
#define @PLUGINNAME_UPPER@_DLL_H_

// IMPORT AND EXPORT
// UNIX
#ifndef WIN32
    #define @PLUGINNAME_UPPER@_API

// WINDOWS
#else

    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <windows.h>
    #ifdef @PLUGGINNAME_UPPER@_EXPORTS
        #define @PLUGGINNAME_UPPER@_API __declspec(dllexport)
    #else
        #define @PLUGGINNAME_UPPER@_API __declspec(dllimport)
    #endif

#endif

#endif // @PLUGINNAME_UPPER@_DLL_H_ 
)";

static const std::string templatePluginDLLcpp =
R"(TODO
)";

int make_plugin(int argc, const char** argv) {

    using namespace OpenSim;

    std::map<std::string, docopt::value> args = OpenSim::parse_arguments(
            HELP_MAKE_PLUGIN, { argv + 1, argv + argc },
            true); // show help if requested

    // No arguments were provided.
    std::string name = args["<name>"] ? args["<name>"].asString() : 
                                         "MyOpenSimPlugin";
    std::string nameUpper = IO::Uppercase(name);

    if (IO::makeDir(name) != 0) {
        // TODO check error code 
        std::cout << "make dir return nonzero" << std::endl;
        // TODO return EXIT_FAILURE;
    }

    // README
    // TODO

    // CMakeLists.txt
    // TODO

    // MyOpenSimPluginDLL.h
    // --------------------
    std::ofstream pluginDLLh(name + "/" + name + "DLL.h");
    std::string contentsPluginDLLh = std::regex_replace(templatePluginDLLh,
            std::regex("@PLUGINNAME_UPPER@"), nameUpper);
    pluginDLLh << contentsPluginDLLh << std::endl;
    pluginDLLh.close();

    // MyOpenSimPluginDLL.cpp TODO not needed?
    // ----------------------
    std::ofstream pluginDLLcpp(name + "/" + name + "DLL.cpp");
    std::string contentsPluginDLLcpp = templatePluginDLLcpp;
    pluginDLLcpp << contentsPluginDLLcpp << std::endl;
    pluginDLLcpp.close();

    // RegisterTypes

    // TODO discover virtual functions?

    return EXIT_SUCCESS;
}

#endif // OPENSIM_CMD_MAKE_PLUGIN_H_
