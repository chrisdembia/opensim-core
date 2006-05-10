#ifndef _SimmMuscle_h_
#define _SimmMuscle_h_

// SimmMuscle.h
// Author: Peter Loan
/* Copyright (c) 2005, Stanford University and Peter Loan.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including 
 * without limitation the rights to use, copy, modify, merge, publish, 
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included 
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


// INCLUDE
#include <iostream>
#include <math.h>
#include <OpenSim/Simulation/rdSimulationDLL.h>
#include <OpenSim/Tools/PropertyDbl.h>
#include <OpenSim/Tools/PropertyObjArray.h>
#include <OpenSim/Tools/Storage.h>
#include <OpenSim/Tools/ArrayPtrs.h>
#include <OpenSim/Tools/XMLDocument.h>
#include <OpenSim/Tools/ScaleSet.h>
#include <OpenSim/Simulation/Model/Actuator.h>
#include <OpenSim/Tools/Function.h>
#include "SimmMusclePoint.h"

namespace OpenSim { 

class SimmKinematicsEngine;
class SimmMuscleGroup;

//=============================================================================
//=============================================================================
/**
 * A class implementing a SIMM muscle.
 *
 * @author Peter Loan
 * @version 1.0
 */
class RDSIMULATION_API SimmMuscle : public Actuator  
{

//=============================================================================
// DATA
//=============================================================================
private:

protected:
	PropertyObjArray _attachmentsProp;
	ArrayPtrs<SimmMusclePoint> &_attachments;

	PropertyDbl _maxIsometricForceProp;
	double &_maxIsometricForce;

	PropertyDbl _optimalFiberLengthProp;
	double &_optimalFiberLength;

	PropertyDbl _tendonSlackLengthProp;
	double &_tendonSlackLength;

	PropertyDbl _pennationAngleProp;
	double &_pennationAngle;

	PropertyDbl _maxContractionVelocityProp;
	double &_maxContractionVelocity;

	PropertyObjArray _tendonForceLengthCurveProp;
	ArrayPtrs<Function> &_tendonForceLengthCurve;

	PropertyObjArray _activeForceLengthCurveProp;
	ArrayPtrs<Function> &_activeForceLengthCurve;

	PropertyObjArray _passiveForceLengthCurveProp;
	ArrayPtrs<Function> &_passiveForceLengthCurve;

	PropertyObjArray _forceVelocityCurveProp;
	ArrayPtrs<Function> &_forceVelocityCurve;

	PropertyStrArray _groupNamesProp;
	Array<std::string>& _groupNames;
	Array<SimmMuscleGroup*> _groups;

//=============================================================================
// METHODS
//=============================================================================
	//--------------------------------------------------------------------------
	// CONSTRUCTION
	//--------------------------------------------------------------------------
public:
	SimmMuscle();
	SimmMuscle(DOMElement *aElement);
	SimmMuscle(const SimmMuscle &aMuscle);
	virtual ~SimmMuscle();
	virtual Object* copy() const;
	virtual Object* copy(DOMElement *aElement) const;

#ifndef SWIG
	SimmMuscle& operator=(const SimmMuscle &aMuscle);
#endif

   void SimmMuscle::copyData(const SimmMuscle &aMuscle);

#ifndef SWIG
	const ArrayPtrs<SimmMusclePoint>& getAttachmentArray() const { return _attachments; }
#endif

	virtual void computeActuation() { }
	virtual void apply() { }

	double getLength(SimmKinematicsEngine* ke) const;
	const Array<std::string>& getGroupNames() const { return _groupNames; }

	/* Register types to be used when reading an SimmModel object from xml file. */
	static void registerTypes();

	void scale(const ScaleSet& aScaleSet);
	void setup(SimmModel* model, SimmKinematicsEngine* ke);

	void writeSIMM(std::ofstream& out) const;

	void peteTest(SimmKinematicsEngine* ke) const;

protected:

private:
	void setNull();
	void setupProperties();
//=============================================================================
};	// END of class SimmMuscle

}; //namespace
//=============================================================================
//=============================================================================

#endif // __SimmMuscle_h__


