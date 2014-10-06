#ifndef OPENSIM_ALL_COORDINATES_ACTUATOR_H_
#define OPENSIM_ALL_COORDINATES_ACTUATOR_H_
/* -------------------------------------------------------------------------- *
 *                       OpenSim:  AllCoordinatesActuator.h                   *
 * -------------------------------------------------------------------------- *
 * The OpenSim API is a toolkit for musculoskeletal modeling and simulation.  *
 * See http://opensim.stanford.edu and the NOTICE file for more information.  *
 * OpenSim is developed at Stanford University and supported by the US        *
 * National Institutes of Health (U54 GM072970, R24 HD065690) and by DARPA    *
 * through the Warrior Web program.                                           *
 *                                                                            *
 * Copyright (c) 2005-2014 Stanford University and the Authors                *
 * Author(s): Chris Dembia                                                    *
 * Contributor(s): Soha Pouya, Michael Sherman                                *
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


#include <OpenSim/Actuators/osimActuatorsDLL.h>
#include <OpenSim/Simulation/Model/Actuator.h>

namespace OpenSim { 

//==============================================================================
//                          ALL COORDINATES ACTUATOR
//==============================================================================
/**
 * An actuator that applies generalized forces for all coordinates (mobilities
 * in Simbody parlance) in the Model. This is more efficient than using a
 * separate CoordinateActuator for each coordinate, but is less flexible (can't
 * specify separate min/max control values for different coordinates). This
 * Actuator is particularly useful for controllers that apply generalized
 * forces to all coordinates.
 */
class OSIMACTUATORS_API AllCoordinatesActuator : public Actuator {
OpenSim_DECLARE_CONCRETE_OBJECT(AllCoordinatesActuator, Actuator);
public:
//==============================================================================
// PROPERTIES
//==============================================================================

//==============================================================================
// PUBLIC METHODS
//==============================================================================
	AllCoordinatesActuator() {}

//==============================================================================
// PRIVATE
//==============================================================================
private:
	//--------------------------------------------------------------------------
	// Implement Force interface
	//--------------------------------------------------------------------------
	void computeForce(const SimTK::State& state, 
					  SimTK::Vector_<SimTK::SpatialVec>& bodyForces, 
					  SimTK::Vector& mobilityForces) const override;

	//--------------------------------------------------------------------------
	// Implement Actuator interface
	//--------------------------------------------------------------------------
    int numControls() const override;
    double getPower(const SimTK::State& state) const override;

//==============================================================================
};	// END of class AllCoordinatesActuator

}; //namespace
//==============================================================================
//==============================================================================

#endif // OPENSIM_ALL_COORDINATES_ACTUATOR_H_


