// SampleIntegrand.cpp
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
* Copyright (c) 2005, Stanford University. All rights reserved. 
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions
* are met: 
*  - Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer. 
*  - Redistributions in binary form must reproduce the above copyright 
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the distribution. 
*  - Neither the name of the Stanford University nor the names of its 
*    contributors may be used to endorse or promote products derived 
*    from this software without specific prior written permission. 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
* POSSIBILITY OF SUCH DAMAGE. 
*/

/*  
 * Author: Frank C. Anderson 
 */

#include <string>
#include <OpenSim/Tools/Math.h>
#include <OpenSim/Tools/Mtx.h>
#include "SampleIntegrand.h"



using namespace OpenSim;
using namespace std;


//=============================================================================
// CONSTRUCTORS & DESTRUCTOR
//=============================================================================
//_____________________________________________________________________________
/**
 * Constructs an integrad for an Model.
 *
 * @param aModel An instance of an Model.
 */
SampleIntegrand::SampleIntegrand()
{
	setNull();
}

//_____________________________________________________________________________
/**
 * Destructor.
 */
SampleIntegrand::~SampleIntegrand()
{
}


//=============================================================================
// CONSTRUCTION AND DESTRUCTION METHODS
//=============================================================================
//_____________________________________________________________________________
/**
 * Set member variables to their NULL values.
 */
void SampleIntegrand::
setNull()
{
	_stateStore = NULL;
	_ti = 0.0;
	_tf = 0.0;
	_tPrev = 0.0;
	_dtPrev = 0.0;
}



//=============================================================================
// GET & SET
//=============================================================================
//-----------------------------------------------------------------------------
// SIZE
//-----------------------------------------------------------------------------
//____________________________________________________________________________
/**
 * Get the size of the state vector (y) and time derivative of the state
 * vector (dydt).  The size is the number of integrated variables.
 *
 * @return Size of the state vector y.
 * @throws Exception All methods in this base class should be over-ridden.
 */
int SampleIntegrand::
getSize() const
{
	return(3);
}

//-----------------------------------------------------------------------------
// STATE STORAGE
//-----------------------------------------------------------------------------
//_____________________________________________________________________________
/**
 * Set the storage buffer for the integration states.
 */
void SampleIntegrand::
setStateStorage(Storage *aStorage)
{
	_stateStore = aStorage;
}
//_____________________________________________________________________________
/**
 * Get the storage buffer for the integration states.
 */
Storage* SampleIntegrand::
getStateStorage()
{
	return(_stateStore);
}


//=============================================================================
// COMPUTATION
//=============================================================================
//____________________________________________________________________________
/**
 * Compute the state derivatives.
 *
 * @param t Current time.
 * @param y State vector (size = getSize()).
 * @param dydt The time derivative of the state vector (size = getSize()).
 */
void SampleIntegrand::
compute(double t,double y[],double dydt[])
{
	// State 0:  1 Hz
	double w0 = 2.0 * Math::PI * 1.0;
	dydt[0] = w0 * cos(w0*t);

	// State 1:  100 Hz
	double w1 = 2.0 * Math::PI * 10.0;
	dydt[1] = w1 * cos(w1*t);

	// State 2:
	dydt[2] = y[0] + exp(-t)*y[1];
}
//____________________________________________________________________________
/**
 * Compute the Jacobian.
 *
 * @param t Current time.
 * @param y State vector (size = getSize()).
 * @param dydtdy Jacobian.
 */
void SampleIntegrand::
computeJacobian(double t,double y[],double *dydtdy)
{
	int i;

	// WRT Y0
	i = Mtx::ComputeIndex(0,3,0);
	dydtdy[i] = 0.0;
	// WRT Y0
	i = Mtx::ComputeIndex(0,3,1);
	dydtdy[i] = 0.0;
	// WRT Y0
	i = Mtx::ComputeIndex(0,3,2);
	dydtdy[i] = 1.0;

	// WRT Y1
	i = Mtx::ComputeIndex(1,3,0);
	dydtdy[i] = 0.0;
	// WRT Y1
	i = Mtx::ComputeIndex(1,3,1);
	dydtdy[i] = 0.0;
	// WRT Y1
	i = Mtx::ComputeIndex(1,3,2);
	dydtdy[i] = exp(-t);

	// WRT Y2
	i = Mtx::ComputeIndex(2,3,0);
	dydtdy[i] = 0.0;
	// WRT Y2
	i = Mtx::ComputeIndex(2,3,1);
	dydtdy[i] = 0.0;
	// WRT Y2
	i = Mtx::ComputeIndex(2,3,2);
	dydtdy[i] = 0.0;

}


//=============================================================================
// HOOKS
//=============================================================================
//____________________________________________________________________________
/**
 * Initialize the SampleIntegrand at the beginning of an integration.
 *
 * @param ti Initial time of the integration.
 * @param tf Final time of the simulation.
 * @param y Initial values of the states.
 */
void SampleIntegrand::
initialize(int step,double &dt,double ti,double tf,double y[])
{
	// VARIABLES
	_ti = ti;
	_tf = tf;
	_tPrev = _ti;

	// STORE STARTING STATES
	if(_stateStore!=NULL) {
		// ONLY IF NO STATES WERE PREVIOUSLY STORED
		if(_stateStore->getLastStateVector()==NULL) {
			_stateStore->store(step,ti,getSize(),y);
		}
	}
}
//____________________________________________________________________________
/**
 * Perform any desired operations after the last successful integration step.
 *
 * @param step Step number.
 * @param dt Time delta for the next integration step.
 * @param t Current time of the integration.
 * @param y Current values of the states.
 */
void SampleIntegrand::
processAfterStep(int step,double &dt,double t,double y[])
{
	// STORE STATES
	if(_stateStore!=NULL) {
		if(t<_tf) {
			_stateStore->store(step,t,getSize(),y);
		} else {
			_stateStore->append(t,getSize(),y);
		}
	}

	// UPDATE PREVIOUS TIME
	_tPrev = t;
}
